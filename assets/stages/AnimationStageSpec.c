/**
 * DaumenKinosaurier – VUEdition
 *
 * © Marten Reiß and Christian Radke
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
// 												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Stage.h>
#include <VIPManager.h>


//---------------------------------------------------------------------------------------------------------
// 												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern FontROMSpec VUEngineFont;

extern CharSetSpec RexLCharset;
extern CharSetSpec RexRCharset;
extern CharSetSpec GalleryButtonResumeLCharset;
extern CharSetSpec GalleryButtonResumeRCharset;
extern CharSetSpec GalleryButtonBackLCharset;
extern CharSetSpec GalleryButtonBackRCharset;

extern TextureSpec RexLTexture;
extern TextureSpec RexRTexture;
extern TextureSpec GalleryButtonResumeLTexture;
extern TextureSpec GalleryButtonResumeRTexture;
extern TextureSpec GalleryButtonBackLTexture;
extern TextureSpec GalleryButtonBackRTexture;

extern EntitySpec RexEntity;
extern EntitySpec GalleryButtonResumeEntity;
extern EntitySpec GalleryButtonBackEntity;


//---------------------------------------------------------------------------------------------------------
// 												ASSETS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMSpec AnimationStChildren[] =
{
	{&GalleryButtonBackEntity,	{354,  30, 	-1, 0}, 0, "Back",   NULL, NULL, true},
	{&GalleryButtonResumeEntity,	{346,  12, 	-1, 0}, 0, "Resume", NULL, NULL, true},
	{&RexEntity,					{192, 112,   0, 0}, 0, "Image",  NULL, NULL, true},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMSpec* const AnimationStFonts[] =
{
	//&VUEngineFont,

	NULL
};

CharSetROMSpec* const AnimationStCharsets[] =
{
	&GalleryButtonBackLCharset,
	&GalleryButtonBackRCharset,
	&GalleryButtonResumeLCharset,
	&GalleryButtonResumeRCharset,

	NULL
};

TextureSpec* const AnimationStTextures[] =
{
	&GalleryButtonBackLTexture,
	&GalleryButtonBackRTexture,
	&GalleryButtonResumeLTexture,
	&GalleryButtonResumeRTexture,

	NULL
};


//---------------------------------------------------------------------------------------------------------
// 											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMSpec AnimationSt =
{
	// allocator
	__TYPE(Stage),

	// Timer config
	{
		__TIMER_100US,
		20,
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
        	0,
            // __spt1
        	0,
            // __spt2
        	0,
            // __spt3
        	0,
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
        (FontSpec**)AnimationStFonts,

		// char sets to preload
		(CharSetSpec**)AnimationStCharsets,

		// textures to preload
		(TextureSpec**)AnimationStTextures,

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
        (PositionedEntity*)AnimationStChildren,
    },

	// post processing effects
	NULL,
};
