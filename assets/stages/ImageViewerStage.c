/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev<jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
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

extern FontROMDef NUMBER_FONT;

extern CharSetDefinition VOLCANO_L_CH;
extern CharSetDefinition VOLCANO_R_CH;
extern CharSetDefinition GALLERY_TITLE_L_CH;
extern CharSetDefinition GALLERY_TITLE_R_CH;
extern CharSetDefinition GALLERY_BUTTON_PAUSE_L_CH;
extern CharSetDefinition GALLERY_BUTTON_PAUSE_R_CH;
extern CharSetDefinition GALLERY_BUTTON_RESUME_L_CH;
extern CharSetDefinition GALLERY_BUTTON_RESUME_R_CH;
extern CharSetDefinition GALLERY_BUTTON_BACK_L_CH;
extern CharSetDefinition GALLERY_BUTTON_BACK_R_CH;
extern CharSetDefinition GALLERY_BUTTON_FRAMES_L_CH;
extern CharSetDefinition GALLERY_BUTTON_FRAMES_R_CH;

extern TextureDefinition VOLCANO_L_TX;
extern TextureDefinition VOLCANO_R_TX;
extern TextureDefinition GALLERY_TITLE_L_TX;
extern TextureDefinition GALLERY_TITLE_R_TX;
extern TextureDefinition GALLERY_BUTTON_PAUSE_L_TX;
extern TextureDefinition GALLERY_BUTTON_PAUSE_R_TX;
extern TextureDefinition GALLERY_BUTTON_RESUME_L_TX;
extern TextureDefinition GALLERY_BUTTON_RESUME_R_TX;
extern TextureDefinition GALLERY_BUTTON_BACK_L_TX;
extern TextureDefinition GALLERY_BUTTON_BACK_R_TX;
extern TextureDefinition GALLERY_BUTTON_FRAMES_L_TX;
extern TextureDefinition GALLERY_BUTTON_FRAMES_R_TX;

extern EntityDefinition VOLCANO_AG;
extern EntityDefinition GALLERY_TITLE_AG;
extern EntityDefinition GALLERY_BUTTON_PAUSE_AG;
extern EntityDefinition GALLERY_BUTTON_RESUME_AG;
extern EntityDefinition GALLERY_BUTTON_BACK_AG;
extern EntityDefinition GALLERY_BUTTON_FRAMES_AG;


//---------------------------------------------------------------------------------------------------------
// 												ASSETS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMDef IMAGE_VIEWER_ST_CHILDREN[] =
{
	{&GALLERY_BUTTON_FRAMES_AG,	{__F_TO_FIX19_13(346),  __F_TO_FIX19_13(48), 	__F_TO_FIX19_13(-0.001f)},  0, "Frames", NULL, NULL, true},
	{&GALLERY_BUTTON_BACK_AG,	{__F_TO_FIX19_13(354),  __F_TO_FIX19_13(30), 	__F_TO_FIX19_13(-0.001f)},  0, "Back", NULL, NULL, true},
	{&GALLERY_BUTTON_PAUSE_AG,	{__F_TO_FIX19_13(350),  __F_TO_FIX19_13(12), 	__F_TO_FIX19_13(-0.001f)},  0, "Pause", NULL, NULL, true},
	{&GALLERY_BUTTON_RESUME_AG,	{__F_TO_FIX19_13(346),  __F_TO_FIX19_13(12), 	__F_TO_FIX19_13(-0.001f)},  0, "Resume", NULL, NULL, true},
	{&GALLERY_TITLE_AG,			{__F_TO_FIX19_13(48),  	__F_TO_FIX19_13(24), 	__F_TO_FIX19_13(-0.001f)},  0, "Title", NULL, NULL, true},
	{&VOLCANO_AG,				{__F_TO_FIX19_13(192),  __F_TO_FIX19_13(112), 	__F_TO_FIX19_13(0)},    	0, "Image", NULL, NULL, true},

	{NULL,{0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMDef* const IMAGE_VIEWER_ST_FONTS[] =
{
	&NUMBER_FONT,

	NULL
};

CharSetROMDef* const IMAGE_VIEWER_ST_CHARSETS[] =
{
	&GALLERY_BUTTON_FRAMES_L_CH,
	&GALLERY_BUTTON_FRAMES_R_CH,
	&GALLERY_BUTTON_BACK_L_CH,
	&GALLERY_BUTTON_BACK_R_CH,
	&GALLERY_BUTTON_PAUSE_L_CH,
	&GALLERY_BUTTON_PAUSE_R_CH,
	&GALLERY_BUTTON_RESUME_L_CH,
	&GALLERY_BUTTON_RESUME_R_CH,
	&GALLERY_TITLE_L_CH,
	&GALLERY_TITLE_R_CH,
	&VOLCANO_L_CH,
	&VOLCANO_R_CH,

	NULL
};

TextureDefinition* const IMAGE_VIEWER_ST_TEXTURES[] =
{
	&GALLERY_BUTTON_FRAMES_L_TX,
	&GALLERY_BUTTON_FRAMES_R_TX,
	&GALLERY_BUTTON_BACK_L_TX,
	&GALLERY_BUTTON_BACK_R_TX,
	&GALLERY_BUTTON_PAUSE_L_TX,
	&GALLERY_BUTTON_PAUSE_R_TX,
	&GALLERY_BUTTON_RESUME_L_TX,
	&GALLERY_BUTTON_RESUME_R_TX,
	&GALLERY_TITLE_L_TX,
	&GALLERY_TITLE_R_TX,
	&VOLCANO_L_TX,
	&VOLCANO_R_TX,
	NULL
};


//---------------------------------------------------------------------------------------------------------
// 											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMDef IMAGE_VIEWER_ST =
{
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

		// screen's initial position inside the game world
        {
            // x
            __I_TO_FIX19_13(0),
            // y
            __I_TO_FIX19_13(0),
            // z
            __I_TO_FIX19_13(0)
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
            (BrightnessRepeatDefinition*)NULL,
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
        	__I_TO_FIX19_13(0),
    		__I_TO_FIX19_13(0),
    		__I_TO_FIX19_13(0),
    		__I_TO_FIX19_13(0)
        },

        // optical configuration values
        {
    		// maximum view distance's power into the horizon
    		__MAXIMUM_VIEW_DISTANCE_POWER,
    		// distance of the eyes to the screen
        	__I_TO_FIX19_13(__DISTANCE_EYE_SCREEN),
    		// distance from left to right eye (depth sensation)
    		__I_TO_FIX19_13(__BASE_FACTOR),
    		// horizontal view point center
    		__I_TO_FIX19_13(__HORIZONTAL_VIEW_POINT_CENTER),
    		// vertical view point center
    		__I_TO_FIX19_13(__VERTICAL_VIEW_POINT_CENTER),
        },
	},

    // physics
    {
        // gravity
        {
    	    __I_TO_FIX19_13(0),
    	    __I_TO_FIX19_13(0),
    	    __I_TO_FIX19_13(0)
        },

        // friction
        __F_TO_FIX19_13(0),
    },

    // assets
    {
        // fonts to preload
        (FontDefinition**)IMAGE_VIEWER_ST_FONTS,

		// char sets to preload
		(CharSetDefinition**)IMAGE_VIEWER_ST_CHARSETS,

		// textures to preload
		(TextureDefinition**)IMAGE_VIEWER_ST_TEXTURES,

        // background music
        NULL,
    },

    // entities
    {
        // ui
        {
        	NULL,
            NULL,
        },

        // children
        IMAGE_VIEWER_ST_CHILDREN,
    },

	// post processing effects
	NULL,
};
