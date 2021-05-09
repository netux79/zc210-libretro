//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  pal.h
//
//  Palette code for ZC.
//
//--------------------------------------------------------

#ifndef _ZC_PAL_H_
#define _ZC_PAL_H_

#include "zdefs.h"

// offsets in "data sets"
#define poFULL   0                                          // main palette
#define poLEVEL  15                                         // first level palette
#define poFADE1  15+4                                       //
#define poFADE2  15+7                                       //
#define poFADE3  15+10                                      //
#define oldpoSPRITE 210                                     // first sprite pal
#define newpoSPRITE 3343                                    // first sprite pal
#define pSprite(x) ((x)+newpoSPRITE)

// palette data sizes in "data sets"
#define pdFULL   15                                         // number of csets in the full palette
#define pdLEVEL  13                                         // complete level pal (normal & fade pals)
#define pdFADE   3                                          // size of a fade pal
#define oldpdTOTAL  240                                     // total data sets
#define newpdTOTAL  3373                                    // total data sets

// palette data sizes in bytes
#define psFULL   ((pdFULL<<4)*3)
#define psLEVEL  ((pdLEVEL<<4)*3)
#define psFADE   ((pdFADE<<4)*3)
#define psTOTAL  ((newpdTOTAL<<4)*3)

extern const byte nes_pal[];

#define CSET_SIZE 16                                        // this is only changed to 4 in the NES title screen
#define CSET_SHFT 4                                         // log2 of CSET_SIZE
#define CSET(x)         ((x)<<CSET_SHFT)
#define csBOSS          14

RGB _RGB(const byte *si);
RGB _RGB(int r, int g, int b);
void loadfullpal();
void loadlvlpal(int level);
void loadpalset(int cset, int dataset);
void loadfadepal(int dataset);
void ringcolor();
void fade(int level, bool blackall, bool fromblack, bool total);
void lighting(int funct, int dir);
void dryuplake();
void rehydratelake();
void reset_pal_cycling();
void cycle_palette();
int reverse_NESpal(RGB c);

inline RGB NESpal(int i)
{
   return   _RGB(nes_pal + i * 3);
}

#endif                                                      // _ZC_PAL_H_
