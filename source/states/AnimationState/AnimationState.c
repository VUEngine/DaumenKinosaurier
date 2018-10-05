/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev<jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Game.h>
#include <Camera.h>
#include <SoundManager.h>
#include <AnimationState.h>
#include <KeypadManager.h>
#include <MessageDispatcher.h>
#include <AnimatedEntity.h>
#include <CustomCameraEffectManager.h>
#include <TitleScreenState.h>
#include <CreditsState.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef ANIMATION_ST;

extern const u16 SELECT_SND[];
extern const u16 BACK_SND[];
extern const u16 ROAR_SND[];
extern const u16 STEP_SND[];
extern const u16 CRASH_SND[];

extern EntityDefinition REX_AG;
extern EntityDefinition BANANA_AG;
extern EntityDefinition REX_RUN_AG;
extern EntityDefinition VERTIGO_AG;
extern EntityDefinition VOLCANO_AG;
extern EntityDefinition ENDE_AG;
extern EntityDefinition CREDITS_TEXT_AG;
extern EntityDefinition CREDITS_AG;


//---------------------------------------------------------------------------------------------------------
// 												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void AnimationState_destructor(AnimationState this);
static void AnimationState_constructor(AnimationState this);
static void AnimationState_enter(AnimationState this, void* owner);
static void AnimationState_onFadeOutToTitleComplete(AnimationState this, Object eventFirer);
static void AnimationState_onFadeOutToCreditsComplete(AnimationState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(AnimationState, GameState);
__SINGLETON_DYNAMIC(AnimationState);


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) AnimationState_constructor(AnimationState this)
{
	// construct base
	__CONSTRUCT_BASE(GameState);

	// init state
	this->currentSequence = kAnimationSequenceRexScream;
	this->isPaused = false;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
	//this->framesButtonEntity = NULL;
}

// class's destructor
static void AnimationState_destructor(AnimationState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void AnimationState_enter(AnimationState this, void* owner __attribute__ ((unused)))
{
	// call base
	GameState_enter(__SAFE_CAST(GameState, this), owner);

	// set the custom screen effect manager
	Camera_setCameraEffectManager(Camera_getInstance(), __SAFE_CAST(CameraEffectManager, CustomCameraEffectManager_getInstance()));

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&ANIMATION_ST, NULL, true);

	// enable user input
    Game_enableKeypad(Game_getInstance());

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// reset state
	this->currentSequence = kAnimationSequenceRexScream;
	this->isPaused = false;

	// get entities from stage
	this->resumeButtonEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Resume",
		false
	));
	this->backButtonEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Back",
		false
	));
	/*
	this->framesButtonEntity = __SAFE_CAST(Entity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Frames",
		false
	));
	*/

	// initially hide buttons
	Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->backButtonEntity));
	//Entity_hide(this->framesButtonEntity);

	// show image
	Camera_startEffect(Camera_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		NULL, // callback function
		NULL // callback scope
	);
}

