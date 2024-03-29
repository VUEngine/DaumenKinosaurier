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

#include <Stage.h>
#include <VIPManager.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern FontROMSpec VUENGINE_FONT;

extern CharSetSpec CREDITS_L_CH;
extern CharSetSpec CREDITS_R_CH;
extern CharSetSpec GALLERY_BUTTON_PAUSE_L_CH;
extern CharSetSpec GALLERY_BUTTON_PAUSE_R_CH;
extern CharSetSpec GALLERY_BUTTON_RESUME_L_CH;
extern CharSetSpec GALLERY_BUTTON_RESUME_R_CH;
extern CharSetSpec GALLERY_BUTTON_BACK_L_CH;
extern CharSetSpec GALLERY_BUTTON_BACK_R_CH;

extern TextureSpec CREDITS_L_TX;
extern TextureSpec CREDITS_R_TX;
extern TextureSpec GALLERY_BUTTON_PAUSE_L_TX;
extern TextureSpec GALLERY_BUTTON_PAUSE_R_TX;
extern TextureSpec GALLERY_BUTTON_RESUME_L_TX;
extern TextureSpec GALLERY_BUTTON_RESUME_R_TX;
extern TextureSpec GALLERY_BUTTON_BACK_L_TX;
extern TextureSpec GALLERY_BUTTON_BACK_R_TX;

extern EntitySpec CREDITS_EN;
extern EntitySpec GALLERY_BUTTON_PAUSE_EN;
extern EntitySpec GALLERY_BUTTON_RESUME_EN;
extern EntitySpec GALLERY_BUTTON_BACK_EN;


