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

#include <string.h>

#include <Game.h>
#include <Screen.h>
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

extern StageROMDef TITLE_SCREEN_ST;
extern EntityDefinition CREDITS_AG;
extern const u16 BLIP_SND[];
extern const u16 SELECT_SND[];
extern const u16 BACK_SND[];


//---------------------------------------------------------------------------------------------------------
// 												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void TitleScreenState_destructor(TitleScreenState this);
static void TitleScreenState_constructor(TitleScreenState this);
static void TitleScreenState_enter(TitleScreenState this, void* owner);
static void TitleScreenState_onFadeInComplete(TitleScreenState this, Object eventFirer);
static void TitleScreenState_onFadeOutToTitleComplete(TitleScreenState this, Object eventFirer);
static void TitleScreenState_onFadeOutComplete(TitleScreenState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(TitleScreenState, GameState);
__SINGLETON_DYNAMIC(TitleScreenState);


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) TitleScreenState_constructor(TitleScreenState this)
{
	__CONSTRUCT_BASE(GameState);

	this->currentSelection = kMenuOptionPlayMovie;
	this->currentCreditsFrame = 0;
	this->mode = kModeMenu;
	this->fadeInComplete = false;
	this->isPaused = false;
	this->cursorEntity = NULL;
	this->pauseButtonEntity = NULL;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
	this->nextButtonEntity = NULL;
}

// class's destructor
static void TitleScreenState_destructor(TitleScreenState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void TitleScreenState_enter(TitleScreenState this, void* owner __attribute__ ((unused)))
{
	// call base
	__CALL_BASE_METHOD(GameState, enter, this, owner);

	// enable user input
	Game_enableKeypad(Game_getInstance());

	// register input events to be caught
	KeypadManager_registerInput(KeypadManager_getInstance(), __KEY_PRESSED | __KEY_RELEASED | __KEY_HOLD);

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&TITLE_SCREEN_ST, NULL, true);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// reset state
	this->currentSelection = kMenuOptionPlayMovie;
	this->mode = kModeMenu;
	this->fadeInComplete = false;
	this->isPaused = false;

	// get entities
	this->cursorEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Cursor",
		false
	));
	this->pauseButtonEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Pause",
		false
	));
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
	this->nextButtonEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Next",
		false
	));

	// initially hide buttons
	Entity_hide(__SAFE_CAST(Entity, this->pauseButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->backButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->nextButtonEntity));

	// start fade in effect
	Screen_startEffect(Screen_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))TitleScreenState_onFadeInComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

