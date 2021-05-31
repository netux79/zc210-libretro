//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  defdata.h
//
//  Default item, weapon, and enemy data for Zelda Classic.
//
//  This data was originally hard coded in the item class,
//  enemy classes, etc. I extracted it to make generic
//  classes, thus allowing custom items and enemies.
//
//--------------------------------------------------------

#ifndef _DEFDATA_H_
#define _DEFDATA_H_

const guydata default_guys[eMAXGUYS] =
{
   // flags        tile  hp                    family       cset  anim   frate   dp                  wdp                 weapon     rate hrate  step  homing  grumble  item_set       misc1  misc2  bosspal
   { 0x00000001,    0,   0 * DAMAGE_MULTIPLIER,  0,            0,  0,         0,   0,                  0,                  0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,   84,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,   85,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,   86,   1 * DAMAGE_MULTIPLIER,  eeGUY,        6,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,  116,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000008,   65,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aFLIP,    12,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             1,     0,     0},
   { 0x00000008,   63,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  a2FRM,     8,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,  130,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,   35,   1 * DAMAGE_MULTIPLIER,  eeGUY,        6,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,   87,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aNONE,     0,   1,                  1,                  wNone,      0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eROCTO1
   { 0x00000000,   92,   1 * DAMAGE_MULTIPLIER,  eeSHOOT,      8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   ewRock,     4,    3,    50,   64,      1,       1,             0,     0,     0},
   { 0x00000000,   92,   2 * DAMAGE_MULTIPLIER,  eeSHOOT,      7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   ewRock,     4,    5,    50,   64,      2,       isBOMBS,       0,     0,     0},
   { 0x00000000,   92,   1 * DAMAGE_MULTIPLIER,  eeSHOOT,      8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   ewRock,     6,    3,   100,  128,      1,       1,             0,     0,     0},
   { 0x00000000,   92,   2 * DAMAGE_MULTIPLIER,  eeSHOOT,      7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   ewRock,     6,    5,   100,  128,      2,       isBOMBS,       0,     0,     0},
   // eRTEK
   { 0x00000000,  103,   1 * DAMAGE_MULTIPLIER,  eeTEK,        8,  aTEK,     32,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   wNone,      0,    0,     0,    0,      0,       1,            24,     3,     0},
   { 0x00000000,  103,   1 * DAMAGE_MULTIPLIER,  eeTEK,        7,  aTEK,     32,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   wNone,      0,    0,     0,    0,      0,       isMAGICMONEY,      80,    16,     0},
   // eRLEV
   { 0x00000000,   96,   2 * DAMAGE_MULTIPLIER,  eeLEV,        8,  aLEV,     40,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   wNone,      0,    0,    50,    0,      0,       isMONEY,       0,     8,     0},
   { 0x00000000,   96,   4 * DAMAGE_MULTIPLIER,  eeLEV,        7,  aLEV,     40,   2 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   wNone,      8,    0,    50,    0,      0,       isMAGICMONEY,       1,     8,     0},
   // eRMOBLIN
   { 0x00000000,  114,   2 * DAMAGE_MULTIPLIER,  eeSHOOT,      8,  aWALK,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   ewArrow,    5,    4,    50,  128,      3,       1,             0,     0,     0},
   { 0x00000000,  114,   3 * DAMAGE_MULTIPLIER,  eeSHOOT,      9,  aWALK,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   ewArrow,    8,    6,    50,  128,      4,       isBOMBS,       1,     0,     0},
   // eRLYNEL
   { 0x00000000,  105,   4 * DAMAGE_MULTIPLIER,  eeSHOOT,      8,  aWALK,    12,   2 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   ewSword,    6,    4,    50,  128,      3,       isMAGICMONEY,       1,     0,     0},
   { 0x00000000,  105,   6 * DAMAGE_MULTIPLIER,  eeSHOOT,      7,  aWALK,    12,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   ewSword,    6,    4,    50,  128,      3,       isMAGICBOMBS,       1,     0,     0},
   // ePEAHAT
   { 0x00000018,  101,   2 * DAMAGE_MULTIPLIER,  eePEAHAT,     8,  aPHAT,     0,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  112,   2 * DAMAGE_MULTIPLIER,  eeZORA,       9,  aZORA,   256,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000008,  118,   1 * DAMAGE_MULTIPLIER,  eeROCK,       8,  a2FRM,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0},
   // eGHINI1
   { 0x00000000,  109,   9 * DAMAGE_MULTIPLIER,  eeGHINI,      7,  aGHINI,    0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,    50,    0,      0,       isMONEY,       0,     0,     0},
   { 0x00000018,  109,   9 * DAMAGE_MULTIPLIER,  eeGHINI,      7,  aGHINI,    0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,     0,    0,      0,       isMONEY,       1,     0,     0},
   // eARMOS
   { 0x00000000,   88,   3 * DAMAGE_MULTIPLIER,  eeARMOS,      8,  aARMOS,   16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          6,    0,    50,  128,      0,       0,             0,     0,     0},
   // eKEESE1
   { 0x00000001,  123,   1,                    eeKEESE,      7,  aKEESE,    0,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,  123,   1,                    eeKEESE,      8,  aKEESE,    0,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   { 0x00000001,  123,   1,                    eeKEESE,      9,  aKEESE,    0,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eSTALFOS
   { 0x00000000,  125,   2 * DAMAGE_MULTIPLIER,  eeWALK,       8,  aFLIP,    16,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    0,    50,  128,      0,       1,             0,     0,     0},
   // eGEL
   { 0x00000001,  120,   1,                    eeGEL,        9,  a2FRM,     4,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,   16,   100,   64,      0,       0,             0,     0,     0},
   { 0x00000000,  137,   2 * DAMAGE_MULTIPLIER,  eeZOL,        9,  a2FRM,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,   16,    40,  144,      0,       isMONEY,       0,     0,     0},
   // eROPE
   { 0x00000000,  126,   1 * DAMAGE_MULTIPLIER,  eeROPE,       8,  aROPE,    16,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    0,    50,    0,      0,       isMONEY,       0,     0,     0},
   // eRGORIYA
   { 0x00000000,  130,   3 * DAMAGE_MULTIPLIER,  eeGORIYA,     8,  aWALK,    12,   1 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          6,    5,    50,  128,      4,       isBOMBS,       0,     0,     0},
   { 0x00000000,  130,   5 * DAMAGE_MULTIPLIER,  eeGORIYA,     7,  aWALK,    12,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          6,    6,    50,  128,      4,       isLIFE,        0,     0,     0},
   // eTRAP
   { 0x00000008,  122,   1 * DAMAGE_MULTIPLIER,  eeTRAP,       7,  aNONE,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eWALLM
   { 0x00000000,  128,   2 * DAMAGE_MULTIPLIER,  eeWALLM,      7,  aWALLM,   16,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,    40,    0,      0,       isMONEY,       0,     0,     0},
   // eRDKNUT
   { 0x0100F720,  140,   4 * DAMAGE_MULTIPLIER,  eeWALK,       8,  aDWALK,   16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          5,    0,    50,  144,      0,       isMAGICBOMBS,       0,     0,     0},
   { 0x0100F720,  140,   8 * DAMAGE_MULTIPLIER,  eeWALK,       7,  aDWALK,   16,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,    67,  160,      0,       isMAGICLIFE,        0,     0,     0},
   // eBUBBLE
   { 0x00000008,  111,   1 * DAMAGE_MULTIPLIER,  eeBUBBLE,     9,  aFLIP,     8,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          4,    0,   100,    0,      0,       0,             0,     0,     0},
   // eVIRE
   { 0x00000000,  165,   4 * DAMAGE_MULTIPLIER,  eeVIRE,       7,  aVIRE,    20,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,    50,   64,      0,       isBOMBS,       0,     0,     0},
   // eLIKE
   { 0x00000000,  162,   9 * DAMAGE_MULTIPLIER,  eeLIKE,       8,  a3FRM,    32,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,    50,   64,      0,       0,             0,     0,     0},
   // eGIBDO
   { 0x00000000,  136,   7 * DAMAGE_MULTIPLIER,  eeWALK,       7,  aFLIP,    16,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          4,    0,    50,  128,      0,       isBOMBS,       0,     0,     0},
   // ePOLSV
   { 0x20F0F4E0,  134,  10 * DAMAGE_MULTIPLIER,  eePOLSV,      6,  a2FRM,    16,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          4,    0,   100,   32,      0,       isMONEY,       0,     0,     0},
   // eRWIZ
   { 0x0000F720,  169,   4 * DAMAGE_MULTIPLIER,  eeWIZZ,       8,  aWIZZ,   256,   2 * HP_PER_HEART / 8,   8 * HP_PER_HEART / 8,   ewMagic,    0,    0,     0,    0,      0,       isBOMBS,       0,     0,     0},
   { 0x0000F720,  169,  10 * DAMAGE_MULTIPLIER,  eeWIZZ,       7,  aWIZZ,    16,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   ewMagic,    4,    0,    50,    0,      0,       1,             1,     0,     0},
   // eRAQUAM
   { 0x00000004,  226,   6 * DAMAGE_MULTIPLIER,  eeAQUA,      14,  aAQUA,     0,   2 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     spAQUA },
   // eMOLDORM
   { 0x00000004,   57,   2 * DAMAGE_MULTIPLIER,  eeMOLD,       8,  aNONE,    32,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,    50,    0,      0,       0,             5,     0,     0},
   // eDODONGO
   { 0x00000000,  149,   8 * DAMAGE_MULTIPLIER,  eeDONGO,      8,  aDONGO,    0,   2 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    0,    50,   64,      0,       1,             0,     0,     0},
   // eMANHAN
   { 0x00000004,  190,   4 * DAMAGE_MULTIPLIER,  eeMANHAN,     7,  aMANHAN,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0},
   // eGLEEOK1
   { 0x0000F7E4,  220,  10 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             1,     6,     spGLEEOK },
   { 0x0000F7E4,  220,  10 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             2,     6,     spGLEEOK },
   { 0x0000F7E4,  220,  10 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             3,     6,     spGLEEOK },
   { 0x0000F7E4,  220,  10 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             4,     6,     spGLEEOK },
   // eDIG1
   { 0x00000004,  230,   6 * DAMAGE_MULTIPLIER,  eeDIG,       14,  aDIG,      6,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,    25,    0,      0,       1,             1,     0,     spDIG },
   { 0x00000004,  230,   6 * DAMAGE_MULTIPLIER,  eeDIG,       14,  aDIG,      6,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,    25,    0,      0,       1,             3,     0,     spDIG },
   // eDIGPUP1
   { 0x00000004,  160,   8 * DAMAGE_MULTIPLIER,  eeDIG,       14,  aDIG,     12,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,    25,    0,      0,       1,             1,     0,     spDIG },
   { 0x00000004,  160,   8 * DAMAGE_MULTIPLIER,  eeDIG,       14,  aDIG,     12,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,    25,    0,      0,       1,             1,     0,     spDIG },
   { 0x00000004,  160,   8 * DAMAGE_MULTIPLIER,  eeDIG,       14,  aDIG,     12,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,    25,    0,      0,       1,             1,     0,     spDIG },
   { 0x00000004,  160,   8 * DAMAGE_MULTIPLIER,  eeDIG,       14,  aDIG,     12,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,    25,    0,      0,       1,             1,     0,     spDIG },
   // eRGOHMA
   { 0x00FFFCE4,  180,   2 * DAMAGE_MULTIPLIER,  eeGHOMA,      8,  aGHOMA,    0,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0 },
   { 0x00FFFCE4,  180,   6 * DAMAGE_MULTIPLIER,  eeGHOMA,      7,  aGHOMA,    0,   4 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0 },
   // eRCENT
   { 0x00000000,  173,   2 * DAMAGE_MULTIPLIER,  eeLANM,       8,  aLANM,     8,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             5,     0,     0 },
   { 0x00000000,  173,   2 * DAMAGE_MULTIPLIER,  eeLANM,       7,  aLANM,     4,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   200,    0,      0,       1,             5,     0,     0 },
   // ePATRA1
   { 0x00000004,  191,  11 * DAMAGE_MULTIPLIER,  eePATRA,      7,  aFLIP,     0,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             8,     0,     0 },
   { 0x00000004,  191,  11 * DAMAGE_MULTIPLIER,  eePATRA,      7,  aFLIP,     0,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             8,     0,     0 },
   // eGANON
   { 0x00000004,  260,  16 * DAMAGE_MULTIPLIER,  eeGANON,     14,  aGANON,    0,   8 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             0,     0,     0 },
   // eSTALFOS2
   { 0x00000000,  125,   2 * DAMAGE_MULTIPLIER,  eeSHOOT,      8,  aFLIP,    16,   1 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   ewSword,    4,    4,    50,  128,      0,       1,             0,     0,     0 },
   // eROPE2
   { 0x00000000,  126,   4 * DAMAGE_MULTIPLIER,  eeROPE,       9,  aROPE,    16,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    0,    50,    0,      0,       isMONEY,       0,     0,     0 },
   // eRBUBBLE
   { 0x00000008,  111,   1 * DAMAGE_MULTIPLIER,  eeBUBBLE,     8,  aFLIP,     8,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          4,    0,   100,    0,      0,       0,             1,     0,     0 },
   { 0x00000008,  111,   1 * DAMAGE_MULTIPLIER,  eeBUBBLE,     7,  aFLIP,     8,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          4,    0,   100,    0,      0,       0,             2,     0,     0 },
   // eFBALL
   { 0x00000008,    0,   1 * DAMAGE_MULTIPLIER,  eeFBALL,      0,  aNONE,     0,   0 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0 },
   // eITEMFAIRY
   { 0x00000008,    0,   1 * DAMAGE_MULTIPLIER,  0,            0,  aNONE,   256,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          0,    0,    50,    0,      0,       0,             0,     0,     0 },
   // eFIRE
   { 0x00000001,   65,   1 * DAMAGE_MULTIPLIER,  0,            8,  aFLIP,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             1,     0,     0 },
   // eCOCTO
   { 0x00000000,   92,  20 * DAMAGE_MULTIPLIER,  eeSHOOT,      9,  aOCTO,     4,  10 * HP_PER_HEART / 8,  20 * HP_PER_HEART / 8,   ewMagic,   16,    1,   200,  255,      0,       isMAGIC,       0,     0,     0 },
   // eDKNIGHT
   { 0x0100F720,  140,  30 * DAMAGE_MULTIPLIER,  eeWALK,       9,  aDWALK,   16,  20 * HP_PER_HEART / 8,  40 * HP_PER_HEART / 8,   ewSword,   10,    0,   150,  255,      0,       isLIFE,        0,     0,     0 },
   // eGELTRIB
   { 0x00000001,  120,   1,                    eeGELTRIB,    9,  a2FRM,     4,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,   16,   100,   64,      0,       isMAGIC,       0,     0,     0 },
   { 0x00000000,  137,   4 * DAMAGE_MULTIPLIER,  eeZOLTRIB,    9,  a2FRM,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,   16,    40,  144,      0,       isMAGICMONEY,  0,     0,     0 },
   // eKEESETRIB
   { 0x00000001,  123,   1,                    eeKEESETRIB,  9,  aKEESE,    0,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       isMAGIC,       0,     0,     0 },
   { 0x00000000,  165,   8 * DAMAGE_MULTIPLIER,  eeVIRETRIB,   9,  aVIRE,    20,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,    50,   64,      0,       isMAGICBOMBS,  0,     0,     0 },
   // eSDKNUT
   { 0x0100F720,  140,  16 * DAMAGE_MULTIPLIER,  eeWALK,       9,  aDWALK,   16,   8 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          4,    0,   100,  200,      0,       isMAGIC,       0,     0,     0},
   // eLAQUAM
   { 0x00000004,  226,   6 * DAMAGE_MULTIPLIER,  eeAQUA,      14,  aAQUA,     0,   2 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     spAQUA },
   // eMANHAN2
   { 0x00000004,  190,   4 * DAMAGE_MULTIPLIER,  eeMANHAN,     8,  aMANHAN,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0},
   // eTRAP_H
   { 0x00000008,  122,   1 * DAMAGE_MULTIPLIER,  eeTRAP,       7,  aNONE,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eTRAP_V
   { 0x00000008,  122,   1 * DAMAGE_MULTIPLIER,  eeTRAP,       7,  aNONE,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eTRAP_LR
   { 0x00000008,  122,   1 * DAMAGE_MULTIPLIER,  eeTRAP,       7,  aNONE,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eTRAP_UD
   { 0x00000008,  122,   1 * DAMAGE_MULTIPLIER,  eeTRAP,       7,  aNONE,    16,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eFWIZ
   { 0x0000F720,  169,  12 * DAMAGE_MULTIPLIER,  eeWIZZ,       8,  aWIZZ,    16,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   ewMagic,    4,    0,    50,    0,      0,       isMAGIC,       1,     0,     0},
   // eWWIZ
   { 0x0000F720,  169,  14 * DAMAGE_MULTIPLIER,  eeWIZZ,       7,  aWIZZ,   256,   2 * HP_PER_HEART / 8,   8 * HP_PER_HEART / 8,   ewMagic,    0,    0,     0,    0,      0,       isMAGICBOMBS,  0,     0,     0},
   // eCEILINGM
   { 0x00000001,    0,   0 * DAMAGE_MULTIPLIER,  0,            0,  0,         0,   0,                  0,                  0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eFLOORM
   { 0x00000001,    0,   0 * DAMAGE_MULTIPLIER,  0,            0,  0,         0,   0,                  0,                  0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // ePATRABS
   { 0x00000004,  191,  11 * DAMAGE_MULTIPLIER,  eePATRA,      7,  aFLIP,     0,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             8,     0,     0 },
   // ePATRAL2
   { 0x00000004,  191,  11 * DAMAGE_MULTIPLIER,  eePATRA,      7,  aFLIP,     0,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             8,     0,     0 },
   // ePATRAL3
   { 0x00000004,  191,  11 * DAMAGE_MULTIPLIER,  eePATRA,      7,  aFLIP,     0,   4 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          8,    0,   100,    0,      0,       1,             8,     0,     0 },
   // eBAT
   { 0x00000001,  123,   4 * DAMAGE_MULTIPLIER,  eeKEESE,      9,  aKEESE,    0,   3 * HP_PER_HEART / 8,   3 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       isMAGIC,       0,     0,     0},
   // eBATROBE
   { 0x0000F720,  169,  14 * DAMAGE_MULTIPLIER,  eeWIZZ,       9,  aWIZZ,   256,   2 * HP_PER_HEART / 8,   8 * HP_PER_HEART / 8,   ewMagic,    0,    0,     0,    0,      0,       isMAGICBOMBS,  0,     0,     0},
   // eBATROBEKING
   { 0x00000001,    0,   0 * DAMAGE_MULTIPLIER,  0,            0,  0,         0,   0,                  0,                  0,          0,    0,     0,    0,      0,       0,             0,     0,     0},
   // eGLEEOK1F
   { 0x0000F7E4,  220,  15 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             1,     10,    spGLEEOK },
   { 0x0000F7E4,  220,  15 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             2,     10,    spGLEEOK },
   { 0x0000F7E4,  220,  15 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             3,     10,    spGLEEOK },
   { 0x0000F7E4,  220,  15 * DAMAGE_MULTIPLIER,  eeGLEEOK,    14,  aGLEEOK,   0,   2 * HP_PER_HEART / 8,   2 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             4,     10,    spGLEEOK },
   // eMWIZ
   { 0x0000F720,  169,  96 * HP_PER_HEART / 8,    eeWIZZ,       9,  aWIZZ,    16,   16 * HP_PER_HEART / 8,  16 * HP_PER_HEART / 8,  ewMagic,    4,    0,    75,    0,      0,       isMAGIC,       1,     0,     0},
   // eDODONGOBS
   { 0x00000000, 3060,   8 * DAMAGE_MULTIPLIER,  eeDONGO,     14,  aDONGO,    0,   2 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    0,    50,   64,      0,       1,             0,     0,     spDIG},
   // eDODONGOF
   { 0x00000000, 3060,   8 * DAMAGE_MULTIPLIER,  eeDONGO,     14,  aDONGO,    0,   2 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    0,    50,   64,      0,       1,             0,     0,     spDIG},
   // eTRIGGER
   { 0x00000000,    0,   1 * DAMAGE_MULTIPLIER,  0,            8,  0,         1,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       0,             0,     0,     0 },
   // eIBUBBLE
   { 0x00000008,  111,   1 * DAMAGE_MULTIPLIER,  eeBUBBLE,     9,  aFLIP,     8,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          4,    0,   100,    0,      0,       0,             0,     0,     0 },
   { 0x00000008,  111,   1 * DAMAGE_MULTIPLIER,  eeBUBBLE,     8,  aFLIP,     8,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          4,    0,   100,    0,      0,       0,             1,     0,     0 },
   { 0x00000008,  111,   1 * DAMAGE_MULTIPLIER,  eeBUBBLE,     7,  aFLIP,     8,   0 * HP_PER_HEART / 8,   0 * HP_PER_HEART / 8,   0,          4,    0,   100,    0,      0,       0,             2,     0,     0 },
   // eSTALFOS3
   { 0x00000000,  125,   6 * DAMAGE_MULTIPLIER,  eeSHOOT,      9,  aFLIP,     8,   3 * HP_PER_HEART / 8,  12 * HP_PER_HEART / 8,   ewSword,    4,    4,   100,  200,      0,       isMAGIC,             0,     0,     0 },
   // eGOHMA3
   { 0x00FFFCE4,  180,  12 * DAMAGE_MULTIPLIER,  eeGHOMA,      8,  aGHOMA,    0,   8 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0 },
   { 0x00FFFCE4,  180,  24 * DAMAGE_MULTIPLIER,  eeGHOMA,      7,  aGHOMA,    0,   8 * HP_PER_HEART / 8,   4 * HP_PER_HEART / 8,   0,          0,    0,     0,    0,      0,       1,             0,     0,     0 },
   // eNPCSTAND1
   { 0x00000000,    0,   1 * DAMAGE_MULTIPLIER,  eeGUY,        7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000000,   20,   1 * DAMAGE_MULTIPLIER,  eeGUY,        7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000000,   40,   1 * DAMAGE_MULTIPLIER,  eeGUY,        7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000000,   60,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000000,   80,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,     0,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  100,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,     0,    0,      0,       1,             0,     0,     0},
   // eNPCWALK1
   { 0x00000000,  120,   1 * DAMAGE_MULTIPLIER,  eeGUY,        7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,    50,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  140,   1 * DAMAGE_MULTIPLIER,  eeGUY,        7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,    50,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  160,   1 * DAMAGE_MULTIPLIER,  eeGUY,        7,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,    50,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  180,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,    50,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  200,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,    50,    0,      0,       1,             0,     0,     0},
   { 0x00000000,  220,   1 * DAMAGE_MULTIPLIER,  eeGUY,        8,  aOCTO,    12,   1 * HP_PER_HEART / 8,   1 * HP_PER_HEART / 8,   0,          4,    3,    50,    0,      0,       1,             0,     0,     0},
};

#endif
