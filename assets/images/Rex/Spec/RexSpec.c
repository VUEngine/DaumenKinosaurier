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


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE RexLeftTiles[];
extern BYTE RexLeftMap[];
extern BYTE RexMiddleTiles[];
extern BYTE RexMiddleMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

// a function which defines the frames to play
AnimationFunctionROMSpec RexDefaultAnimation =
{
	// number of frames of this animation function
	50,

	// frames to play in animation
	{
		 0,  1,  2,  3,  4,  5,  6,  7,  8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
		26, 27, 28, 29, 30, 31, 32, 33,
		26, 27, 28, 29, 30, 31, 32, 33,
		26, 27, 28, 29, 30, 31, 32, 33,
	},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&AnimatedImage_playBanana,

	// function's name
	"Default",
};

// a function which defines the frames to play
AnimationFunctionROMSpec RexScreamLoopAnimation =
{
	// number of frames of this animation function
	24,

	// frames to play in animation
	{
		0,  1,  2,  3,  4,  5,  
		6,  7,  8, 9, 10, 11, 
		12, 13, 14, 15, 16, 17,
		18, 17, 16, 15, 14, 13,
	},

	// number of cycles a frame of animation is displayed
	6,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"ScreamLoop",
};

// a function which defines the frames to play
AnimationFunctionROMSpec RexRunAnimation =
{
	// number of frames of this animation function
	27,

	// frames to play in animation
	{
		32, 33, 26, 27, 28, 29, 30, 31,
		32, 33, 26, 27, 28, 29, 30, 31,
		32, 33, 26, 27, 28, 29, 30, 31,
		32, 33, 26,
	},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&AnimatedImage_playVertigo,

	// function's name
	"Run",
};

// a function which defines the frames to play
AnimationFunctionROMSpec RexRunLoopAnimation =
{
	// number of frames of this animation function
	8,

	// frames to play in animation
	{26, 27, 28, 29, 30, 31, 32, 33},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"RunLoop",
};

// an animation spec
AnimationDescriptionROMSpec RexAnimation =
{
	// animation functions
	{
		(AnimationFunction*)&RexDefaultAnimation,
		(AnimationFunction*)&RexScreamLoopAnimation,
		(AnimationFunction*)&RexRunAnimation,
		(AnimationFunction*)&RexRunLoopAnimation,

		NULL,
	}
};


/* Left */

CharSetROMSpec RexLCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	459,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char spec
	RexLeftTiles,
};

TextureROMSpec RexLTexture =
{
	// charset spec
	(CharSetSpec*)&RexLCharset,

	// bgmap spec
	RexLeftMap,

	// cols (max 64)
	48,

	// rows (max 64)
	28,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	0,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec RexLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&RexLTexture,

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


/* Right */

CharSetROMSpec RexRCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	454,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char spec
	RexMiddleTiles,
};

TextureROMSpec RexRTexture =
{
	// charset spec
	(CharSetSpec*)&RexRCharset,

	// bgmap spec
	RexMiddleMap,

	// cols (max 64)
	48,

	// rows (max 64)
	28,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	0,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec RexRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&RexRTexture,

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


/* Entity */

BgmapSpriteROMSpec* const RexSprites[] =
{
	&RexLSprite,
	&RexRSprite,
	NULL
};

AnimatedImageROMSpec RexEntity =
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
			(SpriteSpec**)RexSprites,

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
		(AnimationDescription*)&RexAnimation,

		// initial animation
		"Default",
	}
};

AnimatedImageROMSpec RexRunEntity =
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
			(SpriteSpec**)RexSprites,

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
		(AnimationDescription*)&RexAnimation,

		// initial animation
		"Run",
	}
};
