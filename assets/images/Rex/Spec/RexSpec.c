/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
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

extern uint32 RexLeftTiles[];
extern uint32 RexLeftTilesFrameOffsets[];
extern uint16 RexLeftMap[];
extern uint32 RexMiddleTiles[];
extern uint32 RexMiddleTilesFrameOffsets[];
extern uint16 RexMiddleMap[];


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

	// pointer to the frames offsets
	RexLeftTilesFrameOffsets,
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

	// pointer to the frames offsets
	RexMiddleTilesFrameOffsets,
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
