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
#include <TitleScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern uint32 CreditsTextTiles[];
extern uint16 CreditsTextMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

// a function which defines the frames to play
AnimationFunctionROMSpec CreditsTextDefaultAnimation =
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
AnimationFunctionROMSpec CreditsTextDefaultAlternativeAnimation =
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
AnimationDescriptionROMSpec CreditsTextAnimation =
{
	// animation functions
	{
		(AnimationFunction*)&CreditsTextDefaultAnimation,
		(AnimationFunction*)&CreditsTextDefaultAlternativeAnimation,
		NULL,
	}
};

CharSetROMSpec CreditsTextCharset =
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

	// pointer to the frames offsets
	NULL,
};

TextureROMSpec CreditsTextTexture =
{
	// charset spec
	(CharSetSpec*)&CreditsTextCharset,

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

BgmapSpriteROMSpec CreditsTextSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&CreditsTextTexture,

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

BgmapSpriteROMSpec* const CreditsTextSprites[] =
{
	&CreditsTextSprite,
	NULL
};

AnimatedImageROMSpec CreditsTextEntity =
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
			(SpriteSpec**)CreditsTextSprites,

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
		(AnimationDescription*)&CreditsTextAnimation,

		// initial animation
		"Default",
	}
};

AnimatedImageROMSpec CreditsTextAlternativeEntity =
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
			(SpriteSpec**)CreditsTextSprites,

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
		(AnimationDescription*)&CreditsTextAnimation,

		// initial animation
		"DfltAlt",
	}
};
