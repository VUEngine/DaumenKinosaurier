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
#include <Screen.h>
#include <CharSetManager.h>
#include <SoundManager.h>
#include <Printing.h>
#include <ImageViewerState.h>
#include <KeypadManager.h>
#include <MessageDispatcher.h>
#include <TitleScreenState.h>
#include <AnimatedEntity.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef IMAGE_VIEWER_ST;
extern TextureROMDef REX_L_TX;
extern TextureROMDef REX_R_TX;
extern TextureROMDef BANANA_L_TX;
extern TextureROMDef BANANA_R_TX;
extern TextureROMDef VERTIGO_L_TX;
extern TextureROMDef VERTIGO_R_TX;
extern TextureROMDef VOLCANO_L_TX;
extern TextureROMDef VOLCANO_R_TX;
extern AnimationDescriptionROMDef VOLCANO_ANIM;
extern AnimationDescriptionROMDef REX_ANIM;
extern AnimationDescriptionROMDef BANANA_ANIM;
extern AnimationDescriptionROMDef VERTIGO_ANIM;
extern const u16 BLIP_SND[];
extern const u16 BACK_SND[];
extern const u16 SELECT_SND[];


//---------------------------------------------------------------------------------------------------------
// 												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void ImageViewerState_destructor(ImageViewerState this);
static void ImageViewerState_constructor(ImageViewerState this);
static void ImageViewerState_enter(ImageViewerState this, void* owner);
static void ImageViewerState_onFadeOutComplete(ImageViewerState this, Object eventFirer);
static void ImageViewerState_onFadeInComplete(ImageViewerState this, Object eventFirer);
TextureDefinition* ImageViewerState_getTexture(ImageViewerState this, u8 textureNumber);
AnimationDescription* ImageViewerState_getAnimationDescription(ImageViewerState this);
char* ImageViewerState_getAnimationName(ImageViewerState this);
void ImageViewerState_printAnimationName(ImageViewerState this);
void ImageViewerState_playAnimation(ImageViewerState this);
void ImageViewerState_printFrameNumber(ImageViewerState this);
void ImageViewerState_clearFrameNumber(ImageViewerState this);


//---------------------------------------------------------------------------------------------------------
// 											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(ImageViewerState, GameState);
__SINGLETON_DYNAMIC(ImageViewerState);


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) ImageViewerState_constructor(ImageViewerState this)
{
	// construct base
	__CONSTRUCT_BASE(GameState);

	// init state
	this->currentAnimation = 0;
	this->isPaused = false;
	this->fadeInComplete = false;
	this->imageEntity = NULL;
	this->titleEntity = NULL;
	this->pauseButtonEntity = NULL;
	this->resumeButtonEntity = NULL;
	this->backButtonEntity = NULL;
	this->framesButtonEntity = NULL;
}

// class's destructor
static void ImageViewerState_destructor(ImageViewerState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void ImageViewerState_enter(ImageViewerState this, void* owner __attribute__ ((unused)))
{
	// call base
	GameState_enter(__SAFE_CAST(GameState, this), owner);

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&IMAGE_VIEWER_ST, NULL, true);

	// enable user input
    Game_enableKeypad(Game_getInstance());

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// reset state
	this->currentAnimation = 0;
	this->isPaused = false;
	this->fadeInComplete = false;

	// get entities
	this->imageEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Image",
		false
	));
	this->titleEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Title",
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
	this->framesButtonEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Frames",
		false
	));

	// show current animation name
	ImageViewerState_printAnimationName(this);

	// initially hide buttons
	Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->framesButtonEntity));

	// start fade in effect
	Screen_startEffect(Screen_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))ImageViewerState_onFadeInComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

