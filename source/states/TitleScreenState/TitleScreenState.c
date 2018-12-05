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

#include <string.h>

#include <Game.h>
#include <Camera.h>
#include <Actor.h>
#include <SoundManager.h>
#include <MessageDispatcher.h>
#include <AnimatedEntity.h>
#include <TitleScreenState.h>
#include <AnimationState.h>
#include <ImageViewerState.h>
#include <CreditsState.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec TITLE_SCREEN_ST;
extern EntitySpec CREDITS_EN;
extern const u16 BLIP_SND[];
extern const u16 SELECT_SND[];
extern const u16 BACK_SND[];


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void TitleScreenState::constructor()
{
	Base::constructor();

	this->currentSelection = kMenuOptionPlayMovie;
	this->currentCreditsFrame = 0;
	this->mode = kModeMenu;
	this->fadeInComplete = false;
	this->isPaused = false;
	this->cursorEntity = NULL;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
	this->nextButtonEntity = NULL;
}

// class's destructor
void TitleScreenState::destructor()
{
	Base::destructor();
}

// state's enter
void TitleScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// register input events to be caught
	KeypadManager::registerInput(KeypadManager::getInstance(), __KEY_PRESSED | __KEY_RELEASED | __KEY_HOLD);

	// load stage
	GameState::loadStage(this, (StageSpec*)&TITLE_SCREEN_ST, NULL, true);

	// start clocks to start animations
	GameState::startClocks(this);

	// reset state
	this->currentSelection = kMenuOptionPlayMovie;
	this->mode = kModeMenu;
	this->fadeInComplete = false;
	this->isPaused = false;

	// get entities
	this->cursorEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Cursor",
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
	this->nextButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Next",
		false
	));

	// initially hide buttons
	Entity::hide(this->resumeButtonEntity);
	Entity::hide(this->backButtonEntity);
	Entity::hide(this->nextButtonEntity);

	// start fade in effect
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))TitleScreenState::onFadeInComplete, // callback function
		Object::safeCast(this) // callback scope
	);
}

