//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zdefs.h
//
//  Data formats, definitions, and a few small functions.
//--------------------------------------------------------

#ifndef _ZDEFS_H_
#define _ZDEFS_H_

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <alport.h>
#include "zcdata.h"
#include "zc_exports.h"

#define SYSTEM_FILE     "zcdata.dat"      /* system file required for ZC */
#define ZCSFX_FILE      "zcsfx.dat"       /* opt sfx.dat used by some quests */
#define ZCSF_FILE       "zcsf.sf2"        /* sf2 file required for midi play */
#define ZELDA_VERSION   0x0210            /* version of the engine */
#define VERSION_BUILD   1                 /* build number of this version */
#define DATE_STR        "January 1, 2007" /* Original ZC Engine Build Date */

#define MIN_VERSION     0x0184

#define ZCDAT_VERSION   0x0210               /* version of zcdata.dat */
#define ZCDAT_BUILD     1                    /* build of zcdata.dat */

enum {ENC_METHOD_192B104 = 0, ENC_METHOD_192B105, ENC_METHOD_192B185, 
      ENC_METHOD_MAX};

#define PI 3.14159265358979323846

#define HP_PER_HEART          16
#define DAMAGE_MULTIPLIER     2

#define RETURN_ERROR \
{                    \
   success = false;  \
   goto error;       \
}

#define RETURN_ERROR_M(message)  \
{                                \
   success = false;              \
   zc_error(message);            \
   goto error;                   \
}

#define ZC_ID(a,b,c,d)  (((a)<<24) | ((b)<<16) | ((c)<<8) | (d))

//ID values of the different section types
#define ID_HEADER         ZC_ID('H','D','R',' ')   //quest header
#define ID_RULES          ZC_ID('R','U','L','E')   //quest rules
#define ID_STRINGS        ZC_ID('S','T','R',' ')   //message strings
#define ID_MISC           ZC_ID('M','I','S','C')   //misc data
#define ID_TILES          ZC_ID('T','I','L','E')   //tiles
#define ID_COMBOS         ZC_ID('C','M','B','O')   //combos
#define ID_CSETS          ZC_ID('C','S','E','T')   //csets (and pal names?)
#define ID_MAPS           ZC_ID('M','A','P',' ')   //maps
#define ID_DMAPS          ZC_ID('D','M','A','P')   //dmaps
#define ID_DOORS          ZC_ID('D','O','O','R')   //door combo sets
#define ID_ITEMS          ZC_ID('I','T','E','M')   //items
#define ID_WEAPONS        ZC_ID('W','P','N',' ')   //weapons
#define ID_COLORS         ZC_ID('C','L','R',' ')   //not sure
#define ID_ICONS          ZC_ID('I','C','O','N')   //not sure
#define ID_GRAPHICSPACK   ZC_ID('G','P','A','K')   //graphics pack header
#define ID_INITDATA       ZC_ID('I','N','I','T')   //initialization data
#define ID_GUYS           ZC_ID('G','U','Y',' ')   //guys
#define ID_MIDIS          ZC_ID('M','I','D','I')   //midis
#define ID_CHEATS         ZC_ID('C','H','T',' ')   //cheats
#define ID_SAVEGAME       ZC_ID('S','V','G','M')   //save game data

//Version number of the different section types
#define V_HEADER          1
#define V_RULES           1
#define V_STRINGS         1
#define V_MISC            1
#define V_TILES           1
#define V_COMBOS          1
#define V_CSETS           1
#define V_MAPS            2
#define V_DMAPS           1
#define V_DOORS           1
#define V_ITEMS           1
#define V_WEAPONS         1
#define V_COLORS          1
#define V_ICONS           1
#define V_GRAPHICSPACK    1
#define V_INITDATA        1
#define V_GUYS            1
#define V_MIDIS           1
#define V_CHEATS          1
#define V_SAVEGAME        1

/*
 * Compatible version number of the different section types
 * Basically, the last version number that this section type
 * is just an extension of, instead of a complete rewrite.
 * If this and the version number are the same, then this is
 * a total reworking of the section and probably won't be able
 * to be read by anything that was written for a previous version.
 */
#define CV_HEADER       1
#define CV_RULES        1
#define CV_STRINGS      1
#define CV_MISC         1
#define CV_TILES        1
#define CV_COMBOS       1
#define CV_CSETS        1
#define CV_MAPS         2
#define CV_DMAPS        1
#define CV_DOORS        1
#define CV_ITEMS        1
#define CV_WEAPONS      1
#define CV_COLORS       1
#define CV_ICONS        1
#define CV_GRAPHICSPACK 1
#define CV_INITDATA     1
#define CV_GUYS         1
#define CV_MIDIS        1
#define CV_CHEATS       1
#define CV_SAVEGAME     1

extern int readsize, writesize;

// system colors
#define BLACK  253
#define WHITE  254

#define SINGLE_TILE_SIZE    128
#define TILES_PER_ROW       20
#define TILE_ROWS_PER_PAGE  13
#define TILES_PER_PAGE      (TILES_PER_ROW*TILE_ROWS_PER_PAGE)
#define TILE_PAGES          252
#define OLDMAXTILES         (TILES_PER_PAGE * 6)               // 1560 tiles
#define NEWMAXTILES         (TILES_PER_PAGE * TILE_PAGES)      // 32760 tiles
#define NEWTILE_SIZE        (NEWMAXTILES * SINGLE_TILE_SIZE)   // 4193280 bytes (new packed format, 6 pages)
#define TILEBUF_SIZE        (320 * 480)                        // 153600 bytes (old unpacked format)

#define COMBOS_PER_ROW      20

// quest stuff
#define ZQ_TILES        0
#define ZQ_MIDIS2       1     //4 bytes
#define ZQ_CHEATS2      5
#define ZQ_MAXDATA      20

#define MAXMIDIS192b177 32    // uses bit string for midi flags, so 32 bytes
#define MAXMIDIS        252   // uses bit string for midi flags, so 32 bytes
#define MIDIFLAGS_SIZE  ((MAXMIDIS + 7) >> 3)

#define MAXMAPS2        255   // 4 times the old number
#define MAPSCRSNORMAL   128
#define MAPSCRS192b136  132
#define MAPSCRS         136
#define TEMPLATES         8
#define TEMPLATE        131
#define TEMPLATE2       132

#define MAXQTS          256
#define MAXMSGS         65535
#define MAXDOORCOMBOSETS 256
#define MAXDMAPS        256   //this and
#define MAXLEVELS       256   //this should be the same number (was 32)
#define MAXITEMS        256
#define MAXWPNS         256
#define MAXGUYS         256
#define COMBOS_PER_PAGE 256
#define COMBO_PAGES     255
#define MAXCOMBOS       COMBO_PAGES * COMBOS_PER_PAGE

