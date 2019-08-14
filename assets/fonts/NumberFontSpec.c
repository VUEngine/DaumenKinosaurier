///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                              THIS FILE WAS AUTO-GENERATED - DO NOT EDIT                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Printing.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE NumberFontTiles[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

CharSetROMSpec NUMBER_FONT_CH =
{
	// number of chars
	12,

	// allocation type
	__NOT_ANIMATED,

	// char spec
	NumberFontTiles,
};

FontROMSpec NUMBER_FONT =
{
	// font charset spec pointer
	(CharSetSpec*)&NUMBER_FONT_CH,

	// character number at which the font starts, allows you to skip the control characters for example
	46,

	// number of characters in this font
	12,

	// size of a single character (in chars) ({width, height})
	{1, 1},

	// font's name
	"Number",
};