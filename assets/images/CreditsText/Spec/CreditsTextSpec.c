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
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Libgccvb.h>
#include <AnimatedImage.h>
#include <BgmapAnimatedSprite.h>
#include <AnimationState.h>
#include <TitleScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE CreditsTextTiles[];
extern BYTE CreditsTextMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

// a function which defines the frames to play
AnimationFunctionROMSpec CREDITS_TEXT_DEFAULT_ANIM =
{
	// number of frames of this animation function
	7,

	// frames to play in animation
	{2, 2, 2, 3, 3, 4, 4},

	// number of cycles a frame of animation is displayed
	72,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&AnimatedImage_playCreditsAnimation,

	// function's name
	"Default",
};

// a function which defines the frames to play
AnimationFunctionROMSpec CREDITS_TEXT_DEFAULT_ALTERNATIVE_ANIM =
{
	// number of frames of this animation function
	5,

	// frames to play in animation
	{
		0, 1, 2, 3, 4
	},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&AnimatedImage_playCreditsAnimationForTitleScreen,

	// function's name
	"DfltAlt",
};

// an animation spec
AnimationDescriptionROMSpec CREDITS_TEXT_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&CREDITS_TEXT_DEFAULT_ANIM,
		(AnimationFunction*)&CREDITS_TEXT_DEFAULT_ALTERNATIVE_ANIM,
		NULL,
	}
};

CharSetROMSpec CREDITS_TEXT_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	240,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	CreditsTextTiles,
};

TextureROMSpec CREDITS_TEXT_TX =
{
	// charset spec
	(CharSetSpec*)&CREDITS_TEXT_CH,

	// bgmap spec
	CreditsTextMap,

	// cols (max 64)
	16,

	// rows (max 64)
	15,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	1,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec CREDITS_TEXT_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&CREDITS_TEXT_TX,

		// transparent
		false,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const CREDITS_TEXT_SPRITES[] =
{
	&CREDITS_TEXT_SPRITE,
	NULL
};

AnimatedImageROMSpec CREDITS_TEXT_EN =
{	
	{
		{
			// class allocator
			__TYPE(AnimatedImage),

			// children
			NULL,

			// behaviors
			NULL,

			// extra
			NULL,
			
			// sprites
			(SpriteSpec**)CREDITS_TEXT_SPRITES,

			// use z displacement in projection
			false,

			// collision shapes
			(ShapeSpec*)NULL,

			// size
			// if 0, width and height will be inferred from the first sprite's texture's size
			{0, 0, 0},

			// gameworld's character's type
			0,

			// physical specification
			(PhysicalSpecification*)NULL,
		},

		// pointer to the animation spec for the item
		(AnimationDescription*)&CREDITS_TEXT_ANIM,

		// initial animation
		"Default",
	}
};

AnimatedImageROMSpec CREDITS_TEXT_ALTERNATIVE_EN =
{	
	{
		{
			// class allocator
			__TYPE(AnimatedImage),

			// children
			NULL,

			// behaviors
			NULL,

			// extra
			NULL,

			// sprites
			(SpriteSpec**)CREDITS_TEXT_SPRITES,

			// use z displacement in projection
			false,
				
		// collision shapes
			(ShapeSpec*)NULL,

			// size
			// if 0, width and height will be inferred from the first sprite's texture's size
			{0, 0, 0},

			// gameworld's character's type
			0,

			// physical specification
			(PhysicalSpecification*)NULL,
		},

		// pointer to the animation spec for the item
		(AnimationDescription*)&CREDITS_TEXT_ANIM,

		// initial animation
		"DfltAlt",
	}
};