#define MAGICPERBLOCK   32
#define PALNAMESIZE     17
// mapscr "valid" byte
#define mVALID          0x01
#define mVERSION        0x80

// lvlitems flags
#define liTRIFORCE      1
#define liMAP           2
#define liCOMPASS       4
#define liBOSS          8
#define liBOSSKEY       16


// door codes        meaning: type | shows on subscreen map
//                       bit: 4321    0
#define dWALL           0                                   // 0000    0
#define dOPEN           1                                   // 0000    1
#define dLOCKED         2                                   // 0001    0
#define dUNLOCKED       3                                   // 0001    1
#define dSHUTTER        4                                   // 0010    0
#define dBOMB           6                                   // 0011    0
#define dBOMBED         7                                   // 0011    1
#define dWALK           8                                   // 0100    0

#define dBOSS           10                                  // 0101    0
#define dOPENBOSS       11                                  // 0101    1

#define dOPENSHUTTER    12 // special case (don't show on map) 0110    0
#define d1WAYSHUTTER    14 // never opens                      0111    0

// screen flags
#define fSHUTTERS       1
#define fITEM           2
#define fDARK           4
#define fROAR           8
#define fWHISTLE        16
#define fLADDER         32
#define fMAZE           64
#define fSEA            128

// flags2
#define wfUP            1
#define wfDOWN          2
#define wfLEFT          4
#define wfRIGHT         8
#define fSECRET         16    // play "secret" sfx upon entering this screen
#define fVADER          32    // play "vader" sfx instead of "roar" sfx
#define fFLOATTRAPS     64
#define fCLEARSECRET    128   // clear all enemies to trigger secret entrance

// flags3
#define fHOLDITEM         1
#define fDODONGO          2   // play dodongo sfx instead of roar sfx
#define fINVISROOM        4
#define fINVISLINK        8
#define fNOSUBSCR         16
#define fIWARPFULLSCREEN  32  // instawarps affect all sprites
#define fNOSECRETSOUND    64  // never play secret sound on this screen
#define fENEMIESRETURN    128 // enemies always return

//flags4
#define fOVERHEADTEXT     1
#define fITEMWARP         2

// enemy flags
#define efZORA          1
#define efTRAP4         2
#define efTRAP2         4
#define efROCKS         8
#define efFIREBALLS     16
#define efLEADER        32
#define efCARRYITEM     64
#define efBOSS          128

// item "pick up" flags
#define ipBIGRANGE      1     // Collision rectangle is large
#define ipHOLDUP        2     // Link holds up item when he gets it
#define ipONETIME       4     // Getting this item sets the map flag
#define ipDUMMY         8     // Dummy item.  Can't get this.
#define ipCHECK         16    // Check restrictions (money in a shop, etc.)
#define ipMONEY         32    // This item causes money loss (bomb upgrade, swindle room, etc.)
#define ipFADE          64    // Blinks if fadeclk>0
#define ipENEMY         128   // Enemy is carrying it around
#define ipTIMER         256   // Disappears after a while
#define ipBIGTRI        512   // Large collision rectangle (used for large triforce)
#define ipNODRAW        1024  // Don't draw this (for underwater items)
#define ipTMPFLASH      2048  // not used (delete this?)

enum
{
   wtCAVE, wtPASS, wtEXIT, wtSCROLL, wtIWARP, wtIWARPBLK, wtIWARPOPEN,
   wtIWARPZAP, wtIWARPWAVE, wtNOWARP, wtWHISTLE, wtMAX
};

// sprite palettes
enum
{
   spAQUA, spGLEEOK, spDIG, spGANON, spBROWN, spPILE, spBLUE, spRED,
   spGOLD, spICON1, spICON2, spICON3, spICON4, spGLEEOKF, spFROZEN
};

// dmap types
enum { dmDNGN, dmOVERW, dmCAVE, dmBSOVERW, dmMAX };

// subscreen types
enum { sstOLD, sstNEWSUBSCR, sstREV2, sstMAX };

// dmap flags
#define dmfCONTINUE      128
#define dmfTYPE          127

// map flags
enum
{
   mfNONE, mfPUSHUD, mfPUSH4, mfWHISTLE, mfBCANDLE, mfARROW, mfBOMB, mfFAIRY,
   mfRAFT, mfARMOS_SECRET, mfARMOS_ITEM, mfSBOMB, mfRAFT_BRANCH, mfDIVE_ITEM,
   mfNONAME, mfZELDA,
   mfSECRETS01, mfSECRETS02, mfSECRETS03, mfSECRETS04,       /*16*/
   mfSECRETS05, mfSECRETS06, mfSECRETS07, mfSECRETS08,
   mfSECRETS09, mfSECRETS10, mfSECRETS11, mfSECRETS12,
   mfSECRETS13, mfSECRETS14, mfSECRETS15, mfSECRETS16,
   mfTRAP_H, mfTRAP_V, mfTRAP_4, mfTRAP_LR, mfTRAP_UD,       /*32*/
   mfENEMY0, mfENEMY1, mfENEMY2, mfENEMY3, mfENEMY4,         /*37*/
   mfENEMY5, mfENEMY6, mfENEMY7, mfENEMY8, mfENEMY9,
   mfPUSHLR, mfPUSHU, mfPUSHD, mfPUSHL, mfPUSHR,
   mfPUSHUDNS, mfPUSHLRNS, mfPUSH4NS, mfPUSHUNS,
   mfPUSHDNS, mfPUSHLNS, mfPUSHRNS, mfPUSHUDINS,
   mfPUSHLRINS, mfPUSH4INS, mfPUSHUINS,
   mfPUSHDINS, mfPUSHLINS, mfPUSHRINS,
   mfBLOCKTRIGGER, mfNOBLOCKS, mfBRANG, mfMBRANG,
   mfFBRANG, mfSARROW, mfGARROW, mfRCANDLE, mfWANDFIRE, mfDINSFIRE,
   mfWANDMAGIC, mfREFMAGIC, mfREFFIREBALL, mfSWORD, mfWSWORD,
   mfMSWORD, mfXSWORD, mfSWORDBEAM, mfWSWORDBEAM, mfMSWORDBEAM,
   mfXSWORDBEAM, mfHOOKSHOT, mfWAND, mfHAMMER, mfSTRIKE, mfMAX, mfPUSHED
};

