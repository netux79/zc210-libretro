//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zelda.h
//
//  Definitions, function prototypes, etc. for zelda.cpp
//
//--------------------------------------------------------

#ifndef _ZELDA_H_
#define _ZELDA_H_

/*********************************/
/********** Definitions **********/
/*********************************/

#include "zdefs.h"
#include "zc_sys.h"

#define  MAXMUSIC           MUSIC_COUNT + MAXMIDIS

// saved games, 6 saves = 2 screens
#define MAXSAVES            6

// game.maps[] flags
#define mSECRET             1      // only overworld and caves use this
#define mVISITED            2      // only overworld uses this

#define mDOOR_UP            1      // only dungeons use this
#define mDOOR_DOWN          2      //        ''
#define mDOOR_LEFT          4      //        ''
#define mDOOR_RIGHT         8      //        ''

#define mITEM               16     // item (main screen)
#define mBELOW              32     // special item (underground)
#define mNEVERRET           64     // enemy never returns
#define mTMPNORET           128    // enemies don't return until you leave the dungeon

#define mLOCKBLOCK          256    // if the lockblock on the screen has been triggered
#define mBOSSLOCKBLOCK      512    // if the bosslockblock on the screen has been triggered

/*********************************/
/******** Enums & Structs ********/
/*********************************/

// magic types
enum  {mgc_none, mgc_dinsfire, mgc_nayruslove, mgc_faroreswind};

// "special" walk flags
enum
{
   spw_none, spw_door, spw_clipright, spw_floater, spw_trap, spw_halfstep,
   spw_water, spw_wizzrobe, spw_clipbottomright
};

// death & fade defs
enum
{
   fade_none, fade_flicker, fade_invisible, fade_flash_die,
   fade_blue_poof
};

/*********************************/
/*********** Procedures **********/
/*********************************/

// zelda.cpp
void addLwpn(int x, int y, int id, int type, int power, int dir);
void ALLOFF(void);
fix LinkX(void);
fix LinkY(void);
int LinkNayrusLoveShieldClk(void);
int LinkLStep(void);
fix LinkModifiedX(void);
fix LinkModifiedY(void);
fix GuyX(int j);
fix GuyY(int j);
int GuyID(int j);
int GuyMisc(int j);
void StunGuy(int j);
bool GuySuperman(int j);
int GuyCount(void);
int LinkDir(void);
void add_grenade(int wx, int wy, int size);
fix distance(int x1, int y1, int x2, int y2);
bool getClock(void);
void setClock(bool state);
void CatchBrang(void);
int LinkAction(void);
void dointro(void);
int init_game(void);
int cont_game(void);
void restart_level(void);
void resume_game(void);
void reset_status(void);

/**********************************/
/******** Global Variables ********/
/**********************************/

extern int db;
extern zinitdata zinit;
extern int lens_hint_item[MAXITEMS][2];   //aclk, aframe
extern int lens_hint_weapon[MAXWPNS][5];  //aclk, aframe, dir, x, y
extern int strike_hint_counter;
extern int strike_hint_timer;
extern int strike_hint;

extern BITMAP *zc_canvas;      /* screen canvas buffer to send to libretro */
extern BITMAP *framebuf, *scrollbuf, *tempbuf, *msgdisplaybuf, *pricesdisplaybuf;
extern DATAFILE *data, *sfxdata, *mididata;
extern FONT *zfont, *font;
extern PALETTE RAMpal;
extern byte *tilebuf, *colordata;
extern newcombo *combobuf;
extern itemdata *itemsbuf;
extern wpndata *wpnsbuf;
extern guydata *guysbuf;
extern ZCHEATS zcheats;
extern byte use_tiles;

//[0]=position in act2, [1]=original tile
extern word animated_combo_table[MAXCOMBOS][2];
extern word animated_combo_table4[MAXCOMBOS][2]; //[0]=combo, [1]=clock
extern word animated_combos;
extern bool blank_tile_table[NEWMAXTILES]; //keeps track of blank tiles
extern bool blank_tile_quarters_table[NEWMAXTILES * 4]; //keeps track of blank tiles
extern bool ewind_restart;
extern word msgclk, msgstr, msgpos, msg_count;
extern word door_combo_set_count;
extern word introclk, intropos, dmapmsgclk, linkedmsgclk;
extern short Bpos, lensclk, lenscnt;
extern byte screengrid[22];
extern bool screenscrolling;
extern bool anymsg, anyprice;

extern int homescr, currscr, frame, currmap, dlevel, warpscr, worldscr;
extern int newscr_clk, opendoors, currdmap, fadeclk, currgame, listpos;
extern int lastentrance, lastentrance_dmap, prices[3][2], loadside, Bwpn, Awpn;
extern int sel_music, wand_x, wand_y, hasitem, whistleclk;
extern int arrow_x, arrow_y, brang_x, brang_y, chainlink_x, chainlink_y;
extern int hs_startx, hs_starty, hs_xdist, hs_ydist, clockclk, clock_zoras;
extern int swordhearts[4], currcset, gfc, gfc2, pitx, pity, refill_what;
extern int heart_beep_timer, new_enemy_tile_start, nets, magictype;
extern int magiccastclk, castx, casty, df_x, df_y, nl1_x, nl1_y, nl2_x, nl2_y,
       magicdrainclk, conveyclk;
extern bool nosecretsounds;
extern bool blockmoving;
extern bool is_playing;
extern bool blockpath, wand_dead, loaded_guys, freeze_guys;
extern bool loaded_enemies, drawguys, watch;
extern bool Udown, Ddown, Ldown, Rdown, Adown, Bdown, Edown, Sdown, Mdown,
       LBdown, RBdown, Mdown;
extern bool fixed_door, darkroom, BSZ, COOLSCROLL;
extern bool hookshot_used, hookshot_frozen, pull_link, add_chainlink;
extern bool del_chainlink, hs_fix, checklink;
extern bool ewind_restart, didpit, castnext;
extern bool add_df1asparkle, add_df1bsparkle, add_nl1asparkle, add_nl1bsparkle,
       add_nl2asparkle, add_nl2bsparkle;
extern bool is_on_conveyor, activated_timed_warp;

extern int add_asparkle, add_bsparkle;

extern short  visited[6];
extern byte   guygrid[176];
extern mapscr tmpscr[2];
extern mapscr tmpscr2[6];
extern mapscr tmpscr3[6];

extern int cheat;  // 0 = none; 1,2,3,4 = cheat level

extern char qst_name[256];
extern gamedata *saves;
extern gamedata game;

// quest file data
extern zquestheader QHeader;
extern byte         quest_rules[QUESTRULES_SIZE];
extern byte         midi_flags[MIDIFLAGS_SIZE];
extern word         map_count;
extern MsgStr       *MsgStrings;
extern DoorComboSet *DoorComboSets;
extern dmap         *DMaps;
extern miscQdata    QMisc;
extern mapscr       *TheMaps;

/**********************************/
/*********** Misc Data ************/
/**********************************/

extern const char startguy[8];
extern const char gambledat[12 * 6];
extern const byte stx[4][9];
extern const byte sty[4][9];
extern const byte ten_rupies_x[10];
extern const byte ten_rupies_y[10];
extern music tunes[MAXMUSIC];
#endif
