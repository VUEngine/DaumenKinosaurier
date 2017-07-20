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
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <libgccvb.h>
#include <AnimatedInGameEntity.h>
#include <BgmapAnimatedSprite.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE GalleryTitleLeftTiles[];
extern BYTE GalleryTitleLeftMap[];
extern BYTE GalleryTitleRightTiles[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

/* Animations */

// a function which defines the frames to play
AnimationFunctionROMDef GALLERY_TITLE_OPTION_1_ANIM =
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
AnimationFunctionROMDef GALLERY_TITLE_OPTION_2_ANIM =
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
AnimationFunctionROMDef GALLERY_TITLE_OPTION_3_ANIM =
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
AnimationFunctionROMDef GALLERY_TITLE_OPTION_4_ANIM =
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
AnimationFunctionROMDef GALLERY_TITLE_OPTION_5_ANIM =
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

// an animation definition
AnimationDescriptionROMDef GALLERY_TITLE_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&GALLERY_TITLE_OPTION_1_ANIM,
		(AnimationFunction*)&GALLERY_TITLE_OPTION_2_ANIM,
		(AnimationFunction*)&GALLERY_TITLE_OPTION_3_ANIM,
		(AnimationFunction*)&GALLERY_TITLE_OPTION_4_ANIM,
		(AnimationFunction*)&GALLERY_TITLE_OPTION_5_ANIM,

		NULL,
	}
};

/* Left */

CharSetROMDef GALLERY_TITLE_L_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	40,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char definition
	GalleryTitleLeftTiles,
};

TextureROMDef GALLERY_TITLE_L_TX =
{
	// charset definition
	(CharSetDefinition*)&GALLERY_TITLE_L_CH,

	// bgmap definition
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
};

BgmapSpriteROMDef GALLERY_TITLE_L_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture definition
		(TextureDefinition*)&GALLERY_TITLE_L_TX,

		// transparent
		false,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the definition (BgmapSprite or BgmapSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_LON,
};

/* Right */

CharSetROMDef GALLERY_TITLE_R_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	40,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char definition
	GalleryTitleRightTiles,
};

TextureROMDef GALLERY_TITLE_R_TX =
{
	// charset definition
	(CharSetDefinition*)&GALLERY_TITLE_R_CH,

	// bgmap definition
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
};

BgmapSpriteROMDef GALLERY_TITLE_R_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture definition
		(TextureDefinition*)&GALLERY_TITLE_R_TX,

		// transparent
		false,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the definition (BgmapSprite or BgmapSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_RON,
};

/* Entity */

BgmapSpriteROMDef* const GALLERY_TITLE_SPRITES[] =
{
	&GALLERY_TITLE_L_SPRITE,
	&GALLERY_TITLE_R_SPRITE,
	NULL
};

AnimatedInGameEntityROMDef GALLERY_TITLE_AG =
{
	{
		{
			__TYPE(AnimatedInGameEntity),
			(SpriteROMDef**)GALLERY_TITLE_SPRITES,
		},

		// collision detection gap (up, down, left, right)
		{0, 0, 0, 0},

		// in game type
		0,

		// width
		// if 0, width and height will be inferred from the texture's size
		0,

		// height
		// if 0, width and height will be inferred from the texture's size
		0,

		// depth
		4
	},

	// pointer to the animation definition for the item
	(AnimationDescription*)&GALLERY_TITLE_ANIM,

	// initial animation
	"Volcano",
};
