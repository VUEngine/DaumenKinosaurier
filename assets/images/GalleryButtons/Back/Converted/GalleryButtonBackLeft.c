//---------------------------------------------------------------------------------------------------------
//
//  GalleryButtonBackLeft
//  • 48×32 pixels
//  • 24 tiles, unreduced, not compressed
//  • 6×4 map, not compressed
//  • 2 animation frames, 6×2, spritesheet
//  Size: 388 + 48 = 436
//
//---------------------------------------------------------------------------------------------------------

const uint32 GalleryButtonBackLeftTiles[97] __attribute__((aligned(4))) =
{
    0x00000000,0x00000000,0xC0000000,0x7000F000,0x70007000,0x00000000,0x3FFF0000,0xD7FDF555,
    0xD70DD70D,0x00000000,0x00000000,0x7000F000,0x75F5FFFF,0x00000000,0x00000000,0x03FD000F,
    0x0D5D0F5F,0x9C000000,0xAA70A9C0,0x6A9C6A70,0x6A9C6A9C,0x00AA0000,0x2AAA0AAA,0xAA592A95,
    0xAA95AA59,0x70007000,0xF0007000,0x0000C000,0x00000000,0xD57DD7FD,0xF555D5FD,0xFF00D7FF,
    0x00000000,0x75F575F5,0x75577575,0x0000FFFF,0x00000000,0x035D0F5D,0x0D7D0F5D,0x00000FFF,
    0x00000000,0x6A9C6A9C,0xAA7C6A5C,0x95F0A970,0xFC005FC0,0xAA59AA59,0xEAAAEA95,0x3DAA3AAA,
    0x00FF0FD5,0x00000000,0xC0000000,0x7000F000,0x70007000,0x00000000,0x3FFF0000,0xD7FDF555,
    0xD70DD70D,0x00000000,0x00000000,0x7000F000,0x75F5FFFF,0x00000000,0x00000000,0x03FD000F,
    0x0D5D0F5F,0xFC000000,0xAAF0AFC0,0x5ABCAAB0,0x5AAC5AAC,0x00FF0000,0x3EAA0FEA,0xFAA53AAA,
    0xEA96EA96,0x70007000,0xF0007000,0x0000C000,0x00000000,0xD57DD7FD,0xF555D5FD,0xFF00D7FF,
    0x00000000,0x75F575F5,0x75577575,0x0000FFFF,0x00000000,0x035D0F5D,0x0D7D0F5D,0x00000FFF,
    0x00000000,0x5AAC5AAC,0x5ABC5AAC,0xAAF0AAB0,0xFC00AFC0,0xEA96EAA5,0xFAA5EA96,0x3EAA3AAA,
    0x00FF0FEA,
};

const uint16 GalleryButtonBackLeftMap[24] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
};

const uint32 GalleryButtonBackLeftTilesFrameOffsets[2] __attribute__((aligned(4))) =
{
    0x00000001,0x00000031,
};