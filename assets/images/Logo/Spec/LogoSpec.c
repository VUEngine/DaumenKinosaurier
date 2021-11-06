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
#include <TitleScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE LogoLeftTiles[];
extern BYTE LogoLeftMap[];
extern BYTE LogoRightTiles[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

// a function which defines the frames to play
AnimationFunctionROMSpec LogoDefaultAnimation =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{0, 1},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"Default",
};

// a function which defines the frames to play
AnimationFunctionROMSpec LogoFadeOutAnimation =
{
	// number of frames of this animation function
	4,

	// frames to play in animation
	{2, 3, 4, 5},

	// number of cycles a frame of animation is displayed
	4,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"FadeOut",
};

// a function which defines the frames to play
AnimationFunctionROMSpec LogoAFlipbookByAnimation =
{
	// number of frames of this animation function
	24,

	// frames to play in animation
	{
		6, 7, 8, 9,
		6, 7, 8, 9,
		6, 7, 8, 9,
		6, 7, 8, 9,
		6, 7, 8, 9,
		6, 7, 8, 9,
	},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	(EventListener)&AnimatedImage_onAFlipbookByComplete,

	// function's name
	"AFlipbBy",
};

// an animation spec
AnimationDescriptionROMSpec LogoAnimation =
{
	// animation functions
	{
		(AnimationFunction*)&LogoDefaultAnimation,
		(AnimationFunction*)&LogoFadeOutAnimation,
		(AnimationFunction*)&LogoAFlipbookByAnimation,
		NULL,
	}
};


/* LEFT */

CharSetROMSpec LogoLCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	190,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	LogoLeftTiles,
};

TextureROMSpec LogoLTexture =
{
	// charset spec
	(CharSetSpec*)&LogoLCharset,

	// bgmap spec
	LogoLeftMap,

	// cols (max 64)
	19,

	// rows (max 64)
	10,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	3,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec LogoLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&LogoLTexture,

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
	__WORLD_LON,
};


/* RIGHT */

CharSetROMSpec LogoRCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	190,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	LogoRightTiles,
};

TextureROMSpec LogoRTexture =
{
	// charset spec
	(CharSetSpec*)&LogoRCharset,

	// bgmap spec
	LogoLeftMap,

	// cols (max 64)
	19,

	// rows (max 64)
	10,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	3,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec LogoRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&LogoRTexture,

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
	__WORLD_RON,
};


/* ENTITY */

BgmapSpriteROMSpec* const LogoSprites[] =
{
	&LogoLSprite,
	&LogoRSprite,
	NULL
};

AnimatedImageROMSpec LogoEntity =
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
			(SpriteSpec**)LogoSprites,

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
		(AnimationDescription*)&LogoAnimation,

		// initial animation
		"Default",
	}
};
