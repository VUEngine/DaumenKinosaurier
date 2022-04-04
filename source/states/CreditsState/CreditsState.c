/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Game.h>
#include <Camera.h>
#include <SoundManager.h>
#include <CreditsState.h>
#include <KeypadManager.h>
#include <AnimatedEntity.h>
#include <MessageDispatcher.h>
#include <TitleScreenState.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec CreditsSt;


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void CreditsState::constructor()
{
	Base::constructor();

	// init state
	this->remaining = __CREDITS_LENGTH;
	this->isPaused = false;
	this->fadeInComplete = false;
	this->pauseButtonEntity = NULL;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
}

// class's destructor
void CreditsState::destructor()
{
	Base::destructor();
}

// state's enter
void CreditsState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(this, (StageSpec*)&CreditsSt, NULL, true, false);

	// enable user input
    Game::enableKeypad(Game::getInstance());

	// start clocks to start animations
	GameState::startClocks(this);

	// reset state
	this->remaining = __CREDITS_LENGTH;
	this->isPaused = false;
	this->fadeInComplete = false;

	// get entities
	this->pauseButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Pause",
		false
	));
	this->resumeButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Resume",
		false
	));
	this->backButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Back",
		false
	));

	// initially hide buttons
	Entity::hide(this->pauseButtonEntity);
	Entity::hide(this->resumeButtonEntity);
	Entity::hide(this->backButtonEntity);

	// show image
	Camera::startEffect(Camera::getInstance(), kHide);
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))CreditsState::onFadeInComplete, // callback function
		Object::safeCast(this) // callback scope
	);
}

void CreditsState::processUserInput(UserInput userInput)
{
	if(K_A & userInput.pressedKey)
	{
		// update internal state
		this->isPaused = !this->isPaused;

		// get image entity from stage
		Container imageEntity = Container::getChildByName(
			Game::getStage(Game::getInstance()),
			"Image",
			false
		);

		// pause/resume animation
		AnimatedEntity::pauseAnimation(imageEntity, this->isPaused);

		// update ui
		if (this->isPaused)
		{
			Entity::hide(this->pauseButtonEntity);
			Entity::show(this->resumeButtonEntity);
			Entity::show(this->backButtonEntity);
		}
		else
		{
			Entity::hide(this->pauseButtonEntity);
			Entity::hide(this->resumeButtonEntity);
			Entity::hide(this->backButtonEntity);
		}

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
	}
	else if(this->fadeInComplete && this->isPaused)
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
			(void (*)(Object, Object))CreditsState::onFadeOutComplete, // callback function
			Object::safeCast(this) // callback scope
		);
	}
}

void CreditsState::execute(void* owner)
{
	Base::execute(this, owner);

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
			Game::disableKeypad(Game::getInstance());

			// start fade out effect
			Brightness brightness = (Brightness){0, 0, 0};
			Camera::startEffect(Camera::getInstance(),
				kFadeTo, // effect type
				0, // initial delay (in ms)
				&brightness, // target brightness
				__FADE_DELAY, // delay between fading steps (in ms)
				(void (*)(Object, Object))CreditsState::onFadeOutComplete, // callback function
				Object::safeCast(this) // callback scope
			);
		}
	}
}

// handle event
void CreditsState::onFadeInComplete(Object eventFirer __attribute__ ((unused)))
{
	this->fadeInComplete = true;
}

// handle event
void CreditsState::onFadeOutComplete(Object eventFirer __attribute__ ((unused)))
{
	// change to next stage
	Game::changeState(Game::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}