void TitleScreenState::processUserInput(UserInput userInput)
{
	if((K_A & userInput.pressedKey) || (K_B & userInput.pressedKey) || (K_STA & userInput.pressedKey))
	{
		if(this->mode == kModeMenu && this->fadeInComplete && (K_A & userInput.pressedKey || K_STA & userInput.pressedKey))
		{
			if(this->currentSelection == kMenuOptionAnimationGallery)
			{
				// disable user input
				Game::disableKeypad(Game::getInstance());

				// play sound
				Vector3D position = {192, 112, 0};
				SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);

				// start fade out effect
				Brightness brightness = (Brightness){0, 0, 0};
				Camera::startEffect(Camera::getInstance(),
					kFadeTo, // effect type
					0, // initial delay (in ms)
					&brightness, // target brightness
					__FADE_DELAY, // delay between fading steps (in ms)
					(void (*)(Object, Object))TitleScreenState::onFadeOutComplete, // callback function
					Object::safeCast(this) // callback scope
				);
			}
			else
			{
				// disable user input
				Game::disableKeypad(Game::getInstance());

				// delete copyright
				Entity copyright = Entity::safeCast(Container::getChildByName(
					Game::getStage(Game::getInstance()),
					"Copyright",
					false
				));

				Entity::releaseSprites(copyright);
				Container::deleteMyself(copyright);

				// delete menu
				Entity menu = Entity::safeCast(Container::getChildByName(
					Game::getStage(Game::getInstance()),
					"Menu",
					false
				));

				Entity::releaseSprites(menu);
				Container::deleteMyself(menu);

				// delete cursor
				Container::deleteMyself(this->cursorEntity);
				this->cursorEntity = NULL;

				// get logo entity from stage
				AnimatedEntity logoEntity = AnimatedEntity::safeCast(Container::getChildByName(
					Game::getStage(Game::getInstance()),
					"Logo",
					false
				));

				// play sound
				Vector3D position = {192, 112, 0};
				SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);

				// play logo's fade out anim
				AnimatedEntity::playAnimation(logoEntity, "FadeOut");

				// after a short delay, handle menu selection
				MessageDispatcher::dispatchMessage(750, Object::safeCast(this), Object::safeCast(this), kMenuSelection, NULL);
			}
		}
		else if(this->mode == kModePlaying && this->currentSelection == kMenuOptionPlayMovie && (K_A & userInput.pressedKey))
		{
			// update internal state
			this->isPaused = !this->isPaused;

			// get image entity from stage
			Container imageEntity = Container::getChildByName(
				Game::getStage(Game::getInstance()),
				"Image",
				false
			);
			if(imageEntity)
			{
				// pause/resume animation
				AnimatedEntity::pauseAnimation(AnimatedEntity::safeCast(imageEntity), this->isPaused);
			}

			// get logo entity from stage
			Container logoEntity = Container::getChildByName(
				Game::getStage(Game::getInstance()),
				"Logo",
				false
			);
			if(logoEntity)
			{
				// pause/resume animation
				AnimatedEntity::pauseAnimation(AnimatedEntity::safeCast(logoEntity), this->isPaused);
			}

			// get ende entity from stage
			Container creditsEntity = Container::getChildByName(
				Game::getStage(Game::getInstance()),
				"CredText",
				false
			);
			if(creditsEntity)
			{
				// pause/resume animation
				AnimatedEntity::pauseAnimation(AnimatedEntity::safeCast(creditsEntity), this->isPaused);
			}

			// update ui
			if (this->isPaused)
			{
				Entity::show(this->resumeButtonEntity);
				Entity::show(this->backButtonEntity);
			}
			else
			{
				Entity::hide(this->resumeButtonEntity);
				Entity::hide(this->backButtonEntity);
			}

			// play sound
			Vector3D position = {192, 112, 0};
			SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
		}
		else if((this->isPaused || (this->mode == kModePlaying && this->currentSelection == kMenuOptionCredits)) && (K_B & userInput.pressedKey))
		{
			// disable user input
			Game::disableKeypad(Game::getInstance());

			// play sound
			Vector3D position = {192, 112, 0};
			SoundManager::playFxSound(SoundManager::getInstance(), BACK_SND, position);

			// start fade out effect
			Brightness brightness = (Brightness){0, 0, 0};
			Camera::startEffect(Camera::getInstance(),
				kFadeTo, // effect type
				0, // initial delay (in ms)
				&brightness, // target brightness
				__FADE_DELAY, // delay between fading steps (in ms)
				(void (*)(Object, Object))TitleScreenState::onFadeOutToTitleComplete, // callback function
				Object::safeCast(this) // callback scope
			);
		}
		else if(this->mode == kModePlaying && this->currentSelection == kMenuOptionCredits && (K_A & userInput.pressedKey))
		{
			switch(this->currentCreditsFrame++)
			{
				case 4:
				{
					Entity::hide(this->nextButtonEntity);
				}
				case 0:
				case 1:
				case 2:
				case 3:
				{
					AnimatedEntity creditsEntity = AnimatedEntity::safeCast(Container::getChildByName(
						Game::getStage(Game::getInstance()),
						"CredText",
						false
					));
					AnimatedEntity::nextFrame(creditsEntity);
					break;
				}
			}

			// play sound
			Vector3D position = {192, 112, 0};
			SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
		}
	}
	else if(this->mode == kModeMenu && (K_LU & userInput.pressedKey || K_RU & userInput.pressedKey || K_LT & userInput.pressedKey))
	{
		// increment/wrap menu selection
		this->currentSelection = (this->currentSelection > 0)
			? this->currentSelection - 1
			: (TITLE_SCREEN_MENU_OPTIONS - 1);

		// adjust cursor position
		Vector3D position = {__PIXELS_TO_METERS(18), __PIXELS_TO_METERS(108 + (this->currentSelection * 12)), 0};
		Entity::setLocalPosition(this->cursorEntity, &position);

		// play sound
		SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
	else if(this->mode == kModeMenu && (K_LD & userInput.pressedKey || K_RD & userInput.pressedKey || K_RT & userInput.pressedKey))
	{
		// decrement/wrap menu selection
		this->currentSelection = (this->currentSelection < (TITLE_SCREEN_MENU_OPTIONS - 1))
			? this->currentSelection + 1
			: 0;

		// adjust cursor position
		Vector3D position = {__PIXELS_TO_METERS(18), __PIXELS_TO_METERS(108 + (this->currentSelection * 12)), 0};
		Entity::setLocalPosition(this->cursorEntity, &position);

		// play sound
		SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
}

// process a telegram
bool TitleScreenState::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kMenuSelection:
		{
			this->mode = kModePlaying;

			switch(this->currentSelection)
			{
				default:
				case kMenuOptionPlayMovie:
				{
					// enable user input
					Game::enableKeypad(Game::getInstance());

					// get logo entity from stage
					AnimatedEntity logoEntity = AnimatedEntity::safeCast(Container::getChildByName(
						Game::getStage(Game::getInstance()),
						"Logo",
						false
					));

					// play "a flipbook by" anim
					AnimatedEntity::playAnimation(logoEntity, "AFlipbBy");

					break;
				}

				case kMenuOptionCredits:
				{
					// remove logo
					Entity logo = Entity::safeCast(Container::getChildByName(
						Game::getStage(Game::getInstance()),
						"Logo",
						false
					));

					Entity::releaseSprites(logo);
					Container::deleteMyself(logo);

					// delayed adding of credits text entity
					MessageDispatcher::dispatchMessage(120, Object::safeCast(this), Object::safeCast(this), kShowCreditsText, NULL);

					break;
				}
			}

			break;
		}

		case kShowCreditsText:
		{
			// enable user input
			Game::enableKeypad(Game::getInstance());

			// add new image entity
			extern EntitySpec CREDITS_TEXT_ALTERNATIVE_EN;
			PositionedEntity POSITIONED_ENTITY = {&CREDITS_TEXT_ALTERNATIVE_EN, {80, 74, -0.003f, 0}, 0, "CredText", NULL, NULL, true};
			Stage::addChildEntity(Game::getStage(Game::getInstance()), &POSITIONED_ENTITY, false);

			// pause credits animation
			AnimatedEntity creditsEntity = AnimatedEntity::safeCast(Container::getChildByName(
				Game::getStage(Game::getInstance()),
				"CredText",
				false
			));
			AnimatedEntity::pauseAnimation(creditsEntity, true);

			// show buttons
			Entity::show(this->nextButtonEntity);
			Entity::show(this->backButtonEntity);

			// reset current credits frame
			this->currentCreditsFrame = 0;

			break;
		}

		case kShowCreditsAnimation:
		{
			// add image entity
			PositionedEntity POSITIONED_ENTITY = {&CREDITS_EN, {192, 112, 0, 0}, 0, "Image", NULL, NULL, true};
			Stage::addChildEntity(Game::getStage(Game::getInstance()), &POSITIONED_ENTITY, false);

			break;
		}
	}
	return true;
}