void ImageViewerState_processUserInput(ImageViewerState this, UserInput userInput)
{
	if(userInput.pressedKey & ~K_PWR)
	{
		if(K_B & userInput.pressedKey)
		{
			// disable user input
			Game_disableKeypad(Game_getInstance());

			// pause/resume animation
			AnimatedEntity_pauseAnimation(__SAFE_CAST(AnimatedEntity, this->imageEntity), true);
			AnimatedEntity_pauseAnimation(this->titleEntity, true);
			AnimatedEntity_pauseAnimation(this->pauseButtonEntity, true);
			AnimatedEntity_pauseAnimation(this->resumeButtonEntity, true);
			AnimatedEntity_pauseAnimation(this->backButtonEntity, true);
			AnimatedEntity_pauseAnimation(this->framesButtonEntity, true);

			// play sound
			VBVec3D position = {192, 112, 0};
			SoundManager_playFxSound(SoundManager_getInstance(), BACK_SND, position);

			// start fade out effect
			Brightness brightness = (Brightness){0, 0, 0};
			Screen_startEffect(Screen_getInstance(),
				kFadeTo, // effect type
				0, // initial delay (in ms)
				&brightness, // target brightness
				__FADE_DELAY, // delay between fading steps (in ms)
				(void (*)(Object, Object))ImageViewerState_onFadeOutComplete, // callback function
				__SAFE_CAST(Object, this) // callback scope
			);
		}
		else if(K_A & userInput.pressedKey)
		{
			// update internal state
			this->isPaused = !this->isPaused;

			// update ui
			if (this->isPaused)
			{
				Entity_hide(__SAFE_CAST(Entity, this->pauseButtonEntity));
				Entity_show(__SAFE_CAST(Entity, this->resumeButtonEntity));
				Entity_show(__SAFE_CAST(Entity, this->framesButtonEntity));
				ImageViewerState_printFrameNumber(this);
			}
			else
			{
				Entity_show(__SAFE_CAST(Entity, this->pauseButtonEntity));
				Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
				Entity_hide(__SAFE_CAST(Entity, this->framesButtonEntity));
				ImageViewerState_clearFrameNumber(this);
			}

			// pause/resume animation
			AnimatedEntity_pauseAnimation(__SAFE_CAST(AnimatedEntity, this->imageEntity), this->isPaused);
			//AnimatedEntity_pauseAnimation(this->titleEntity, this->isPaused);

			// play sound
			VBVec3D position = {192, 112, 0};
			SoundManager_playFxSound(SoundManager_getInstance(), SELECT_SND, position);
		}
		else if(K_LT & userInput.pressedKey)
		{
			// change current animation number
			this->currentAnimation = (this->currentAnimation > 0)
				? this->currentAnimation - 1
				: (IMAGE_VIEWER_NUMBER_OF_ANIMATIONS - 1);

			// show new animation and update ui
			ImageViewerState_printAnimationName(this);
			ImageViewerState_playAnimation(this);

			// play sound
			VBVec3D position = {192, 112, 0};
			SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
		}
		else if(K_RT & userInput.pressedKey)
		{
			// change current animation number
			this->currentAnimation = (this->currentAnimation < (IMAGE_VIEWER_NUMBER_OF_ANIMATIONS - 1))
				? this->currentAnimation + 1
				: 0;

			// show new animation and update ui
			ImageViewerState_printAnimationName(this);
			ImageViewerState_playAnimation(this);

			// play sound
			VBVec3D position = {192, 112, 0};
			SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
		}
		else if(this->isPaused && (K_LL & userInput.pressedKey || K_RL & userInput.pressedKey))
		{
			AnimatedEntity_previousFrame(this->imageEntity);
			ImageViewerState_printFrameNumber(this);

			// play sound
			//VBVec3D position = {192, 112, 0};
			//SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
		}
		else if(this->isPaused && (K_LR & userInput.pressedKey || K_RR & userInput.pressedKey))
		{
			AnimatedEntity_nextFrame(this->imageEntity);
			ImageViewerState_printFrameNumber(this);

			// play sound
			//VBVec3D position = {192, 112, 0};
			//SoundManager_playFxSound(SoundManager_getInstance(), BLIP_SND, position);
		}
	}
}

void ImageViewerState_printAnimationName(ImageViewerState this)
{
	switch(this->currentAnimation)
	{
		case 0:
		default:
			AnimatedEntity_playAnimation(this->titleEntity, "Volcano");
			break;
		case 1:
			AnimatedEntity_playAnimation(this->titleEntity, "Roooar");
			break;
		case 2:
			AnimatedEntity_playAnimation(this->titleEntity, "WalkCycle");
			break;
		case 3:
			AnimatedEntity_playAnimation(this->titleEntity, "Banana");
			break;
		case 4:
			AnimatedEntity_playAnimation(this->titleEntity, "Extinction");
			break;
	}
}