//---------------------------------------------------------------------------------------------------------
// 												ASSETS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMSpec CREDITS_ST_CHILDREN[] =
{
	{&GALLERY_BUTTON_BACK_EN,	{354,  30, -0.001f, 0}, 0, "Back",   NULL, NULL, true},
	{&GALLERY_BUTTON_PAUSE_EN,	{350,  12, -0.001f, 0}, 0, "Pause",  NULL, NULL, true},
	{&GALLERY_BUTTON_RESUME_EN,	{346,  12, -0.001f, 0}, 0, "Resume", NULL, NULL, true},
	{&CREDITS_EN,				{192, 112,       0, 0},	0, "Image",  NULL, NULL, true},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMSpec* const CREDITS_ST_FONTS[] =
{
	//&VUENGINE_FONT,

	NULL
};

CharSetROMSpec* const CREDITS_ST_CHARSETS[] =
{
	&GALLERY_BUTTON_BACK_L_CH,
	&GALLERY_BUTTON_BACK_R_CH,
	&GALLERY_BUTTON_PAUSE_L_CH,
	&GALLERY_BUTTON_PAUSE_R_CH,
	&GALLERY_BUTTON_RESUME_L_CH,
	&GALLERY_BUTTON_RESUME_R_CH,
	&CREDITS_L_CH,
	&CREDITS_R_CH,

	NULL
};

TextureSpec* const CREDITS_ST_TEXTURES[] =
{
	&GALLERY_BUTTON_BACK_L_TX,
	&GALLERY_BUTTON_BACK_R_TX,
	&GALLERY_BUTTON_PAUSE_L_TX,
	&GALLERY_BUTTON_PAUSE_R_TX,
	&GALLERY_BUTTON_RESUME_L_TX,
	&GALLERY_BUTTON_RESUME_R_TX,
	&CREDITS_L_TX,
	&CREDITS_R_TX,
	NULL
};


//---------------------------------------------------------------------------------------------------------
// 											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMSpec CREDITS_ST =
{
	// allocator
	__TYPE(Stage),

	// Timer config
	{
		__TIMER_100US,
		10,
		kMS
	},

	// Sound config
	{
		__DEFAULT_PCM_HZ,
		false
	},

	// level
	{
        // size
        {
            // x
            __SCREEN_WIDTH,
            // y
            __SCREEN_HEIGHT,
            // z
            __SCREEN_DEPTH,
        },

		// camera's initial position inside the game world
        {
            // x
            0,
            // y
            0,
            // z
            0,
			// p
			0,
        },

		// camera's frustum
		{
			// x0
			0,
			// y0
			0,
			// z0
			-10,
			// x1
			__SCREEN_WIDTH,
			// y1
			__SCREEN_HEIGHT,
			// z1
			__SCREEN_WIDTH * 5,
		},
	},

    // streaming
	{
		// load padding
		40,
		// unload padding
		16,
		// streaming amplitude
		24,
		// particle removal delay cycles
		0,
		// deferred
		false,
	},

	// rendering
	{
		// number of cycles the texture writing is idle
		0,

		// maximum number of texture's rows to write each time the texture writing is active
		64,

		// maximum number of rows to compute on each call to the affine functions
		16,

        // colors config
        {
            // background color
            __COLOR_BLACK,

            // brightness
            // these values times the repeat values specified in the column table (max. 16) make the final
            // brightness values on the respective regions of the screen. maximum brightness is 128.
            {
                // dark red
                __BRIGHTNESS_DARK_RED,
                // medium red
                __BRIGHTNESS_MEDIUM_RED,
                // bright red
                __BRIGHTNESS_BRIGHT_RED,
            },

            // brightness repeat
            (BrightnessRepeatSpec*)NULL,
        },

        // palettes' config
        {
        	{
        	    // bgmap palette 0
                __BGMAP_PALETTE_0,
        	    // bgmap palette 1
                __BGMAP_PALETTE_1,
                // bgmap palette 2
                __BGMAP_PALETTE_2,
        	    // bgmap palette 3
                __BGMAP_PALETTE_3,
        	},
        	{
        	    // object palette 0
                __OBJECT_PALETTE_0,
        	    // object palette 1
                __OBJECT_PALETTE_1,
        	    // object palette 2
                __OBJECT_PALETTE_2,
        	    // object palette 3
                __OBJECT_PALETTE_3,
        	},
        },

        // bgmap segments configuration
        // number of segments reserved for the param table
        0,

    	// obj segments sizes (must total 1024)
        {
            // __spt0
        	__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
            // __spt1
        	__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
            // __spt2
        	__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
            // __spt3
        	__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
        },

        // obj segments z coordinates
        // Note that each SPT's z coordinate much be larger than or equal to the previous one's,
        // since the VIP renders OBJ Worlds in reverse order (__SPT3 to __SPT0)
        {
			// __spt0
			0,
			// __spt1
			0,
			// __spt2
			0,
			// __spt3
			0,
        },

        // optical configuration values
        {
			// maximum view distance's power
			__MAXIMUM_X_VIEW_DISTANCE,
			__MAXIMUM_Y_VIEW_DISTANCE,
			// distance of the eyes to the screen
			__DISTANCE_EYE_SCREEN,
			// distance from left to right eye (depth sensation)
			__BASE_FACTOR,
			// horizontal view point center
			__HORIZONTAL_VIEW_POINT_CENTER,
			// vertical view point center
			__VERTICAL_VIEW_POINT_CENTER,
			// scaling factor
			__SCALING_MODIFIER_FACTOR,
        },
	},

    // physics
    {
        // gravity
        {
    	    0,
    	    0,
    	    0,
        },

        // friction
        0,
    },

    // assets
    {
        // fonts to preload
        (FontSpec**)CREDITS_ST_FONTS,

		// char sets to preload
		(CharSetSpec**)CREDITS_ST_CHARSETS,

		// textures to preload
		(TextureSpec**)CREDITS_ST_TEXTURES,

        // background sounds
		(Sound**)NULL,
    },

    // entities
    {
        // ui
        {
        	NULL,
            NULL,
        },

        // children
        CREDITS_ST_CHILDREN,
    },

	// post processing effects
	NULL,
};