// handle event
void TitleScreenState::onFadeInComplete(Object eventFirer __attribute__ ((unused)))
{
	this->fadeInComplete = true;
	Game::enableKeypad(Game::getInstance());
}

// handle event
void TitleScreenState::onFadeOutComplete(Object eventFirer __attribute__ ((unused)))
{
	// change to next stage
	switch(this->currentSelection)
	{
		default:
		case kMenuOptionPlayMovie:
			Game::changeState(Game::getInstance(), GameState::safeCast(AnimationState::getInstance()));
			break;
		case kMenuOptionAnimationGallery:
			Game::changeState(Game::getInstance(), GameState::safeCast(ImageViewerState::getInstance()));
			break;
		case kMenuOptionCredits:
			Game::changeState(Game::getInstance(), GameState::safeCast(CreditsState::getInstance()));
			break;
	}
}

// handle event
void TitleScreenState::onFadeOutToTitleComplete(Object eventFirer __attribute__ ((unused)))
{
	// change to next stage
	Game::changeState(Game::getInstance(), GameState::safeCast(this));
}

void TitleScreenState::onAFlipbookByComplete()
{
	// disable user input
	Game::disableKeypad(Game::getInstance());

	// start fade out effect
	Brightness brightness = (Brightness){0, 0, 0};
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		&brightness, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))TitleScreenState::onFadeOutComplete, // callback function
		Object::safeCast(this) // callback scope
	);
}

void TitleScreenState::playCreditsAnimation()
{
	// delete image entity
	Entity image = Entity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Image",
		false
	));

	Entity::releaseSprites(image);
	Container::deleteMyself(Container::safeCast(image));

	// delete credits entity
	Entity creditsText = Entity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"CredText",
		false
	));

	Entity::releaseSprites(creditsText);
	Container::deleteMyself(creditsText);

	// delayed adding of credits animation entity
	MessageDispatcher::dispatchMessage(120, Object::safeCast(TitleScreenState::getInstance()), Object::safeCast(TitleScreenState::getInstance()), kShowCreditsAnimation, NULL);
}
