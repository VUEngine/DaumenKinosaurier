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

extern uint32 EndeLeftTiles[];
extern uint32 EndeLeftTilesFrameOffsets[];
extern uint16 EndeLeftMap[];
extern uint32 EndeRightTiles[];
extern uint32 EndeRightTilesFrameOffsets[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

// a function which defines the frames to play
AnimationFunctionROMSpec EndeDefaultAnimation =
{
	// number of frames of this animation function
	50,

	// frames to play in animation
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3,
		4, 5, 6, 7,
		4, 5, 6, 7,
		4, 5, 6, 7,
		4, 5, 6, 7,
		4, 5, 6, 7,
		4, 5, 6, 7,
		3, 2, 1, 0,
		0, 0, 0, 0, 0, 0, 0,
	},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&AnimatedImage_playCreditsText,

	// function's name
	"Default",
};

// an animation spec
AnimationDescriptionROMSpec EndeAnimation =
{
	// animation functions
	{
		(AnimationFunction*)&EndeDefaultAnimation,
		NULL,
	}
};


/* LEFT */

CharSetROMSpec EndeLCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	133,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	EndeLeftTiles,

	// pointer to the frames offsets
	EndeLeftTilesFrameOffsets,
};

TextureROMSpec EndeLTexture =
{
	// charset spec
	(CharSetSpec*)&EndeLCharset,

	// bgmap spec
	EndeLeftMap,

	// cols (max 64)
	19,

	// rows (max 64)
	7,

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

BgmapSpriteROMSpec EndeLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&EndeLTexture,

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

CharSetROMSpec EndeRCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	133,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	EndeRightTiles,

	// pointer to the frames offsets
	EndeRightTilesFrameOffsets,
};

TextureROMSpec EndeRTexture =
{
	// charset spec
	(CharSetSpec*)&EndeRCharset,

	// bgmap spec
	EndeLeftMap,

	// cols (max 64)
	19,

	// rows (max 64)
	7,

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

BgmapSpriteROMSpec EndeRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&EndeRTexture,

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

BgmapSpriteROMSpec* const EndeSprites[] =
{
	&EndeLSprite,
	&EndeRSprite,
	NULL
};

AnimatedImageROMSpec EndeEntity =
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
			(SpriteSpec**)EndeSprites,

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
		(AnimationDescription*)&EndeAnimation,

		// initial animation
		"Default",
	}
};