void TitleScreenState_processUserInput(TitleScreenState this, UserInput userInput)
{
	if(userInput.pressedKey & ~K_PWR)
	{

		if((K_A & userInput.pressedKey) || (K_B & userInput.pressedKey) || (K_STA & userInput.pressedKey))
		{
			if(this->mode == kModeMenu && this->fadeInComplete && (K_A & userInput.pressedKey || K_STA & userInput.pressedKey))
			{
				if(this->currentSelection == kMenuOptionAnimationGallery)
				{
					// disable user input
					Game_disableKeypad(Game_getInstance());

					// play sound
					VBVec3D position = {__F_TO_FIX19_13(192), __F_TO_FIX19_13(112), 0};
					SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);

					// start fade out effect
					Brightness brightness = (Brightness){0, 0, 0};
					Screen_startEffect(Screen_getInstance(),
						kFadeTo, // effect type
						0, // initial delay (in ms)
						&brightness, // target brightness
						__FADE_DELAY, // delay between fading steps (in ms)
						(void (*)(Object, Object))TitleScreenState_onFadeOutComplete, // callback function
						__SAFE_CAST(Object, this) // callback scope
					);
				}
				else
				{
					// disable user input
					Game_disableKeypad(Game_getInstance());

					// delete copyright
					Entity copyright = __SAFE_CAST(Entity, Container_getChildByName(
						__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
						"Copyright",
						false
					));

					Entity_releaseSprites(copyright, true);
					Container_deleteMyself(__SAFE_CAST(Container, copyright));

					// delete menu
					Entity menu = __SAFE_CAST(Entity, Container_getChildByName(
						__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
						"Menu",
						false
					));

					Entity_releaseSprites(menu, true);
					Container_deleteMyself(__SAFE_CAST(Container, menu));

					// delete cursor
					Container_deleteMyself(__SAFE_CAST(Container, this->cursorEntity));
					this->cursorEntity = NULL;

					// get logo entity from stage
					AnimatedEntity logoEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
						__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
						"Logo",
						false
					));

					// play sound
					VBVec3D position = {__F_TO_FIX19_13(192), __F_TO_FIX19_13(112), 0};
					SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);

					// play logo's fade out anim
					AnimatedEntity_playAnimation(logoEntity, "FadeOut");

					// after a short delay, handle menu selection
					MessageDispatcher_dispatchMessage(750, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kMenuSelection, NULL);
				}
			}
			else if(this->mode == kModePlaying && this->currentSelection == kMenuOptionPlayMovie && (K_A & userInput.pressedKey))
			{
				// update internal state
				this->isPaused = !this->isPaused;

				// get image entity from stage
				Container imageEntity = Container_getChildByName(
					__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
					"Image",
					false
				);
				if(imageEntity) {
					// pause/resume animation
					AnimatedEntity_pauseAnimation(__SAFE_CAST(AnimatedEntity, imageEntity), this->isPaused);
				}

				// get logo entity from stage
				Container logoEntity = Container_getChildByName(
					__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
					"Logo",
					false
				);
				if(logoEntity)
				{
					// pause/resume animation
					AnimatedEntity_pauseAnimation(__SAFE_CAST(AnimatedEntity, logoEntity), this->isPaused);
				}

				// get ende entity from stage
				Container creditsEntity = Container_getChildByName(
					__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
					"CredText",
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
				VBVec3D position = {__F_TO_FIX19_13(192), __F_TO_FIX19_13(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);
			}
			else if((this->isPaused || (this->mode == kModePlaying && this->currentSelection == kMenuOptionCredits)) && (K_B & userInput.pressedKey))
			{
				// disable user input
				Game_disableKeypad(Game_getInstance());

				// play sound
				VBVec3D position = {__F_TO_FIX19_13(192), __F_TO_FIX19_13(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), BACK_SND, position);

				// start fade out effect
				Brightness brightness = (Brightness){0, 0, 0};
				Screen_startEffect(Screen_getInstance(),
					kFadeTo, // effect type
					0, // initial delay (in ms)
					&brightness, // target brightness
					__FADE_DELAY, // delay between fading steps (in ms)
					(void (*)(Object, Object))TitleScreenState_onFadeOutToTitleComplete, // callback function
					__SAFE_CAST(Object, this) // callback scope
				);
			}
			else if(this->mode == kModePlaying && this->currentSelection == kMenuOptionCredits && (K_A & userInput.pressedKey))
			{
				switch(this->currentCreditsFrame++)
				{
					case 4:
					{
						Entity_hide(__SAFE_CAST(Entity, this->nextButtonEntity));
					}
					case 0:
					case 1:
					case 2:
					case 3:
					{
						AnimatedEntity creditsEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
							__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
							"CredText",
							false
						));
						AnimatedEntity_nextFrame(creditsEntity);
						break;
					}
				}

				// play sound
				VBVec3D position = {__F_TO_FIX19_13(192), __F_TO_FIX19_13(112), 0};
				SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);
			}
		}
		else if(this->mode == kModeMenu && (K_LU & userInput.pressedKey || K_RU & userInput.pressedKey || K_LT & userInput.pressedKey))
		{
			// increment/wrap menu selection
			this->currentSelection = (this->currentSelection > 0)
				? this->currentSelection - 1
				: (TITLE_SCREEN_MENU_OPTIONS - 1);

			// adjust cursor position
			VBVec3D position = {__F_TO_FIX19_13(18), __F_TO_FIX19_13(108 + (this->currentSelection * 12)), 0};
			Actor_setPosition(__SAFE_CAST(Actor, this->cursorEntity), &position);

			// play sound
			SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
		}
		else if(this->mode == kModeMenu && (K_LD & userInput.pressedKey || K_RD & userInput.pressedKey || K_RT & userInput.pressedKey))
		{
			// decrement/wrap menu selection
			this->currentSelection = (this->currentSelection < (TITLE_SCREEN_MENU_OPTIONS - 1))
				? this->currentSelection + 1
				: 0;

			// adjust cursor position
			VBVec3D position = {__F_TO_FIX19_13(18), __F_TO_FIX19_13(108 + (this->currentSelection * 12)), 0};
			Actor_setPosition(__SAFE_CAST(Actor, this->cursorEntity), &position);

			// play sound
			SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
		}
	}
}