// combo types
enum
{
   cNONE, cSTAIR, cCAVE, cWATER, cARMOS, cGRAVE, cDOCK,
   cUNDEF, cPUSH_WAIT, cPUSH_HEAVY, cPUSH_HW, cL_STATUE, cR_STATUE,
   cWALKSLOW, cCVUP, cCVDOWN, cCVLEFT, cCVRIGHT, cSWIMWARP, cDIVEWARP,
   cLADDERHOOKSHOT, cTRIGNOFLAG, cTRIGFLAG, cZELDA, cSLASH, cSLASHITEM,
   cPUSH_HEAVY2, cPUSH_HW2, cPOUND, cHSGRAB, cHSBRIDGE, cDAMAGE1,
   cDAMAGE2, cDAMAGE3, cDAMAGE4, cC_STATUE, cTRAP_H, cTRAP_V, cTRAP_4,
   cTRAP_LR, cTRAP_UD, cPIT, cHOOKSHOTONLY, cOVERHEAD, cNOFLYZONE, cMIRROR,
   cMIRRORSLASH, cMIRRORBACKSLASH, cMAGICPRISM, cMAGICPRISM4,
   cMAGICSPONGE, cCAVE2, cEYEBALL_A, cEYEBALL_B, cNOJUMPZONE, cBUSH,
   cFLOWERS, cTALLGRASS, cSHALLOWWATER, cLOCKBLOCK, cLOCKBLOCK2,
   cBOSSLOCKBLOCK, cBOSSLOCKBLOCK2, cLADDERONLY, cBSGRAVE, cMAX
};

#define QUESTRULES_SIZE 20

// "quest rules" flags (bit numbers in bit string)
enum
{
   qr_SOLIDBLK, qr_NOTMPNORET, qr_ALWAYSRET, qr_MEANTRAPS,
   qr_BSZELDA, qr_FADE, qr_FADECS5, qr_FASTDNGN,

   qr_NOLEVEL3FIX, qr_COOLSCROLL, qr_999R, qr_4TRI,
   qr_24HC, qr_FASTFILL, qr_VIEWMAP, qr_3TRI,

   qr_TIME, qr_FREEFORM, qr_KILLALL, qr_NOFLICKER,
   qr_CONTFULL, qr_RLFIX, qr_LENSHINTS, qr_LINKDUNGEONPOSFIX,

   qr_HOLDITEMANIMATION, qr_HESITANTPUSHBLOCKS, qr_HIDECARRIEDITEMS, 
   qr_SASPARKLES, qr_GASPARKLES, qr_MBSPARKLES, qr_FBSPARKLES, qr_NOFLASHDEATH,

   qr_KEEPOLDITEMS, qr_FIREPROOFLINK, qr_OUCHBOMBS, qr_NOCLOCKS,
   qr_TEMPCLOCKS, qr_BRKBLSHLDS, qr_BRKNSHLDTILES, qr_MEANPLACEDTRAPS,

   qr_PHANTOMPLACEDTRAPS, qr_ALLOWFASTMSG, qr_LINKEDCOMBOS, qr_NOGUYFIRES,
   qr_HEARTRINGFIX, qr_NOHEARTRING, qr_DODONGOCOLORFIX, qr_SWORDWANDFLIPFIX,

   qr_ENABLEMAGIC, qr_MAGICWAND, qr_MAGICCANDLE, qr_MAGICBOOTS,
   qr_NONBUBBLEMEDICINE, qr_NONBUBBLEFAIRIES, qr_NONBUBBLETRIFORCE, 
   qr_NEWENEMYTILES,

   qr_NOROPE2FLASH, qr_NOBUBBLEFLASH, qr_GHINI2BLINK, qr_WPNANIMFIX,
   qr_PHANTOMGHINI2, qr_Z3BRANG_HSHOT, qr_NOITEMMELEE, qr_SHADOWS,

   qr_TRANSSHADOWS, qr_QUICKSWORD, qr_BOMBHOLDFIX, qr_FULLLTM,
   qr_NOPOTIONCOMBINE, qr_LINKFLICKER, qr_SHADOWSFLICKER, qr_WALLFLIERS,

   qr_NOBOMBPALFLASH, qr_TRANSLUCENTNAYRUSLOVESHIELD, qr_FLICKERINGNAYRUSLOVESHIELD, 
   qr_TRANSLUCENTNAYRUSLOVEROCKET, qr_FLICKERINGNAYRUSLOVEROCKET, 
   qr_NOSCROLLCONTINUE, qr_OLDTRIBBLES,

   qr_MAX
};

// room types
enum
{
   rNONE, rSP_ITEM, rINFO, rMONEY, rGAMBLE, rREPAIR, rRP_HC, rGRUMBLE,
   rTRIFORCE, rP_SHOP, rSHOP, rBOMBS, rSWINDLE, r10RUPIES, rWARP,
   rGANON, rZELDA, rITEMPOND, rMUPGRADE, rLEARNSLASH,
   rMAX
};

// directions
enum {up, down, left, right, l_up, r_up, l_down, r_down};

// refill stuff
enum {REFILL_LIFE, REFILL_MAGIC, REFILL_ALL};
enum {REFILL_POTION, REFILL_FAIRY, REFILL_TRIFORCE};

// magic rates
#define LENSDRAINAMOUNT          2
#define LENSDRAINSPEED           1
#define WANDDRAINAMOUNT          8
#define CANDLEDRAINAMOUNT        4
#define DINSFIREDRAINAMOUNT     32
#define FARORESWINDDRAINAMOUNT  32
#define NAYRUSLOVEDRAINAMOUNT   64
#define BOOTSDRAINAMOUNT         1
#define BOOTSDRAINSPEED          1

//other
#define MAXDRUNKCLOCK 500

enum
{
   dBUSHLEAVES, dFLOWERCLIPPINGS, dGRASSCLIPPINGS, dHAMMERSMACK,
   dTALLGRASS, dRIPPLES, dNAYRUSLOVESHIELD, dMAXDECORATIONS
};

// items
enum  // value matters because of current quest file data
{
   iRupy, i5Rupies, iHeart, iBombs, iClock,
   iSword, iWSword, iMSword, iShield, iKey,
   iBCandle, iRCandle, iLetter, iArrow, iSArrow,             /*10*/
   iBow, iBait, iBRing, iRRing, iBracelet,
   iTriforce, iMap, iCompass, iBrang, iMBrang,               /*20*/
   iWand, iRaft, iLadder, iHeartC, iBPotion,
   iRPotion, iWhistle, iBook, iMKey, iFairyMoving,           /*30*/
   iFBrang, iXSword, iMShield, i20Rupies, i50Rupies,
   i200Rupies, iWallet500, iWallet999, iPile, iBigTri,       /*40*/
   iSelect, iMisc1, iMisc2, iSBomb, iHCPiece,
   iAmulet, iFlippers, iHookshot, iLens, iHammer,            /*50*/
   iBoots, iL2Bracelet, iGArrow, iMagicC, iSMagic,
   iLMagic, iGRing, iKillAll, iL2Amulet, iDinsFire,          /*60*/
   iFaroresWind, iNayrusLove, iBossKey, iBow2, iFairyStill, iMax
};

