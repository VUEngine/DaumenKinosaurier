//---------------------------------------------------------------------------------------------------------
//
//  MenuCursor
//  • 8×24 pixels
//  • 3 tiles, unreduced, not compressed
//  • 1×3 map, not compressed
//  • 3 animation frames, 1×1, spritesheet
//  Size: 52 + 6 = 58
//
//---------------------------------------------------------------------------------------------------------

const uint32 MenuCursorTiles[13] __attribute__((aligned(4))) =
{
    0x00000000,0x00000000,0x00540014,0x00540154,0x00000014,0x00000000,0x01500050,0x01500550,
    0x00000050,0x00000000,0x05400140,0x05401540,0x00000140,
};

const uint16 MenuCursorMap[3] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,
};

const uint32 MenuCursorTilesFrameOffsets[3] __attribute__((aligned(4))) =
{
    0x00000001,0x00000005,0x00000009,
};
