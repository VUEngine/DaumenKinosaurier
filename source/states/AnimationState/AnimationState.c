/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
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
#include <AnimatedImage.h>
#include <CustomCameraEffectManager.h>
#include <TitleScreenState.h>
#include <CreditsState.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec ANIMATION_ST;

extern EntitySpec REX_EN;
extern EntitySpec BANANA_EN;
extern EntitySpec REX_RUN_EN;
extern EntitySpec VERTIGO_EN;
extern EntitySpec VOLCANO_EN;
extern EntitySpec ENDE_EN;
extern EntitySpec CREDITS_TEXT_EN;
extern EntitySpec CREDITS_EN;


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void AnimationState::constructor()
{
	Base::constructor();

	// init state
	this->currentSequence = kAnimationSequenceRexScream;
	this->isPaused = false;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
	//this->framesButtonEntity = NULL;

	this->stream = false;
	this->transform = false;
	this->updatePhysics = false;
	this->processCollisions = false;
}

// class's destructor
void AnimationState::destructor()
{
	Base::destructor();
}

// state's enter
void AnimationState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// set the custom screen effect manager
	Camera::setCameraEffectManager(Camera::getInstance(), CameraEffectManager::safeCast(CustomCameraEffectManager_getInstance()));

	// load stage
	GameState::loadStage(this, (StageSpec*)&ANIMATION_ST, NULL, true);

	// enable user input
    Game::enableKeypad(Game::getInstance());

	// start clocks to start animations
	GameState::startClocks(this);

	// reset state
	this->currentSequence = kAnimationSequenceRexScream;
	this->isPaused = false;

	// get entities from stage
	this->resumeButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		this->stage,
		"Resume",
		false
	));
	this->backButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		this->stage,
		"Back",
		false
	));
	/*
	this->framesButtonEntity = Entity::safeCast(Container::getChildByName(
		this->stage,
		"Frames",
		false
	));
	*/

	// initially hide buttons
	Entity::hide(this->resumeButtonEntity);
	Entity::hide(this->backButtonEntity);
	//Entity::hide(this->framesButtonEntity);

	// show image
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		NULL, // callback function
		NULL // callback scope
	);
}

void AnimationState::processUserInput(UserInput userInput)
{
	if(K_A & userInput.pressedKey)
	{
		// get image entity from stage
		AnimatedEntity imageEntity = AnimatedEntity::safeCast(Container::getChildByName(
			this->stage,
			"Image",
			false
		));

		// update internal state
		this->isPaused = !this->isPaused;

		// pause/resume animation
		AnimatedEntity::pauseAnimation(imageEntity, this->isPaused);

		// stop all sound playback
		SoundManager::stopAllSounds(SoundManager::getInstance());

		// get ende entity from stage
		Container endeEntity = Container::getChildByName(
			this->stage,
			"Ende",
			false
		);

		if(endeEntity)
		{
			// pause/resume animation
			AnimatedEntity::pauseAnimation(endeEntity, this->isPaused);
		}

		// get ende entity from stage
		Container creditsEntity = Container::getChildByName(
			this->stage,
			"Credits",
			false
		);

		if(creditsEntity)
		{
			// pause/resume animation
			AnimatedEntity::pauseAnimation(creditsEntity, this->isPaused);
		}

		// update ui
		if (this->isPaused)
		{
			Entity::show(this->resumeButtonEntity);
			Entity::show(this->backButtonEntity);
			//Entity::show(this->framesButtonEntity);
		}
		else
		{
			Entity::hide(this->resumeButtonEntity);
			Entity::hide(this->backButtonEntity);
			//Entity::hide(this->framesButtonEntity);
		}

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
	}
	else if(this->isPaused && (K_B & userInput.pressedKey))
	{
		// disable user input
		Game::disableKeypad(Game::getInstance());

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), BACK_SND, position);

		// start fade out effect
		Brightness brightness = (Brightness){0, 0, 0};
		Camera::startEffect(Camera::getInstance(),
			kFadeTo, // effect type
			0, // initial delay (in ms)
			&brightness, // target brightness
			__FADE_DELAY, // delay between fading steps (in ms)
			(void (*)(Object, Object))AnimationState::onFadeOutToTitleComplete, // callback function
			Object::safeCast(this) // callback scope
		);
	}
}