void AnimationState_processUserInput(AnimationState this, UserInput userInput)
{
	if(userInput.pressedKey & ~K_PWR)
	{
		if(K_A & userInput.pressedKey)
		{
			// get image entity from stage
			AnimatedEntity imageEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"Image",
				false
			));

			// update internal state
			this->isPaused = !this->isPaused;

			// pause/resume animation
			AnimatedEntity_pauseAnimation(imageEntity, this->isPaused);

			// stop all sound playback
			SoundManager_stopAllSound(SoundManager_getInstance());

			// get ende entity from stage
			Container endeEntity = Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"Ende",
				false
			);

			if(endeEntity)
			{
				// pause/resume animation
				AnimatedEntity_pauseAnimation(__SAFE_CAST(AnimatedEntity, endeEntity), this->isPaused);
			}

			// get ende entity from stage
			Container creditsEntity = Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"Credits",
				false
			);

			if(creditsEntity)
			{
				// pause/resume animation
				AnimatedEntity_pauseAnimation(__SAFE_CAST(AnimatedEntity, creditsEntity), this->isPaused);
			}

			// update ui
			if (this->isPaused)
			{
				Entity_show(__SAFE_CAST(Entity, this->resumeButtonEntity));
				Entity_show(__SAFE_CAST(Entity, this->backButtonEntity));
				//Entity_show(this->framesButtonEntity);
			}
			else
			{
				Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
				Entity_hide(__SAFE_CAST(Entity, this->backButtonEntity));
				//Entity_hide(this->framesButtonEntity);
			}

			// play sound
			Vector3D position = {__F_TO_FIX10_6(192), __F_TO_FIX10_6(112), 0};
			SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);
		}
		else if(this->isPaused && (K_B & userInput.pressedKey))
		{
			// disable user input
			Game_disableKeypad(Game_getInstance());

			// play sound
			Vector3D position = {__F_TO_FIX10_6(192), __F_TO_FIX10_6(112), 0};
			SoundManager_playFxSound(SoundManager_getInstance(), BACK_SND, position);

			// start fade out effect
			Brightness brightness = (Brightness){0, 0, 0};
			Camera_startEffect(Camera_getInstance(),
				kFadeTo, // effect type
				0, // initial delay (in ms)
				&brightness, // target brightness
				__FADE_DELAY, // delay between fading steps (in ms)
				(void (*)(Object, Object))AnimationState_onFadeOutToTitleComplete, // callback function
				__SAFE_CAST(Object, this) // callback scope
			);
		}
	}
}