// item sets
enum
{
   isNONE, isDEFAULT, isBOMBS, isMONEY, isLIFE, isBOMB100, isSBOMB100,
   isMAGIC, isMAGICBOMBS, isMAGICMONEY, isMAGICLIFE, isMAGIC2
};

// weapons (in qst data)
enum
{
   wSWORD, wWSWORD, wMSWORD, wXSWORD, wBRANG, wMBRANG, wFBRANG, wBOMB,
   wSBOMB, wBOOM, wARROW, wSARROW, wFIRE, wWIND, wBAIT, wWAND, wMAGIC,
   ewFIREBALL, ewROCK, ewARROW, ewSWORD, ewMAGIC, iwSpawn, iwDeath, iwSwim,
   wHAMMER, wHSHEAD, wHSCHAIN_H, wHSHANDLE, wSSPARKLE, wGSPARKLE,
   wMSPARKLE, wFSPARKLE, iwHammerSmack, wGARROW, ewFLAME, ewWIND, iwMMeter,
   wDINSFIRE1A, wDINSFIRE1B, wDINSFIRES1A, wDINSFIRES1B, wHSCHAIN_V,
   iwMore, iwBossMarker, iwLinkSlash, wSWORDSLASH, wWSWORDSLASH,
   wMSWORDSLASH, wXSWORDSLASH, iwShadow, iwLargeShadow, iwBushLeaves,
   iwFlowerClippings, iwGrassClippings, iwTallGrass, iwRipples, iwNPCs,
   wNAYRUSLOVE1A, wNAYRUSLOVE1B, wNAYRUSLOVES1A, wNAYRUSLOVES1B,
   wNAYRUSLOVE2A, wNAYRUSLOVE2B, wNAYRUSLOVES2A, wNAYRUSLOVES2B,
   iwNayrusLoveShieldFront, iwNayrusLoveShieldBack, wMAX
};

// weapon types in game engine
enum
{
   wNone, wSword, wBeam, wBrang, wBomb, wSBomb, wLitBomb, wLitSBomb, wArrow,
   wFire, wWhistle, wBait, wWand, wMagic, wCatching, wWind, wRefMagic, wRefFireball,
   wEnemyWeapons,
   ewFireball, ewArrow, ewBrang, ewSword, ewRock, ewMagic, wHammer, wHookshot,
   wHSHandle, wHSChain, wSSparkle, wGSparkle, wMSparkle, wFSparkle,
   wSmack, wGArrow, ewFlame, ewWind, wPhantom
};

// phantom weapon types
enum
{
   pDINSFIREROCKET, pDINSFIREROCKETRETURN, pDINSFIREROCKETTRAIL,
   pDINSFIREROCKETTRAILRETURN, pMESSAGEMORE, pNAYRUSLOVEROCKET1,
   pNAYRUSLOVEROCKETRETURN1, pNAYRUSLOVEROCKETTRAIL1,
   pNAYRUSLOVEROCKETTRAILRETURN1, pNAYRUSLOVEROCKET2, pNAYRUSLOVEROCKETRETURN2,
   pNAYRUSLOVEROCKETTRAIL2, pNAYRUSLOVEROCKETTRAILRETURN2
};

enum
{
   gABEI = 1, gAMA, gDUDE, gMOBLIN, gFIRE,
   gFAIRY, gGORIYA, gZELDA, gDABEI,

   eROCTO1 = 10, eBOCTO1, eROCTO2, eBOCTO2, eRTEK,
   eBTEK, eRLEV, eBLEV, eRMOBLIN, eBMOBLIN,
   eRLYNEL, eBLYNEL, ePEAHAT, eZORA, eROCK,                    /*20*/
   eGHINI1, eGHINI2, eARMOS,

   eKEESE1, eKEESE2,
   eKEESE3, eSTALFOS, eGEL, eZOL, eROPE,                       /*30*/
   eRGORIYA, eBGORIYA, eTRAP, eWALLM, eRDKNUT,
   eBDKNUT, eBUBBLE, eVIRE, eLIKE, eGIBDO,                     /*40*/
   ePOLSV, eRWIZ, eBWIZ,

   eRAQUAM, eMOLDORM,
   eDODONGO, eMANHAN, eGLEEOK1, eGLEEOK2, eGLEEOK3, eGLEEOK4,  /*50*/
   eDIG1, eDIG3, eDIGPUP1, eDIGPUP2, eDIGPUP3, eDIGPUP4,
   eRGOHMA, eBGOHMA, eRCENT,
   eBCENT, ePATRA1, ePATRA2, eGANON,                           /*60*/
   eSTALFOS2,
   eROPE2, eRBUBBLE, eBBUBBLE, eFBALL, eITEMFAIRY,
   eFIRE, eCOCTO, eDKNIGHT, eGELTRIB, eZOLTRIB,                /*70*/
   eKEESETRIB, eVIRETRIB, eSDKNUT, eLAQUAM, eMANHAN2,
   eTRAP_H, eTRAP_V, eTRAP_LR, eTRAP_UD, eFWIZ,                /*80*/
   eWWIZ, eCEILINGM, eFLOORM, ePATRABS, ePATRAL2,
   /*90*/
   ePATRAL3, eBAT, eBATROBE, eBATROBEKING, eGLEEOK1F, eGLEEOK2F,
   eGLEEOK3F, eGLEEOK4F, eMWIZ, eDODONGOBS, eDODONGOF, eTRIGGER,
   eIBUBBLE, eRIBUBBLE, eBIBUBBLE, eSTALFOS3, eGOHMA3, eGOHMA4,
   eNPCSTAND1, eNPCSTAND2, eNPCSTAND3, eNPCSTAND4, eNPCSTAND5,
   eNPCSTAND6, eNPCWALK1, eNPCWALK2, eNPCWALK3, eNPCWALK4, eNPCWALK5,
   eNPCWALK6, eMAXGUYS
};

// enemy families
enum
{
   eeGUY, eeWALK, eeSHOOT, eeTEK, eeLEV, eePEAHAT, eeZORA, eeROCK,
   eeGHINI, eeARMOS, eeKEESE, eeGEL, eeZOL, eeROPE, eeGORIYA, eeTRAP,
   eeWALLM, eeBUBBLE, eeVIRE, eeLIKE, eePOLSV, eeWIZZ, eeAQUA, eeMOLD,
   eeDONGO, eeMANHAN, eeGLEEOK, eeDIG, eeGHOMA, eeLANM, eePATRA, eeGANON,
   eeFBALL, eeGELTRIB, eeZOLTRIB, eeVIRETRIB, eeKEESETRIB, eeNONE
};

