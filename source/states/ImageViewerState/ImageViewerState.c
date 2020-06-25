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

extern StageROMSpec IMAGE_VIEWER_ST;
extern TextureROMSpec REX_L_TX;
extern TextureROMSpec REX_R_TX;
extern TextureROMSpec BANANA_L_TX;
extern TextureROMSpec BANANA_R_TX;
extern TextureROMSpec VERTIGO_L_TX;
extern TextureROMSpec VERTIGO_R_TX;
extern TextureROMSpec VOLCANO_L_TX;
extern TextureROMSpec VOLCANO_R_TX;
extern AnimationDescriptionROMSpec VOLCANO_ANIM;
extern AnimationDescriptionROMSpec REX_ANIM;
extern AnimationDescriptionROMSpec BANANA_ANIM;
extern AnimationDescriptionROMSpec VERTIGO_ANIM;


//---------------------------------------------------------------------------------------------------------
// 												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void ImageViewerState::constructor()
{
	Base::constructor();

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
void ImageViewerState::destructor()
{
	Base::destructor();
}

// state's enter
void ImageViewerState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(this, (StageSpec*)&IMAGE_VIEWER_ST, NULL, true);

	// enable user input
    Game::enableKeypad(Game::getInstance());

	// start clocks to start animations
	GameState::startClocks(this);

	// reset state
	this->currentAnimation = 0;
	this->isPaused = false;
	this->fadeInComplete = false;

	// get entities
	this->imageEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Image",
		false
	));
	this->titleEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Title",
		false
	));
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
	this->framesButtonEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Game::getStage(Game::getInstance()),
		"Frames",
		false
	));

	// show current animation name
	ImageViewerState::printAnimationName(this);

	// initially hide buttons
	Entity::hide(this->resumeButtonEntity);
	Entity::hide(this->framesButtonEntity);

	// start fade in effect
	Camera::startEffect(Camera::getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))ImageViewerState::onFadeInComplete, // callback function
		Object::safeCast(this) // callback scope
	);
}