void AnimationState::execute(void* owner)
{
	// do not play any sound when paused
	if (this->isPaused)
	{
		return;
	}

	// call base
	Base::execute(this, owner);

	// get image entity from stage
	AnimatedEntity imageEntity = AnimatedEntity::safeCast(Container::getChildByName(
		this->stage,
		"Image",
		false
	));

	// get current animation frame
	s8 currentFrame = AnimatedEntity::getActualFrame(imageEntity);

	// play sounds according to current animation sequence and current animation frame
	switch (this->currentSequence)
	{
		case kAnimationSequenceRexScream:
		{
			// scream
			if (currentFrame == 9)
			{
				//Vector3D position = {192, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), ROAR_SND, position);
			}
			// right step
			else if (currentFrame == 27 || currentFrame == 35 || currentFrame == 43)
			{
				//Vector3D position = {44, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 31 || currentFrame == 39 || currentFrame == 47)
			{
				//Vector3D position = {344, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceRexRun:
		{
			// right step
			if (currentFrame == 3 || currentFrame == 11 || currentFrame == 19)
			{
				//Vector3D position = {44, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 7 || currentFrame == 15 || currentFrame == 23)
			{
				//Vector3D position = {344, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceBanana:
		{
			// right step
			if (currentFrame == 2 || currentFrame == 10)
			{
				//Vector3D position = {44, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 6 || currentFrame == 14)
			{
				//Vector3D position = {344, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			break;
		}
		case kAnimationSequenceVertigo:
		{
			// right step
			if (currentFrame == 0 || currentFrame == 8)
			{
				//Vector3D position = {44, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
			}
			// left step
			else if (currentFrame == 4 || currentFrame == 12)
			{
				//Vector3D position = {344, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), STEP_SND, position);
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

void AnimationState::setCurrentAnimationSequence(u8 currentSequence)
{
	this->currentSequence = currentSequence;
}

void AnimationState::playBanana()
{
	// delete image entity
	AnimatedImage imageEntity = AnimatedImage::safeCast(Stage::getChildByName(
		this->stage,
		"Image",
		false
	));

	extern AnimationDescription BANANA_ANIM;

	AnimatedImage::changeSpec(imageEntity, &BANANA_ANIM, "Default");

	// update current animation sequence
	AnimationState::setCurrentAnimationSequence(AnimationState::getInstance(), kAnimationSequenceBanana);
}

void AnimationState::playRexRun()
{
	// delete image entity
	AnimatedImage imageEntity = AnimatedImage::safeCast(Stage::getChildByName(
		this->stage,
		"Image",
		false
	));

	extern AnimationDescription REX_ANIM;

	AnimatedImage::changeSpec(imageEntity, &REX_ANIM, "Run");

	// update current animation sequence
	AnimationState::setCurrentAnimationSequence(AnimationState::getInstance(), kAnimationSequenceRexRun);
}

void AnimationState::playVertigo()
{
	// delete image entity
	AnimatedImage imageEntity = AnimatedImage::safeCast(Stage::getChildByName(
		this->stage,
		"Image",
		false
	));

	extern AnimationDescription VERTIGO_ANIM;

	AnimatedImage::changeSpec(imageEntity, &VERTIGO_ANIM, "Default");

	// update current animation sequence
	AnimationState::setCurrentAnimationSequence(AnimationState::getInstance(), kAnimationSequenceVertigo);
}

void AnimationState::playVolcanoEnd()
{
	// delete image entity
	AnimatedImage imageEntity = AnimatedImage::safeCast(Stage::getChildByName(
		this->stage,
		"Image",
		false
	));

	extern AnimationDescription VOLCANO_ANIM;

	AnimatedImage::changeSpec(imageEntity, &VOLCANO_ANIM, "Loop");

	// update current animation sequence
	AnimationState::setCurrentAnimationSequence(AnimationState::getInstance(), kAnimationSequenceVolcano);

	// screen shake
	Camera::startEffect(Camera::getInstance(), kShake, 800);

	// play sound
	//Vector3D position = {192, 112, 0};
	//SoundManager::playFxSound(SoundManager::getInstance(), CRASH_SND, position);

	// play "ende" fade in animation
	PositionedEntityROMSpec endeEntity[] =
	{
		{&ENDE_EN, {86, 54, -0.001f, 0}, 0, "Ende", NULL, NULL, true},
		{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
	};
	Stage::addChildEntity(this->stage, endeEntity, false);
}

void AnimationState::playCreditsText()
{
	AnimatedEntity ende = AnimatedEntity::safeCast(
			Container::getChildByName(
			this->stage,
			"Ende",
			false
		)
	);

	if(!isDeleted(ende))
	{
		// Cannot add the Credits right aways since there is not enough CHAR memory
		AnimatedEntity::addEventListener(ende, Object::safeCast(this), (EventListener)&AnimationState::onEndeDeleted, kEventContainerDeleted);
		// delete ende entity
		AnimatedEntity::deleteMyself(ende);
	}
}

void AnimationState::onEndeDeleted(Object eventFirer __attribute__((unused)))
{
	// add credits entity
	PositionedEntity POSITIONED_ENTITY = {&CREDITS_TEXT_EN, {80, 74, 0, 0}, 0, "Credits", NULL, NULL, true};
	Stage::addChildEntity(this->stage, &POSITIONED_ENTITY, false);

	// update current animation sequence
	AnimationState::setCurrentAnimationSequence(AnimationState::getInstance(), kAnimationSequenceCredits);
}

void AnimationState::playCreditsAnimation()
{
	// start delayed fade out effect
	Brightness brightness = (Brightness){0, 0, 0};
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		&brightness, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))AnimationState::onFadeOutToCreditsComplete, // callback function
		Object::safeCast(this) // callback scope
	);
}

// handle event
void AnimationState::onFadeOutToTitleComplete(Object eventFirer __attribute__ ((unused)))
{
	// change to next stage
	Game::changeState(Game::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}

// handle event
void AnimationState::onFadeOutToCreditsComplete(Object eventFirer __attribute__ ((unused)))
{
	// change to next stage
	Game::changeState(Game::getInstance(), GameState::safeCast(CreditsState::getInstance()));
}