// enemy animation styles
enum
{
   aNONE, aFLIP, a2FRM, aOCTO, aTEK, aLEV, aWALK, aZORA, aGHINI, aARMOS,
   aROPE, aWALLM, aDWALK, aVIRE, a3FRM, aWIZZ, aAQUA, aDONGO, aMANHAN,
   aGLEEOK, aDIG, aGHOMA, aLANM, aPHAT, aKEESE, aGANON
};

// enemy patters
enum {pRANDOM, pSIDES};

typedef struct itemdata
{
   uint16_t tile;
   uint8_t misc;        // 0000vhtf (vh:flipping, t:two hands, f:flash)
   uint8_t csets;       // ffffcccc (f:flash cset, c:cset)
   uint8_t frames;      // animation frame count
   uint8_t speed;       // animation speed
   uint8_t delay;       // extra delay factor (-1) for first frame
   //uint8_t padding;
   int32_t ltm;         // Link Tile Modifier
   uint8_t exp[10];     // not used
   //uint8_t padding[2];
   // 21 bytes (uses 24)
} itemdata;

typedef struct wpndata
{
   uint16_t tile;
   uint8_t misc;        // 0000vhff (vh:flipping, f:flash (1:NES, 2:BSZ))
   uint8_t csets;       // ffffcccc (f:flash cset, c:cset)
   uint8_t frames;      // animation frame count
   uint8_t speed;       // animation speed
   uint8_t type;        // used by certain weapons
   uint8_t exp;         // not used
   // 8 bytes
} wpndata;

typedef struct quest_template
{
   char name[31];
   char path[280];
   //311 bytes
} quest_template;

#define guy_bhit        0x00000001
#define guy_invisible   0x00000002
#define guy_neverret    0x00000004
#define guy_superman    0x00000008

#define guy_sbombonly   0x00000010
#define inv_brang       0x00000020
#define inv_bomb        0x00000040
#define inv_sbomb       0x00000080

#define inv_arrow       0x00000100
#define inv_L2arrow     0x00000200
#define inv_fire        0x00000400
#define inv_wand        0x00000800

#define inv_magic       0x00001000
#define inv_hookshot    0x00002000
#define inv_hammer      0x00004000
#define inv_L3brang     0x00008000

#define inv_L1sword     0x00010000
#define inv_L3sword     0x00020000
#define inv_L1beam      0x00040000
#define inv_L3beam      0x00080000

#define inv_refbeam     0x00100000
#define inv_refmagic    0x00200000
#define inv_refball     0x00400000
#define inv_extra       0x00800000

#define inv_front       0x01000000
#define inv_left        0x02000000
#define inv_right       0x04000000
#define inv_back        0x08000000

#define weak_whistle    0x10000000
#define weak_arrow      0x20000000
#define weak_L3brang    0x40000000
#define lens_only       0x80000000

typedef struct guydata
{
   uint32_t flags;
   uint16_t  tile;
   short hp;

   short  family, cset, anim, frate;
   short  dp, wdp, weapon;

   short  rate, hrate, step, homing, grumble;
   short  item_set, misc1, misc2, bosspal;

   short  startx, starty;
   short  foo1, foo2, foo3, foo4, foo5, foo6;
   // 56 bytes
} guydata;

typedef struct mapscr2
{
   uint8_t valid;
   uint8_t guy;
   uint16_t str;
   uint8_t room;
   uint8_t item;
   uint8_t warptype;
   //uint8_t padding;
   uint16_t door_combo_set;
   uint8_t warpreturnx;
   uint8_t warpreturny;
   uint8_t stairx;
   uint8_t stairy;
   uint8_t itemx;
   uint8_t itemy;
   uint8_t color;
   uint8_t enemyflags;
   uint8_t door[4];
   uint8_t warpdmap;
   uint8_t warpscr;
   uint8_t exitdir;
   uint8_t _FOO3_;
   uint16_t enemy[10];
   uint8_t pattern;
   uint8_t warparrivalx;
   uint8_t warparrivaly;
   uint8_t path[4];
   uint8_t sidewarptype;
   uint8_t sidewarpscr;
   uint8_t sidewarpdmap;
   uint8_t itemwarptype;
   uint8_t itemwarpdmap;
   uint8_t itemwarpscr;
   uint16_t undercombo;
   uint8_t old_cpage;
   uint8_t undercset;
   uint16_t catchall;
   uint8_t flags;
   uint8_t flags2;
   uint8_t flags3;
   uint8_t flags4;
   uint8_t layermap[6];
   uint8_t layerscreen[6];
   uint8_t layerxsize[6];
   uint8_t layerxspeed[6];
   uint8_t layerxdelay[6];
   uint8_t layerysize[6];
   uint8_t layeryspeed[6];
   uint8_t layerydelay[6];
   uint8_t layeropacity[6];
   //uint8_t padding;
   uint16_t timedwarptics;
   uint8_t extra[62];
   uint16_t secretcombo[128];
   uint8_t secretcset[128];
   uint8_t secretflag[128];
   uint16_t data[16 * 11];
   uint8_t sflag[16 * 11];
   uint8_t cset[16 * 11];
} mapscr2;

typedef struct mapscr
{
   uint8_t valid;
   uint8_t guy;
   uint16_t str;
   uint8_t room;
   uint8_t item;
   uint8_t tilewarptype;
   //uint8_t padding;
   uint16_t door_combo_set;
   uint8_t warpreturnx;
   uint8_t warpreturny;
   uint8_t stairx;
   uint8_t stairy;
   uint8_t itemx;
   uint8_t itemy;
   uint8_t color;
   uint8_t enemyflags;
   uint8_t door[4];
   uint8_t tilewarpdmap;
   uint8_t tilewarpscr;
   uint8_t exitdir;
   uint8_t _FOO3_;
   uint16_t enemy[10];
   uint8_t pattern;
   uint8_t sidewarptype;
   uint8_t warparrivalx;
   uint8_t warparrivaly;
   uint8_t path[4];
   uint8_t sidewarpscr;
   uint8_t sidewarpdmap;
   uint16_t undercombo;
   uint8_t old_cpage;
   uint8_t undercset;
   uint16_t catchall;
   uint8_t flags;
   uint8_t flags2;
   uint8_t flags3;
   uint8_t layermap[6];
   uint8_t layerscreen[6];
   uint8_t layerxsize[6];
   uint8_t layerxspeed[6];
   uint8_t layerxdelay[6];
   uint8_t layerysize[6];
   uint8_t layeryspeed[6];
   uint8_t layerydelay[6];
   uint8_t layeropacity[6];
   //uint8_t padding;
   uint16_t timedwarptics;
   uint8_t extra[62];
   uint16_t secretcombo[128];
   uint8_t secretcset[128];
   uint8_t secretflag[128];
   uint16_t data[16 * 11];
   uint8_t sflag[16 * 11];
   uint8_t cset[16 * 11];
} mapscr;


