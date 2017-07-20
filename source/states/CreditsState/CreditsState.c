/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2017 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
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
#include <Screen.h>
#include <SoundManager.h>
#include <CreditsState.h>
#include <KeypadManager.h>
#include <AnimatedInGameEntity.h>
#include <MessageDispatcher.h>
#include <TitleScreenState.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef CREDITS_ST;
extern const u16 SELECT_SND[];
extern const u16 BACK_SND[];


//---------------------------------------------------------------------------------------------------------
// 												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void CreditsState_destructor(CreditsState this);
static void CreditsState_constructor(CreditsState this);
static void CreditsState_enter(CreditsState this, void* owner);
static void CreditsState_onFadeInComplete(CreditsState this, Object eventFirer);
static void CreditsState_onFadeOutComplete(CreditsState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(CreditsState, GameState);
__SINGLETON(CreditsState);


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) CreditsState_constructor(CreditsState this)
{
	// construct base
	__CONSTRUCT_BASE(GameState);

	// init state
	this->remaining = __CREDITS_LENGTH;
	this->isPaused = false;
	this->fadeInComplete = false;
	this->pauseButtonEntity = NULL;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
}

// class's destructor
static void CreditsState_destructor(CreditsState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void CreditsState_enter(CreditsState this, void* owner __attribute__ ((unused)))
{
	// call base
	GameState_enter(__SAFE_CAST(GameState, this), owner);

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&CREDITS_ST, NULL, true);

	// enable user input
    Game_enableKeypad(Game_getInstance());

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// reset state
	this->remaining = __CREDITS_LENGTH;
	this->isPaused = false;
	this->fadeInComplete = false;

	// get entities
	this->pauseButtonEntity = __SAFE_CAST(AnimatedInGameEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Pause",
		false
	));
	this->resumeButtonEntity = __SAFE_CAST(AnimatedInGameEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Resume",
		false
	));
	this->backButtonEntity = __SAFE_CAST(AnimatedInGameEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Back",
		false
	));

	// initially hide buttons
	Entity_hide(__SAFE_CAST(Entity, this->pauseButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->backButtonEntity));

	// show image
	Screen_startEffect(Screen_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))CreditsState_onFadeInComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

void CreditsState_processUserInput(CreditsState this, UserInput userInput)
{
	if(userInput.pressedKey & ~K_PWR)
	{
		if(K_A & userInput.pressedKey)
		{
			// update internal state
			this->isPaused = !this->isPaused;

			// get image entity from stage
			Container imageEntity = Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"Image",
				false
			);

			// pause/resume animation
			AnimatedInGameEntity_pauseAnimation(__SAFE_CAST(AnimatedInGameEntity, imageEntity), this->isPaused);

			// update ui
			if (this->isPaused)
			{
				Entity_hide(__SAFE_CAST(Entity, this->pauseButtonEntity));
				Entity_show(__SAFE_CAST(Entity, this->resumeButtonEntity));
				Entity_show(__SAFE_CAST(Entity, this->backButtonEntity));
			}
			else
			{
				Entity_hide(__SAFE_CAST(Entity, this->pauseButtonEntity));
				Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
				Entity_hide(__SAFE_CAST(Entity, this->backButtonEntity));
			}

			// play sound
			VBVec3D position = {FTOFIX19_13(192), FTOFIX19_13(112), 0};
			SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);
		}
		else if(this->fadeInComplete && this->isPaused)
		{
			// disable user input
			Game_disableKeypad(Game_getInstance());

			// play sound
			VBVec3D position = {FTOFIX19_13(192), FTOFIX19_13(112), 0};
			SoundManager_playFxSound(SoundManager_getInstance(), BACK_SND, position);

			// start fade out effect
			Brightness brightness = (Brightness){0, 0, 0};
			Screen_startEffect(Screen_getInstance(),
				kFadeTo, // effect type
				0, // initial delay (in ms)
				&brightness, // target brightness
				__FADE_DELAY, // delay between fading steps (in ms)
				(void (*)(Object, Object))CreditsState_onFadeOutComplete, // callback function
				__SAFE_CAST(Object, this) // callback scope
			);
		}
	}
}

void CreditsState_execute(CreditsState this, void* owner __attribute__ ((unused)))
{
	ASSERT(this, "CreditsState::execute: null this");

	GameState_execute(__SAFE_CAST(GameState, this), owner);

	if(!this->isPaused)
	{
		if(this->remaining > 0)
		{
			this->remaining--;
		}
		else
		{
			this->remaining = __CREDITS_LENGTH;

			// disable user input
			Game_disableKeypad(Game_getInstance());

			// start fade out effect
			Brightness brightness = (Brightness){0, 0, 0};
			Screen_startEffect(Screen_getInstance(),
				kFadeTo, // effect type
				0, // initial delay (in ms)
				&brightness, // target brightness
				__FADE_DELAY, // delay between fading steps (in ms)
				(void (*)(Object, Object))CreditsState_onFadeOutComplete, // callback function
				__SAFE_CAST(Object, this) // callback scope
			);
		}
	}
}

// handle event
static void CreditsState_onFadeInComplete(CreditsState this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "CreditsState::onFadeInComplete: null this");

	this->fadeInComplete = true;
}

// handle event
static void CreditsState_onFadeOutComplete(CreditsState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "CreditsState::onFadeOutComplete: null this");

	// change to next stage
	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
}