void ImageViewerState::processUserInput(UserInput userInput)
{
	if(K_B & userInput.pressedKey)
	{
		// disable user input
		Game::disableKeypad(Game::getInstance());

		// pause/resume animation
		AnimatedEntity::pauseAnimation(this->imageEntity, true);
		AnimatedEntity::pauseAnimation(this->titleEntity, true);
		AnimatedEntity::pauseAnimation(this->pauseButtonEntity, true);
		AnimatedEntity::pauseAnimation(this->resumeButtonEntity, true);
		AnimatedEntity::pauseAnimation(this->backButtonEntity, true);
		AnimatedEntity::pauseAnimation(this->framesButtonEntity, true);

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
			(void (*)(Object, Object))ImageViewerState::onFadeOutComplete, // callback function
			Object::safeCast(this) // callback scope
		);
	}
	else if(K_A & userInput.pressedKey)
	{
		// update internal state
		this->isPaused = !this->isPaused;

		// update ui
		if (this->isPaused)
		{
			Entity::hide(this->pauseButtonEntity);
			Entity::show(this->resumeButtonEntity);
			Entity::show(this->framesButtonEntity);
			ImageViewerState::printFrameNumber(this);
		}
		else
		{
			Entity::show(this->pauseButtonEntity);
			Entity::hide(this->resumeButtonEntity);
			Entity::hide(this->framesButtonEntity);
			ImageViewerState::clearFrameNumber(this);
		}

		// pause/resume animation
		AnimatedEntity::pauseAnimation(this->imageEntity, this->isPaused);
		//AnimatedEntity::pauseAnimation(this->titleEntity, this->isPaused);

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), SELECT_SND, position);
	}
	else if(K_LT & userInput.pressedKey)
	{
		// change current animation number
		this->currentAnimation = (this->currentAnimation > 0)
			? this->currentAnimation - 1
			: (IMAGE_VIEWER_NUMBER_OF_ANIMATIONS - 1);

		// show new animation and update ui
		ImageViewerState::printAnimationName(this);
		ImageViewerState::playAnimation(this);

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
	else if(K_RT & userInput.pressedKey)
	{
		// change current animation number
		this->currentAnimation = (this->currentAnimation < (IMAGE_VIEWER_NUMBER_OF_ANIMATIONS - 1))
			? this->currentAnimation + 1
			: 0;

		// show new animation and update ui
		ImageViewerState::printAnimationName(this);
		ImageViewerState::playAnimation(this);

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
	else if(this->isPaused && (K_LL & userInput.pressedKey || K_RL & userInput.pressedKey))
	{
		AnimatedEntity::previousFrame(this->imageEntity);
		ImageViewerState::printFrameNumber(this);

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
	else if(this->isPaused && (K_LR & userInput.pressedKey || K_RR & userInput.pressedKey))
	{
		AnimatedEntity::nextFrame(this->imageEntity);
		ImageViewerState::printFrameNumber(this);

		// play sound
		//Vector3D position = {192, 112, 0};
		//SoundManager::playFxSound(SoundManager::getInstance(), BLIP_SND, position);
	}
}

void ImageViewerState::printAnimationName()
{
	switch(this->currentAnimation)
	{
		case 0:
		default:
			AnimatedEntity::playAnimation(this->titleEntity, "Volcano");
			break;
		case 1:
			AnimatedEntity::playAnimation(this->titleEntity, "Roooar");
			break;
		case 2:
			AnimatedEntity::playAnimation(this->titleEntity, "WalkCycle");
			break;
		case 3:
			AnimatedEntity::playAnimation(this->titleEntity, "Banana");
			break;
		case 4:
			AnimatedEntity::playAnimation(this->titleEntity, "Extinction");
			break;
	}
}

void ImageViewerState::playAnimation()
{
	// hide screen during transition
	//Camera::startEffect(Camera::getInstance(), kHide);

	// get image entity sprites
	VirtualList entitySprites = Entity::getSprites(this->imageEntity);

	// cycle left and right sprites
	VirtualNode node = VirtualList::begin(entitySprites);
	u8 i = 0;

//	VIPManager::disableInterrupts(VIPManager::getInstance());
	VIPManager::disableDrawing(VIPManager::getInstance());

	// rewrite animation description and play loop animation

	for(i = 0; node; node = VirtualNode::getNext(node), i++)
	{
		// get image entity texture
		Texture texture = Sprite::getTexture(VirtualNode::getData(node));

		Texture::setSpec(texture, ImageViewerState::getTexture(this, i));
	}

	AnimatedEntity::setAnimationDescription(this->imageEntity, ImageViewerState::getAnimationDescription(this));
	AnimatedEntity::playAnimation(this->imageEntity, ImageViewerState::getAnimationName(this));

	// force CHAR memory defragmentation to prevent memory depletion
	CharSetManager::writeCharSets(CharSetManager::getInstance());

	node = VirtualList::begin(entitySprites);

	for(i = 0; node; node = VirtualNode::getNext(node), i++)
	{
		Sprite sprite = Sprite::safeCast(VirtualNode::getData(node));
		Texture texture = Sprite::getTexture(sprite);

		Sprite::render(sprite, 31, false);
		Texture::update(texture);
	}

	VIPManager::enableDrawing(VIPManager::getInstance());

	// force unpaused
	this->isPaused = false;
	Entity::show(this->pauseButtonEntity);
	Entity::hide(this->resumeButtonEntity);
	Entity::hide(this->framesButtonEntity);
	ImageViewerState::clearFrameNumber(this);

	// show screen again
	//Camera::startEffect(Camera::getInstance(), kShow);
}

TextureSpec* ImageViewerState::getTexture(u8 textureNumber)
{
	switch(this->currentAnimation)
	{
		case 0:
		default:
			{
				return (textureNumber == 0) ? (TextureSpec*)&VOLCANO_L_TX : (TextureSpec*)&VOLCANO_R_TX;
				break;
			}
		case 1:
		case 2:
			{
				return (textureNumber == 0) ? (TextureSpec*)&REX_L_TX : (TextureSpec*)&REX_R_TX;
				break;
			}
		case 3:
			{
				return (textureNumber == 0) ? (TextureSpec*)&BANANA_L_TX : (TextureSpec*)&BANANA_R_TX;
				break;
			}
		case 4:
			{
				return (textureNumber == 0) ? (TextureSpec*)&VERTIGO_L_TX : (TextureSpec*)&VERTIGO_R_TX;
				break;
			}
	}
}

AnimationDescription* ImageViewerState::getAnimationDescription()
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

char* ImageViewerState::getAnimationName()
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

void ImageViewerState::printFrameNumber()
{
	s8 actualFrame = AnimatedEntity::getActualFrame(this->imageEntity) + 1;
	int numberOfFrames = AnimatedEntity::getNumberOfFrames(this->imageEntity);

	Printing::text(Printing::getInstance(), "00/00", 40, 7, "Number");
	Printing::int(Printing::getInstance(), actualFrame, (actualFrame < 10) ? 41 : 40, 7, "Number");
	Printing::int(Printing::getInstance(), numberOfFrames, (numberOfFrames < 10) ? 44 : 43, 7, "Number");
}

void ImageViewerState::clearFrameNumber()
{
	Printing::text(Printing::getInstance(), ".....", 40, 7, "Number");
}

// handle event
void ImageViewerState::onFadeInComplete(Object eventFirer __attribute__ ((unused)))
{
	this->fadeInComplete = true;
}

// handle event
void ImageViewerState::onFadeOutComplete(Object eventFirer __attribute__ ((unused)))
{
	// change to next stage
	Game::changeState(Game::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}