enum
{
   sBCANDLE, sARROW, sBOMB, sSTAIRS, sSECRET01, sSECRET02, sSECRET03,
   sSECRET04, sSECRET05, sSECRET06, sSECRET07, sSECRET08, sSECRET09,
   sSECRET10, sSECRET11, sSECRET12, sSECRET13, sSECRET14, sSECRET15,
   sSECRET16, sRCANDLE, sWANDFIRE, sDINSFIRE, sSARROW, sGARROW,
   sSBOMB, sBRANG, sMBRANG, sFBRANG, sWANDMAGIC, sREFMAGIC, sREFFIREBALL,
   sSWORD, sWSWORD, sMSWORD, sXSWORD, sSWORDBEAM, sWSWORDBEAM,
   sMSWORDBEAM, sXSWORDBEAM, sHOOKSHOT, sWAND, sHAMMER, sSTRIKE
};

enum {cfOFFSET, cfMAX};

typedef struct newcombo
{
   uint16_t tile;
   uint8_t flip;
   uint8_t walk;
   uint8_t type;
   uint8_t csets;
   uint16_t foo;   //do not change!  used for positioning!  no idea why.
   uint8_t frames;
   uint8_t speed;
   uint16_t nextcombo;
   uint8_t nextcset;
   uint8_t expansion[11];
   //24
} newcombo;

typedef struct tiletype
{
   uint8_t bitplanes;
   uint8_t *data;
} tiletype;

typedef struct ZCHEATS
{
   uint32_t flags;
   char  codes[4][41];
   //168
} ZCHEATS;

typedef struct zquestheader
{
   char  id_str[31];
   //byte  padding;
   //32
   short zelda_version;
   uint16_t  internal;
   uint8_t  quest_number;
   uint8_t  old_rules[2];
   uint8_t  old_map_count;
   char  old_str_count;
   //41
   uint8_t  data_flags[ZQ_MAXDATA];
   uint8_t  old_rules2[2];
   char  old_options;
   char  version[9];
   //73
   char  title[65];
   char  author[65];
   //byte  padding;
   //204
   short pwdkey;
   char  password[30];
   //236
   char  minver[9];
   uint8_t  build;
   uint8_t  use_keyfile;
   uint8_t  old_foo[9];
   uint8_t  old_rules3[2];
   uint8_t  old_rules4[2];
   uint8_t  old_rules5[2];
   uint8_t  old_rules6[2];
   uint8_t  old_rules7[2];
   uint8_t  old_rules8[2];
   uint8_t  old_rules9[2];
   uint8_t  old_rules10[2];
   uint8_t  old_midi_flags[MIDIFLAGS_SIZE];
   //304
   uint8_t  old_foo2[18];
   char  templatepath[280];
   //602
} zquestheader;

enum {msLINKED};

typedef struct MsgStr
{
   char s[73];
   //byte padding
   //74
   uint16_t nextstring;
   uint16_t expansion[16];
   //108
} MsgStr;

enum {dt_pass = 0, dt_lock, dt_shut, dt_boss, dt_olck, dt_osht, dt_obos, 
      dt_wall, dt_bomb, dt_walk, dt_max};
enum {df_walktrans = 0};

typedef struct DoorComboSet
{
   char name[21];
   //byte padding;
   //22
   uint16_t doorcombo_u[9][4];      //[door type][door combo]
   uint8_t doorcset_u[9][4];        //[door type][door combo]
   uint16_t doorcombo_d[9][4];      //[door type][door combo]
   uint8_t doorcset_d[9][4];        //[door type][door combo]
   //238 (216)
   uint16_t doorcombo_l[9][6];      //[door type][door combo]
   uint8_t doorcset_l[9][6];        //[door type][door combo]
   uint16_t doorcombo_r[9][6];      //[door type][door combo]
   uint8_t doorcset_r[9][6];        //[door type][door combo]
   //562 (324)
   uint16_t bombdoorcombo_u[2];     //rubble
   uint8_t bombdoorcset_u[2];       //rubble
   uint16_t bombdoorcombo_d[2];     //rubble
   uint8_t bombdoorcset_d[2];       //rubble
   //574 (12)
   uint16_t bombdoorcombo_l[3];     //rubble
   uint8_t bombdoorcset_l[3];       //rubble
   //uint8_t padding;
   uint16_t bombdoorcombo_r[3];     //rubble
   uint8_t bombdoorcset_r[3];       //rubble
   //uint8_t padding;
   //594 (18)
   uint16_t walkthroughcombo[4];    //[n, s, e, w]
   uint8_t walkthroughcset[4];      //[n, s, e, w]

   uint8_t flags[2];
   uint8_t expansion[30];
   //638 (44)
} DoorComboSet;

typedef struct dmap
{
   uint8_t map;
   uint8_t level;
   char xoff;
   uint8_t compass;
   uint8_t color;
   uint8_t midi;
   uint8_t cont;
   uint8_t type;
   //8
   uint8_t grid[8];
   //16
   char name[21];
   char title[21];
   char intro[73];
   //byte padding;
   //132
   uint16_t minimap_1_tile;   //before getting map
   uint8_t minimap_1_cset;    //cset for minimap 1
   //byte padding;
   uint16_t minimap_2_tile;   //after getting map
   uint8_t minimap_2_cset;    //cset for minimap 2
   //byte padding;
   //140
   uint16_t largemap_1_tile;  //large map
   uint8_t largemap_1_cset;   //cset for large
   //byte padding;
   uint16_t largemap_2_tile;  //large map
   uint8_t largemap_2_cset;   //cset for large
   char tmusic[56];
   //byte padding;
   //204
} dmap;

typedef struct shoptype
{
   uint8_t item[3];
   uint8_t d1;
   uint16_t price[3];
   //10
} shoptype;

typedef struct pondtype
{
   uint8_t olditem[3];
   uint8_t d1;
   uint8_t newitem[3];
   uint8_t fairytile;
   uint8_t aframes;
   uint8_t aspeed;
   uint16_t msg[15];
   uint8_t foo[32];
   //72
} pondtype;

typedef struct infotype192b145
{
   uint8_t str[3];
   uint8_t d1;
   uint16_t price[3];
   //10
} infotype192b145;

typedef struct infotype
{
   uint16_t str[3];
   uint8_t d1;
   //byte padding;
   uint16_t price[3];
   //14
} infotype;

typedef struct warpring
{
   uint8_t dmap[8];
   uint8_t scr[8];
   uint8_t size;
   uint8_t d1;
   //18
} warpring;

typedef struct windwarp
{
   uint8_t dmap;
   uint8_t scr;

   //2
} windwarp;