void AnimationState_execute(AnimationState this, void* owner)
{
	// do not play any sound when paused
	if (this->isPaused)
	{
		return;
	}

	// call base
	GameState_execute(__SAFE_CAST(GameState, this), owner);

	// get image entity from stage
	AnimatedEntity imageEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Image",
		false
	));

	// get current animation frame
	s8 currentFrame = AnimatedEntity_getActualFrame(imageEntity);

	// play sounds according to current animation sequence and current animation frame
	switch (this->currentSequence)
	{
		case kAnimationSequenceRexScream:
		{
			// scream
			if (currentFrame == 9)
			{
				Vector3D position = {__F_TO_FIX10_6(192), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), ROAR_SND, position);
			}
			// right step
			else if (currentFrame == 27 || currentFrame == 35 || currentFrame == 43)
			{
				Vector3D position = {__F_TO_FIX10_6(44), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 31 || currentFrame == 39 || currentFrame == 47)
			{
				Vector3D position = {__F_TO_FIX10_6(344), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceRexRun:
		{
			// right step
			if (currentFrame == 3 || currentFrame == 11 || currentFrame == 19)
			{
				Vector3D position = {__F_TO_FIX10_6(44), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 7 || currentFrame == 15 || currentFrame == 23)
			{
				Vector3D position = {__F_TO_FIX10_6(344), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceBanana:
		{
			// right step
			if (currentFrame == 2 || currentFrame == 10)
			{
				Vector3D position = {__F_TO_FIX10_6(44), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 6 || currentFrame == 14)
			{
				Vector3D position = {__F_TO_FIX10_6(344), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceVertigo:
		{
			// right step
			if (currentFrame == 0 || currentFrame == 8)
			{
				Vector3D position = {__F_TO_FIX10_6(44), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 4 || currentFrame == 12)
			{
				Vector3D position = {__F_TO_FIX10_6(344), __F_TO_FIX10_6(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceVolcano:
		{
			break;
		}
		case kAnimationSequenceCredits:
		{
			break;
		}
	}
}

void AnimationState_setCurrentAnimationSequence(AnimationState this, u8 currentSequence)
{
	this->currentSequence = currentSequence;
}

void AnimationState_playBanana(AnimationState this __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::playBanana: null this");

	// delete image entity
	Container imageEntity = Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Image",
		false
	);
	Container_deleteMyself(imageEntity);

	// add new image entity
	PositionedEntityROMDef positionedEntity[] =
	{
		{&BANANA_AG, {192, 112, 0, 0}, 0, "Image", NULL, NULL, true},
		{NULL,{0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

	// update current animation sequence
	AnimationState_setCurrentAnimationSequence(AnimationState_getInstance(), kAnimationSequenceBanana);
}

void AnimationState_playRexRun(AnimationState this __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::playRexRun: null this");

	// delete image entity
	Container imageEntity = Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Image",
		false
	);
	Container_deleteMyself(imageEntity);

	// add new image entity
	PositionedEntityROMDef positionedEntity[] =
	{
		{&REX_RUN_AG, {192, 112, 0, 0}, 0, "Image", NULL, NULL, true},
		{NULL,{0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

	// update current animation sequence
	AnimationState_setCurrentAnimationSequence(AnimationState_getInstance(), kAnimationSequenceRexRun);
}

void AnimationState_playVertigo(AnimationState this __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::playVertigo: null this");

	// delete image entity
	Container imageEntity = Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Image",
		false
	);
	Container_deleteMyself(imageEntity);

	// add new image entity
	PositionedEntityROMDef positionedEntity[] =
	{
		{&VERTIGO_AG, {192, 112, 0, 0}, 0, "Image", NULL, NULL, true},
		{NULL,{0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

	// update current animation sequence
	AnimationState_setCurrentAnimationSequence(AnimationState_getInstance(), kAnimationSequenceVertigo);
}

void AnimationState_playVolcanoEnd(AnimationState this __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::playVolcanoEnd: null this");

	// delete image entity
	Container imageEntity = Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Image",
		false
	);
	Container_deleteMyself(imageEntity);

	// add new image entity
	PositionedEntityROMDef positionedEntity[] =
	{
		{&VOLCANO_AG, {192, 112, 0, 0}, 0, "Image", NULL, NULL, true},
		{NULL,{0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

	// update current animation sequence
	AnimationState_setCurrentAnimationSequence(AnimationState_getInstance(), kAnimationSequenceVolcano);

	// screen shake
	Camera_startEffect(Camera_getInstance(), kShake, 800);

	// play sound
	Vector3D position = {__F_TO_FIX10_6(192), __F_TO_FIX10_6(112), 0};
	SoundManager_playFxSound(SoundManager_getInstance(), CRASH_SND, position);

	// play "ende" fade in animation
	PositionedEntityROMDef endeEntity[] =
	{
		{&ENDE_AG, {86, 54, -0.001f, 0}, 0, "Ende", NULL, NULL, true},
		{NULL,{0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage_addChildEntity(Game_getStage(Game_getInstance()), endeEntity, false);
}

void AnimationState_playCreditsText(AnimationState this __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::playCreditsText: null this");

	// delete ende entity
	Container_deleteMyself(Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Ende",
		false
	));

	// add credits entity
	PositionedEntityROMDef positionedEntity[] =
	{
		{&CREDITS_TEXT_AG, {80, 74, 0, 0}, 0, "Credits", NULL, NULL, true},
		{NULL,{0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

	// update current animation sequence
	AnimationState_setCurrentAnimationSequence(AnimationState_getInstance(), kAnimationSequenceCredits);
}

void AnimationState_playCreditsAnimation(AnimationState this __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::playCreditsAnimation: null this");

	// start delayed fade out effect
	Brightness brightness = (Brightness){0, 0, 0};
	Camera_startEffect(Camera_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		&brightness, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))AnimationState_onFadeOutToCreditsComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

// handle event
static void AnimationState_onFadeOutToTitleComplete(AnimationState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::onFadeOutToTitleComplete: null this");

	// change to next stage
	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
}

// handle event
static void AnimationState_onFadeOutToCreditsComplete(AnimationState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "AnimationState::onFadeOutToCreditsComplete: null this");

	// change to next stage
	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, CreditsState_getInstance()));
}