void ImageViewerState_playAnimation(ImageViewerState this)
{
	// hide screen during transition
	//Screen_startEffect(Screen_getInstance(), kHide);

	// get image entity sprites
	VirtualList entitySprites = Entity_getSprites(__SAFE_CAST(Entity, this->imageEntity));

	// cycle left and right sprites
	VirtualNode node = VirtualList_begin(entitySprites);
	u8 i = 0;
	for(i = 0; node; node = VirtualNode_getNext(node), i++)
	{
		// get image entity texture
		Texture entityTexture = Sprite_getTexture(__SAFE_CAST(Sprite, VirtualNode_getData(node)));

		// rewrite texture definition
		Texture_setDefinition(entityTexture, ImageViewerState_getTexture(this, i));
	}

	// force CHAR memory defragmentation to prevent memory depletion
	CharSetManager_defragment(CharSetManager_getInstance());

	// rewrite animation description and play loop animation
	AnimatedEntity_setAnimationDescription(this->imageEntity, ImageViewerState_getAnimationDescription(this));
	AnimatedEntity_playAnimation(this->imageEntity, ImageViewerState_getAnimationName(this));

	// force unpaused
	this->isPaused = false;
	Entity_show(__SAFE_CAST(Entity, this->pauseButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->resumeButtonEntity));
	Entity_hide(__SAFE_CAST(Entity, this->framesButtonEntity));
	ImageViewerState_clearFrameNumber(this);

	// show screen again
	//Screen_startEffect(Screen_getInstance(), kShow);
}

TextureDefinition* ImageViewerState_getTexture(ImageViewerState this, u8 textureNumber)
{
	switch(this->currentAnimation)
	{
		case 0:
		default:
			{
				return (textureNumber == 0) ? (TextureDefinition*)&VOLCANO_L_TX : (TextureDefinition*)&VOLCANO_R_TX;
				break;
			}
		case 1:
		case 2:
			{
				return (textureNumber == 0) ? (TextureDefinition*)&REX_L_TX : (TextureDefinition*)&REX_R_TX;
				break;
			}
		case 3:
			{
				return (textureNumber == 0) ? (TextureDefinition*)&BANANA_L_TX : (TextureDefinition*)&BANANA_R_TX;
				break;
			}
		case 4:
			{
				return (textureNumber == 0) ? (TextureDefinition*)&VERTIGO_L_TX : (TextureDefinition*)&VERTIGO_R_TX;
				break;
			}
	}
}

AnimationDescription* ImageViewerState_getAnimationDescription(ImageViewerState this)
{
	switch(this->currentAnimation)
	{
		case 0:
		default:
			{
				return (AnimationDescription*)&VOLCANO_ANIM;
				break;
			}
		case 1:
		case 2:
			{
				return (AnimationDescription*)&REX_ANIM;
				break;
			}
		case 3:
			{
				return (AnimationDescription*)&BANANA_ANIM;
				break;
			}
		case 4:
			{
				return (AnimationDescription*)&VERTIGO_ANIM;
				break;
			}
	}
}

char* ImageViewerState_getAnimationName(ImageViewerState this)
{
	switch(this->currentAnimation)
	{
		case 0:
		case 3:
		case 4:
		default:
			{
				return "Loop";
				break;
			}
		case 1:
			{
				return "ScreamLoop";
				break;
			}
		case 2:
			{
				return "RunLoop";
				break;
			}
	}
}

void ImageViewerState_printFrameNumber(ImageViewerState this)
{
	ASSERT(this, "ImageViewerState::printFrameNumber: null this");

	s8 actualFrame = AnimatedEntity_getActualFrame(this->imageEntity) + 1;
	int numberOfFrames = AnimatedEntity_getNumberOfFrames(this->imageEntity);

	Printing_text(Printing_getInstance(), "00/00", 40, 7, NULL);
	Printing_int(Printing_getInstance(), actualFrame, (actualFrame < 10) ? 41 : 40, 7, NULL);
	Printing_int(Printing_getInstance(), numberOfFrames, (numberOfFrames < 10) ? 44 : 43, 7, NULL);
}

void ImageViewerState_clearFrameNumber(ImageViewerState this __attribute__ ((unused)))
{
	ASSERT(this, "ImageViewerState::clearFrameNumber: null this");

	Printing_text(Printing_getInstance(), ".....", 40, 7, NULL);
}

// handle event
static void ImageViewerState_onFadeInComplete(ImageViewerState this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "ImageViewerState::onFadeInComplete: null this");

	this->fadeInComplete = true;
}

// handle event
static void ImageViewerState_onFadeOutComplete(ImageViewerState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "ImageViewerState::onFadeOutComplete: null this");

	// change to next stage
	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
}