typedef struct zcolors
{
   uint8_t text, caption;
   uint8_t overw_bg, dngn_bg;
   uint8_t dngn_fg, cave_fg;
   uint8_t bs_dk, bs_goal;
   uint8_t compass_lt, compass_dk;
   //10
   uint8_t subscr_bg, triframe_color;
   uint8_t link_dot;
   uint8_t bmap_bg, bmap_fg;
   //15
   uint8_t triforce_cset;
   uint8_t triframe_cset;
   uint8_t overworld_map_cset;
   uint8_t dungeon_map_cset;
   uint8_t blueframe_cset;
   //20
   uint16_t triforce_tile;
   uint16_t triframe_tile;
   uint16_t overworld_map_tile;
   uint16_t dungeon_map_tile;
   uint16_t blueframe_tile;
   //30
   uint16_t HCpieces_tile;
   uint8_t HCpieces_cset;
   uint8_t foo[7];
   //40
   uint8_t foo2[256];
   //296 bytes
} zcolors;

typedef struct palcycle
{
   uint8_t first, count, speed;
   //3
} palcycle;

typedef struct miscQdata
{
   shoptype shop[16];
   //160 (160=10*16)
   infotype info[16];
   //384 (224=14*16)
   warpring warp[8];
   //528 (144=18*8)
   palcycle cycles[256][3];
   //2832 (2304=3*256*3)
   windwarp wind[9];          // destination of whirlwind for each level
   //2850 (18=2*2)
   uint8_t triforce[8];       // positions of triforce pieces on subscreen
   //2858 (8)
   zcolors  colors;
   //3154 (296)
   uint16_t icons[4];
   //3162 (8=2*4)
   pondtype pond[16];
   //4314 (1152=72*16)
   uint16_t endstring;
   //  byte dummy;            // left over from a word
   uint16_t expansion[98];
   //4512
} miscQdata;

typedef struct music
{
   char title[20];
   //20
   int32_t start;
   int32_t loop_start;
   int32_t loop_end;
   //32
   short loop;
   short volume;
   //36
   void *midi;
   //40
} music;

enum
{
   itype_sword, itype_brang, itype_arrow, itype_candle, itype_whistle,
   itype_bait, itype_letter, itype_potion, itype_wand, itype_ring,
   itype_wallet, itype_amulet, itype_shield, itype_bow, itype_raft,
   itype_ladder, itype_book, itype_magickey, itype_bracelet,
   itype_flippers, itype_boots, itype_hookshot, itype_lens, itype_hammer,
   itype_dinsfire, itype_faroreswind, itype_nayruslove, itype_bomb,
   itype_sbomb, itype_clock, itype_key, itype_magiccontainer,
   itype_triforcepiece, itype_map, itype_compass, itype_bosskey,
   itype_max
};

enum {i_sword = 1, i_wsword, i_msword, i_xsword, imax_sword};
enum {i_wbrang = 1, i_mbrang, i_fbrang, imax_brang};
enum {i_warrow = 1, i_sarrow, i_garrow, imax_arrow};
enum {i_bcandle = 1, i_rcandle, imax_candle};
enum {i_recorder = 1, imax_whistle};
enum {i_bait = 1, imax_bait};
enum {i_letter = 1, i_letter_used, imax_letter};
enum {i_bpotion = 1, i_rpotion, imax_potion};
enum {i_wand = 1, imax_wand};
enum {i_bring = 1, i_rring, i_gring, imax_ring};
enum {i_swallet = 1, i_lwallet, imax_wallet};
enum {i_amulet1 = 1, i_amulet2, imax_amulet};
enum {i_smallshield = 1, i_largeshield, i_mirrorshield, imax_shield};
enum {i_shortbow = 1, i_longbow, imax_bow};
enum {i_raft = 1, imax_raft};
enum {i_ladder = 1, imax_ladder};
enum {i_book = 1, imax_book};
enum {i_magickey = 1, imax_magickey};
enum {i_bracelet1 = 1, i_bracelet2, imax_bracelet};
enum {i_flippers = 1, imax_flippers};
enum {i_boots = 1, imax_boots};
enum {i_hookshot = 1, i_longshot, imax_hookshot};
enum {i_lens = 1, imax_lens};
enum {i_hammer = 1, imax_hammer};
enum {i_dinsfire = 1, imax_dinsfire};
enum {i_faroreswind = 1, imax_faroreswind};
enum {i_nayruslove = 1, imax_nayruslove};

typedef struct gamedata
{
   char  name[9];
   uint8_t  quest;
   //10
   short life, maxlife, drupy, rupies, deaths;
   //20
   uint8_t  keys, maxbombs, wlevel, cheat;
   //24
   uint8_t  items[MAXITEMS];
   //280

   char  version[9];
   char  title[65];
   //354
   uint8_t  hasplayed;
   //byte  padding;
   //356
   uint32_t time;
   //360
   uint8_t  timevalid;
   uint8_t  lvlitems[256];
   uint8_t  HCpieces;
   uint8_t  continue_scrn;
   uint8_t  continue_dmap;
   //620
   short maxmagic;
   short magic;
   short dmagic;
   uint8_t  magicdrainrate;
   uint8_t  canslash;               // Link slashes instead of stabs.
   //636
   uint8_t  visited[MAXDMAPS];
   //892 (256)
   uint8_t  bmaps[MAXDMAPS * 64];   // the dungeon progress maps
   //17276 (16384)
   uint16_t  maps[MAXMAPS2 * 128];  // info on map changes, items taken, etc.
   //82556 (65280)
   uint8_t  guys[MAXMAPS2 * 128];   // guy counts (though dungeon guys are reset on entry)
   //115196 (32640)
   char  qstpath[1024];
   uint8_t  icon[128];
   uint8_t  pal[48];
   //115456 (260)
} gamedata;


// "initialization data" flags (bit numbers in bit string)
enum
{
   idE_RAFT, idE_LADDER, idE_BOOK, idE_KEY,
   idE_FLIPPERS, idE_BOOTS, idE_MAX
};
enum
{
   idI_WAND, idI_LETTER, idI_LENS, idI_HOOKSHOT,
   idI_BAIT, idI_HAMMER, idI_MAX
};
enum { idI_DFIRE, idI_FWIND, idI_NLOVE, idI_MAX2 };
enum { idM_CONTPERCENT, idM_DOUBLEMAGIC, idM_CANSLASH, idM_MAX };
enum
{
   idBP_WOODENPERCENT, idBP_WHITEPERCENT,
   idBP_MAGICPERCENT, idBP_MASTERPERCENT, idBP_MAX
};

