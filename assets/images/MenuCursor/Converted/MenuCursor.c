//======================================================================
//
//	MenuCursor, 8x24@2, 
//	+ 3 tiles not compressed
//	+ regular map (flat), not compressed, 1x3 
//	Total size: 48 + 8 = 56
//
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const uint32 MenuCursorTiles[12] __attribute__((aligned(4)))=
{
	0x00000000,0x00540014,0x00540154,0x00000014,
	0x00000000,0x01500050,0x01500550,0x00000050,0x00000000,0x05400140,0x05401540,0x00000140,
};

const uint16 MenuCursorMap[4] __attribute__((aligned(4)))=
{
	0x0000,0x0001,0x0002,0x0000,
};
