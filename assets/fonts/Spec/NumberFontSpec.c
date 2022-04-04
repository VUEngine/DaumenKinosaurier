///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              THIS FILE WAS AUTO-GENERATED - DO NOT EDIT                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------
//                                                INCLUDES                                                 
//---------------------------------------------------------------------------------------------------------

#include <Printing.h>


//---------------------------------------------------------------------------------------------------------
//                                              DECLARATIONS                                               
//---------------------------------------------------------------------------------------------------------

extern uint32 NumberFontTiles[];


//---------------------------------------------------------------------------------------------------------
//                                               DEFINITIONS                                               
//---------------------------------------------------------------------------------------------------------

CharSetROMSpec NumberFontCharSet =
{
	// number of chars
	12,

	// allocation type
	__NOT_ANIMATED,

	// char spec
	NumberFontTiles,

	// pointer to the frames offsets
	NULL,
};

FontROMSpec NumberFont =
{
	// font charset spec pointer
	(CharSetSpec*)&NumberFontCharSet,

	// character number at which the font starts, allows you to skip the control characters for example
	46,

	// number of characters in this font
	12,

	// number of characters per line in charset
	1,

	// size of a single character (in chars) ({width, height})
	{1, 1},

	// font's name
	"Number",
};