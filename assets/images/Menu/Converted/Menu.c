//---------------------------------------------------------------------------------------------------------
//
//  Menu
//  • 104×32 pixels
//  • 38 tiles, reduced by non-unique and flipped tiles, not compressed
//  • 13×4 map, not compressed
//  • 1 animation frames, 13×4, spritesheet
//  Size: 612 + 104 = 716
//
//---------------------------------------------------------------------------------------------------------

const uint32 MenuTiles[153] __attribute__((aligned(4))) =
{
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x14050555,0x14051405,0x00050555,
    0x00050005,0x00050005,0x55050005,0x55054005,0x55054145,0x00000000,0x14500000,0x14511451,
    0x05011541,0x01405540,0x55400140,0x01400140,0x01400140,0x14500051,0x14500050,0x14501450,
    0x14501450,0x40004000,0x41554000,0x45054505,0x41554505,0x00010001,0x40550001,0x51415141,
    0x40555141,0x00000000,0x50150000,0x14501450,0x50151450,0x05000500,0x45050500,0x15145514,
    0x45055514,0x00000000,0x00010000,0x00000000,0x00010000,0x00000000,0x00000000,0x01500150,
    0x05140514,0x00000000,0x00000000,0x40000000,0x41550000,0x00500140,0x00000000,0x00010000,
    0x55510000,0x00000000,0x00000000,0x00000000,0x55010000,0x00000000,0x00000000,0x50000000,
    0x51500050,0x00050005,0x00000000,0x00000000,0x05500000,0x00000000,0x00000000,0x00000000,
    0x01550000,0x00000000,0x00000000,0x00505540,0x00500050,0x00000000,0x00000000,0x00050001,
    0x55000000,0x00000000,0x00000000,0x14501450,0x14501450,0x00000000,0x00000000,0x00000000,
    0x41540000,0x00000000,0x00000000,0x00000000,0x51450000,0x15550514,0x14051405,0x00000000,
    0x00000000,0x45054505,0x45054505,0x00000000,0x00000000,0x14511451,0x14511451,0x00000000,
    0x00000000,0x55054005,0x55054145,0x00000000,0x00000000,0x50515051,0x51415051,0x00000000,
    0x00000000,0x14141414,0x05501414,0x00000000,0x00000000,0x05050505,0x05050505,0x00000000,
    0x00000000,0x00505050,0x55400050,0x00000000,0x00000000,0x55054005,0x55014145,0x00000000,
    0x00000000,0x45554505,0x41544005,0x00000000,0x00000000,0x51415141,0x14015501,0x01400500,
    0x00000000,0x01500150,0x05140514,0x15550514,0x14051405,0x00050005,0x01550005,0x45054505,
    0x01554505,0x00000000,0x50550000,0x51415141,0x40555141,0x00000000,0x54501400,0x14501450,
    0x50551454,
};

const uint16 MenuMap[52] __attribute__((aligned(4))) =
{
    0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,
    0x0009,0x000A,0x0000,0x0000,0x0000,0x000B,0x000C,0x000D,
    0x000E,0x000F,0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,
    0x0016,0x0000,0x0017,0x0018,0x0019,0x001A,0x001B,0x001C,
    0x001D,0x001E,0x001F,0x0019,0x0020,0x0021,0x0000,0x0022,
    0x0023,0x0024,0x0025,0x0000,0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
};

const uint32 MenuTilesFrameOffsets[1] __attribute__((aligned(4))) =
{
    0x00000001,
};