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

extern StageROMSpec TitleScreenSt;
extern EntitySpec CreditsEntity;


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
	GameState::loadStage(this, (StageSpec*)&TitleScreenSt, NULL, true, false);

	// start clocks to start animations
	GameState::startClocks(this);

	// reset state
	this->currentSelection = kMenuOptionPlayMovie;
	this->mode = kModeMenu;
	this->fadeInComplete = false;
	this->isPaused = false;

	// get entities
	this->cursorEntity = AnimatedEntity::safeCast(Container::getChildByName(
		this->stage,
		"Cursor",
		false
	));
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
	this->nextButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		this->stage,
		"Next",
		false
	));

	// initially hide buttons
	Entity::hide(this->resumeButtonEntity);
	Entity::hide(this->backButtonEntity);
	Entity::hide(this->nextButtonEntity);

	// start fade in effect
	Camera::startEffect(Camera::getInstance(), kHide);
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
				//Vector3D position = {192, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);

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
					this->stage,
					"Copyright",
					false
				));

				Entity::releaseSprites(copyright);
				Container::deleteMyself(copyright);

				// delete menu
				Entity menu = Entity::safeCast(Container::getChildByName(
					this->stage,
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
					this->stage,
					"Logo",
					false
				));

				// play sound
				//Vector3D position = {192, 112, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);

				// play logo's fade out anim
				AnimatedEntity::playAnimation(logoEntity, "FadeOut");

				// after a short delay, handle menu selection
				MessageDispatcher::dispatchMessage(750, Object::safeCast(this), Object::safeCast(this), kMessageMenuSelection, NULL);
			}
		}
		else if(this->mode == kModePlaying && this->currentSelection == kMenuOptionPlayMovie && (K_A & userInput.pressedKey))
		{
			// update internal state
			this->isPaused = !this->isPaused;

			// get image entity from stage
			Container imageEntity = Container::getChildByName(
				this->stage,
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
				this->stage,
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
				this->stage,
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
			//Vector3D position = {192, 112, 0};
			//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
		}
		else if((this->isPaused || (this->mode == kModePlaying && this->currentSelection == kMenuOptionCredits)) && (K_B & userInput.pressedKey))
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
						this->stage,
						"CredText",
						false
					));
					AnimatedEntity::nextFrame(creditsEntity);
					break;
				}
			}

			// play sound
			//Vector3D position = {192, 112, 0};
			//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
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
		//SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
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
		//SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
}

// process a telegram
bool TitleScreenState::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kMessageMenuSelection:
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
						this->stage,
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
						this->stage,
						"Logo",
						false
					));

					Entity::releaseSprites(logo);
					Container::deleteMyself(logo);

					// delayed adding of credits text entity
					MessageDispatcher::dispatchMessage(120, Object::safeCast(this), Object::safeCast(this), kMessageShowCreditsText, NULL);

					break;
				}
			}

			break;
		}

		case kMessageShowCreditsText:
		{
			// enable user input
			Game::enableKeypad(Game::getInstance());

			// add new image entity
			extern EntitySpec CreditsTextAlternativeEntity;
			PositionedEntity POSITIONED_ENTITY = {&CreditsTextAlternativeEntity, {80, 74, -0.003f, 0}, 0, "CredText", NULL, NULL, true};
			Stage::addChildEntity(this->stage, &POSITIONED_ENTITY, false);

			// pause credits animation
			AnimatedEntity creditsEntity = AnimatedEntity::safeCast(Container::getChildByName(
				this->stage,
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

		case kMessageShowCreditsAnimation:
		{
			// add image entity
			PositionedEntity POSITIONED_ENTITY = {&CreditsEntity, {192, 112, 0, 0}, 0, "Image", NULL, NULL, true};
			Stage::addChildEntity(this->stage, &POSITIONED_ENTITY, false);

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
//			Game::changeState(Game::getInstance(), GameState::safeCast(CreditsState::getInstance()));
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
		this->stage,
		"Image",
		false
	));

	if(!isDeleted(image))
	{
		// Cannot add the Credits right aways since there is not enough CHAR memory
		Entity::addEventListener(image, Object::safeCast(this), (EventListener)&TitleScreenState::onImageDeleted, kEventContainerDeleted);
		// delete ende entity
		Entity::deleteMyself(image);
	}
}

void TitleScreenState::onImageDeleted(Object eventFirer __attribute__((unused)))
{
/*
	// delete credits entity
	Entity creditsText = Entity::safeCast(Container::getChildByName(
		this->stage,
		"CredText",
		false
	));
*/
	// delayed adding of credits animation entity
	MessageDispatcher::dispatchMessage(120, Object::safeCast(TitleScreenState::getInstance()), Object::safeCast(TitleScreenState::getInstance()), kMessageShowCreditsAnimation, NULL);
}