typedef struct zinitdata
{
   uint8_t raft, ladder, book, key, flippers, boots;
   uint8_t ring, sword, shield, wallet, bracelet, amulet, bow;
   uint8_t more_eq[32];
   //45
   uint8_t candle, boomerang, arrow, potion, whistle, bombs, super_bombs;
   uint8_t wand, letter, lens, hookshot, bait, hammer, dins_fire, farores_wind;
   uint8_t nayrus_love, cloak;
   uint8_t more_items[32];
   //94
   uint8_t hc, start_heart, cont_heart, hcp, max_bombs, keys;
   uint16_t rupies;
   uint8_t triforce;             // bit flags
   uint8_t map[32];
   uint8_t compass[32];
   uint8_t boss_key[32];
   uint8_t misc[16];
   uint8_t sword_hearts[4];
   uint8_t last_map;             // last map worked on
   //220
   uint8_t last_screen;          // last screen worked on
   uint8_t max_magic;
   uint8_t magic;
   uint8_t beam_hearts[4];
   uint8_t beam_percent;         // bit flags
   //228
   uint8_t beam_power[4];
   uint8_t hookshot_links;
   uint8_t msg_more_x, msg_more_y;
   uint8_t subscreen;
   uint8_t start_dmap;
   uint8_t linkwalkstyle;
   //238
   uint8_t expansion[98];
   //336 bytes total
} zinitdata;


/******************/
/**  Misc Stuff  **/
/******************/

#define max(a,b)  ((a)>(b)?(a):(b))
#define min(a,b)  ((a)<(b)?(a):(b))

#define DCLICK_START      0
#define DCLICK_RELEASE    1
#define DCLICK_AGAIN      2
#define DCLICK_NOT        3

template <class T>
static inline void swap(T &a, T &b)
{
   T c = a;
   a = b;
   b = c;
}

inline bool pfwrite(void *p, long n, PACKFILE *f)
{
   bool success = (pack_fwrite(p, n, f) == n);
   if (success)
      writesize += n;
   return success;
}

inline bool pfread(void *p, long n, PACKFILE *f, bool keepdata)
{
   bool success;
   if (keepdata == true)
   {
      success = (pack_fread(p, n, f) == n);
      if (success)
         readsize += n;
      return success;
   }
   else
   {
      success = (pack_fseek(f, n) == 0);
      if (success)
         readsize += n;
      return success;
   }
}

inline bool p_getc(void *p, PACKFILE *f, bool keepdata)
{
   unsigned char *cp = (unsigned char *)p;
   int c;
   if (!f)
      return false;

   if (f->normal.flags & PACKFILE_FLAG_WRITE)
   {
      return false;    //must not be writing to file
   }

   if (pack_feof(f))
      return false;
   c = pack_getc(f);
   if (pack_ferror(f))
      return false;
   if (keepdata == true)
      *cp = c;
   readsize += 1;
   return true;
}

inline bool p_putc(int c, PACKFILE *f)
{
   if (!f)
      return false;

   if (!(f->normal.flags & PACKFILE_FLAG_WRITE))
   {
      return false;    //must be writing to file
   }

   pack_putc(c, f);
   bool success = (pack_ferror(f) == 0);
   if (success)
      writesize += 1;
   return success;
}

inline bool p_igetw(void *p, PACKFILE *f, bool keepdata)
{
   short *cp = (short *)p;
   int c;
   
   if (!f)
      return false;

   if (f->normal.flags & PACKFILE_FLAG_WRITE)
   {
      return false;    //must not be writing to file
   }

   if (pack_feof(f))
      return false;
   c = pack_igetw(f);
   if (pack_ferror(f))
      return false;
   if (keepdata == true)
      *cp = c;
   readsize += 2;
   return true;
}

inline bool p_iputw(int c, PACKFILE *f)
{
   if (!f)
      return false;

   if (!(f->normal.flags & PACKFILE_FLAG_WRITE))
   {
      return false;    //must be writing to file
   }

   pack_iputw(c, f);
   bool success = (pack_ferror(f) == 0);
   if (success)
      writesize += 2;
   return success;
}

inline bool p_igetl(void *p, PACKFILE *f, bool keepdata)
{
   uint32_t *cp = (uint32_t *)p;
   int32_t c;
   if (!f)
      return false;

   if (f->normal.flags & PACKFILE_FLAG_WRITE)
   {
      return false;    //must not be writing to file
   }

   if (pack_feof(f))
      return false;
   c = pack_igetl(f);
   if (pack_ferror(f))
      return false;
   if (keepdata == true)
      *cp = c;
   readsize += 4;
   return true;
}

inline bool p_iputl(long c, PACKFILE *f)
{
   if (!f)
      return false;

   if (!(f->normal.flags & PACKFILE_FLAG_WRITE))
   {
      return false;    //must be writing to file
   }

   pack_iputl(c, f);
   bool success = (pack_ferror(f) == 0);
   if (success)
      writesize += 4;
   return success;
}

inline bool p_mgetw(void *p, PACKFILE *f, bool keepdata)
{
   short *cp = (short *)p;
   int c;
   if (!f)
      return false;

   if (f->normal.flags & PACKFILE_FLAG_WRITE)
   {
      return false;    //must not be writing to file
   }

   if (pack_feof(f))
      return false;
   c = pack_mgetw(f);
   if (pack_ferror(f))
      return false;
   if (keepdata == true)
      *cp = c;
   readsize += 2;
   return true;
}

inline bool p_mputw(int c, PACKFILE *f)
{
   if (!f)
      return false;

   if (!(f->normal.flags & PACKFILE_FLAG_WRITE))
   {
      return false;    //must be writing to file
   }

   pack_mputw(c, f);
   bool success = (pack_ferror(f) == 0);
   if (success)
      writesize += 2;
   return success;
}

inline bool p_mgetl(void *p, PACKFILE *f, bool keepdata)
{
   uint32_t *cp = (uint32_t *)p;
   int32_t c;
   
   if (!f)
      return false;

   if (f->normal.flags & PACKFILE_FLAG_WRITE)
   {
      return false;    //must not be writing to file
   }

   if (pack_feof(f))
      return false;
   c = pack_mgetl(f);
   if (pack_ferror(f))
      return false;
   if (keepdata == true)
      *cp = c;
   readsize += 4;
   return true;
}

inline bool p_mputl(long c, PACKFILE *f)
{
   if (!f)
      return false;

   if (!(f->normal.flags & PACKFILE_FLAG_WRITE))
   {
      return false;    //must be writing to file
   }

   pack_mputl(c, f);
   bool success = (pack_ferror(f) == 0);
   if (success)
      writesize += 4;
   return success;
}

inline bool isinRect(int x, int y, int rx1, int ry1, int rx2, int ry2)
{
   return x >= rx1 && x <= rx2 && y >= ry1 && y <= ry2;
}
#endif      //_ZDEFS_H_