// process a telegram
bool TitleScreenState_handleMessage(TitleScreenState this __attribute__ ((unused)), Telegram telegram)
{
	ASSERT(this, "TitleScreenState::handleMessage: null this");

	switch(Telegram_getMessage(telegram))
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
					Game_enableKeypad(Game_getInstance());

					// get logo entity from stage
					AnimatedEntity logoEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
						__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
						"Logo",
						false
					));

					// play "a flipbook by" anim
					AnimatedEntity_playAnimation(logoEntity, "AFlipbBy");

					break;
				}

				case kMenuOptionCredits:
				{
					Entity logo = __SAFE_CAST(Entity, Container_getChildByName(
						__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
						"Logo",
						false
					));

					Entity_releaseSprites(logo, true);
					Container_deleteMyself(__SAFE_CAST(Container, logo));

					// delayed adding of credits text entity
					MessageDispatcher_dispatchMessage(1, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kShowCreditsText, NULL);

					break;
				}
			}

			break;
		}

		case kShowCreditsText:
		{
			// enable user input
			Game_enableKeypad(Game_getInstance());

			// add new image entity
			extern EntityDefinition CREDITS_TEXT_ALTERNATIVE_AG;
			PositionedEntityROMDef positionedEntity[] =
			{
				{&CREDITS_TEXT_ALTERNATIVE_AG,	{__F_TO_FIX19_13(80),  __F_TO_FIX19_13(74), 	__F_TO_FIX19_13(-0.003f)},    0, "CredText", NULL, NULL, true},
				{NULL,{0,0,0}, 0, NULL, NULL, NULL, false},
			};
			Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

			// pause credits animation
			AnimatedEntity creditsEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"CredText",
				false
			));
			AnimatedEntity_pauseAnimation(creditsEntity, true);

			// show buttons
			Entity_show(__SAFE_CAST(Entity, this->nextButtonEntity));
			Entity_show(__SAFE_CAST(Entity, this->backButtonEntity));

			// reset current credits frame
			this->currentCreditsFrame = 0;

			break;
		}

		case kShowCreditsAnimation:
		{
			// delete image entity
			Entity image = __SAFE_CAST(Entity, Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"Image",
				false
			));

			Entity_releaseSprites(image, true);
			Container_deleteMyself(__SAFE_CAST(Container, image));

			// delete credits entity
			Entity creditsText = __SAFE_CAST(Entity, Container_getChildByName(
				__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
				"CredText",
				false
			));

			Entity_releaseSprites(creditsText, true);
			Container_deleteMyself(__SAFE_CAST(Container, creditsText));

			// add image entity
			PositionedEntityROMDef positionedEntity[] =
			{
				{&CREDITS_AG,	{__F_TO_FIX19_13(192),  __F_TO_FIX19_13(112), 	__F_TO_FIX19_13(0)},    0, "Image", NULL, NULL, true},
				{NULL,{0,0,0}, 0, NULL, NULL, NULL, false},
			};
			Stage_addChildEntity(Game_getStage(Game_getInstance()), positionedEntity, false);

			break;
		}
	}
	return true;
}

// handle event
static void TitleScreenState_onFadeInComplete(TitleScreenState this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "TitleScreenState::onFadeInComplete: null this");

	this->fadeInComplete = true;
}

// handle event
static void TitleScreenState_onFadeOutComplete(TitleScreenState this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "TitleScreenState::onFadeOutComplete: null this");

	// change to next stage
	switch(this->currentSelection)
	{
		default:
		case kMenuOptionPlayMovie:
			Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, AnimationState_getInstance()));
			break;
		case kMenuOptionAnimationGallery:
			Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, ImageViewerState_getInstance()));
			break;
		case kMenuOptionCredits:
			Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, CreditsState_getInstance()));
			break;
	}
}

// handle event
static void TitleScreenState_onFadeOutToTitleComplete(TitleScreenState this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "TitleScreenState::onFadeOutComplete: null this");

	// change to next stage
	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, this));
}

void TitleScreenState_onAFlipbookByComplete(TitleScreenState this)
{
	ASSERT(this, "TitleScreenState::playCreditsAnimation: null this");

	// disable user input
	Game_disableKeypad(Game_getInstance());

	// start fade out effect
	Brightness brightness = (Brightness){0, 0, 0};
	Screen_startEffect(Screen_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		&brightness, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))TitleScreenState_onFadeOutComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

void TitleScreenState_playCreditsAnimation(TitleScreenState this __attribute__ ((unused)))
{
	ASSERT(this, "TitleScreenState::playCreditsAnimation: null this");

	// delayed adding of credits animation entity
	MessageDispatcher_dispatchMessage(1, __SAFE_CAST(Object, TitleScreenState_getInstance()), __SAFE_CAST(Object, TitleScreenState_getInstance()), kShowCreditsAnimation, NULL);
}
