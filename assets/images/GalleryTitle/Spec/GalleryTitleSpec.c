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
#include <AnimatedEntity.h>
#include <BgmapAnimatedSprite.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern uint32 GalleryTitleLeftTiles[];
extern uint32 GalleryTitleLeftTilesFrameOffsets[];
extern uint16 GalleryTitleLeftMap[];
extern uint32 GalleryTitleRightTiles[];
extern uint32 GalleryTitleRightTilesFrameOffsets[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

/* Animations */

// a function which defines the frames to play
AnimationFunctionROMSpec GalleryTitleOption1Animation =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{0, 1},

	// number of cycles a frame of animation is displayed
	16,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"Volcano",
};

// a function which defines the frames to play
AnimationFunctionROMSpec GalleryTitleOption2Animation =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{2, 3},

	// number of cycles a frame of animation is displayed
	16,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"Roooar",
};

// a function which defines the frames to play
AnimationFunctionROMSpec GalleryTitleOption3Animation =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{4, 5},

	// number of cycles a frame of animation is displayed
	16,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"WalkCycle",
};

// a function which defines the frames to play
AnimationFunctionROMSpec GalleryTitleOption4Animation =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{6, 7},

	// number of cycles a frame of animation is displayed
	16,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"Banana",
};

// a function which defines the frames to play
AnimationFunctionROMSpec GalleryTitleOption5Animation =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{8, 9},

	// number of cycles a frame of animation is displayed
	16,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"Extinction",
};

// an animation spec
AnimationDescriptionROMSpec GalleryTitleAnimation =
{
	// animation functions
	{
		(AnimationFunction*)&GalleryTitleOption1Animation,
		(AnimationFunction*)&GalleryTitleOption2Animation,
		(AnimationFunction*)&GalleryTitleOption3Animation,
		(AnimationFunction*)&GalleryTitleOption4Animation,
		(AnimationFunction*)&GalleryTitleOption5Animation,

		NULL,
	}
};

/* Left */

CharSetROMSpec GalleryTitleLCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	40,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	GalleryTitleLeftTiles,

	// pointer to the frames offsets
	GalleryTitleLeftTilesFrameOffsets,
};

TextureROMSpec GalleryTitleLTexture =
{
	// charset spec
	(CharSetSpec*)&GalleryTitleLCharset,

	// bgmap spec
	GalleryTitleLeftMap,

	// cols (max 64)
	10,

	// rows (max 64)
	4,

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

BgmapSpriteROMSpec GalleryTitleLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&GalleryTitleLTexture,

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

CharSetROMSpec GalleryTitleRCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	40,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	GalleryTitleRightTiles,

	// pointer to the frames offsets
	GalleryTitleRightTilesFrameOffsets,
};

TextureROMSpec GalleryTitleRTexture =
{
	// charset spec
	(CharSetSpec*)&GalleryTitleRCharset,

	// bgmap spec
	GalleryTitleLeftMap,

	// cols (max 64)
	10,

	// rows (max 64)
	4,

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

BgmapSpriteROMSpec GalleryTitleRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&GalleryTitleRTexture,

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

BgmapSpriteROMSpec* const GalleryTitleSprites[] =
{
	&GalleryTitleLSprite,
	&GalleryTitleRSprite,
	NULL
};

AnimatedEntityROMSpec GalleryTitleEntity =
{
	{
		// class allocator
		__TYPE(AnimatedEntity),

		// children
		NULL,

		// behaviors
		NULL,

		// extra
		NULL,

		// sprites
		(SpriteSpec**)GalleryTitleSprites,

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
	(AnimationDescription*)&GalleryTitleAnimation,

	// initial animation
	"Volcano",
};
