//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zelda.cpp
//
//  Main code for Zelda Classic. Originally written in
//  SPHINX C--, now rewritten in DJGPP with Allegro.
//
//--------------------------------------------------------

#include <stdio.h>
#include "zdefs.h"
#include "zelda.h"
#include "tiles.h"
#include "pal.h"
#include "qst.h"
#include "particles.h"
#include "link.h"
#include "sprite.h"
#include "maps.h"
#include "subscr.h"
#include "guys.h"
#include "title.h"
#include "ending.h"
#include "zc_sys.h"

/**********************************/
/******** Global Variables ********/
/**********************************/

bool blockmoving;
movingblock mblock2;
sprite_list  guys, items, Ewpns, Lwpns, Sitems, chainlinks, decorations,
             particles;
LinkClass   Link;

int db = 0;
zinitdata  zinit;
int lens_hint_item[MAXITEMS][2];
int lens_hint_weapon[MAXWPNS][5];
int strike_hint_counter = 0;
int strike_hint_timer = 0;
int strike_hint;

/* pointers to bitmap and palette objects used by Libretro */
BITMAP *zc_canvas;
RGB *zc_palette;

BITMAP *framebuf, *scrollbuf, *tempbuf, *msgdisplaybuf, *pricesdisplaybuf;
DATAFILE *data = NULL, *sfxdata, *mididata;
FONT *zfont, *font;
PALETTE RAMpal;
byte *tilebuf, *colordata;
newcombo *combobuf;
itemdata *itemsbuf;
wpndata *wpnsbuf;
guydata *guysbuf;
ZCHEATS zcheats;
byte use_tiles, oldflags3;
word animated_combo_table[MAXCOMBOS][2];  //[0]=position in act2, [1]=original tile
word animated_combo_table4[MAXCOMBOS][2]; //[0]=combo, [1]=clock
word animated_combos;
bool blank_tile_table[NEWMAXTILES]; //keeps track of blank tiles
bool blank_tile_quarters_table[NEWMAXTILES * 4];  //keeps track of blank tiles
bool ewind_restart = false;
word msgclk, msgstr, msgpos, msg_count;
word door_combo_set_count;
word introclk, intropos, dmapmsgclk, linkedmsgclk;
short Bpos, lensclk, lenscnt;
byte screengrid[22];
bool screenscrolling = false;
bool anymsg = false;
bool anyprice = false;

int readsize, writesize;

int homescr, currscr, frame = 0, currmap = 0, dlevel, warpscr, worldscr;
int newscr_clk = 0, opendoors = 0, currdmap = 0, fadeclk = -1, currgame = 0,
    listpos = 0;
int lastentrance = 0, lastentrance_dmap = 0, prices[3][2], loadside, Bwpn, Awpn;
int master_vol, sfx_vol, music_vol, pan_style, mix_quality;
int sel_music, wand_x, wand_y, hasitem, whistleclk;
int Akey, Bkey, Ekey, Skey, Lkey, Rkey, Mkey;
int DUkey, DDkey, DLkey, DRkey, zc_state = ZC_RUN;
int arrow_x, arrow_y, brang_x, brang_y, chainlink_x, chainlink_y;
int hs_startx, hs_starty, hs_xdist, hs_ydist, clockclk, clock_zoras;
int swordhearts[4], currcset;
int gfc, gfc2, pitx, pity, refill_what, heart_beep_timer = 0,
                                        new_enemy_tile_start = 1580;
int nets = 1580, magictype = mgc_none, magiccastclk, castx, casty, df_x, df_y,
    nl1_x, nl1_y, nl2_x, nl2_y;
int magicdrainclk = 0, conveyclk = 3;
int checkx, checky;
bool nosecretsounds = false;
bool trans_layers, heart_beep;
bool is_playing;
bool zc_sync_pal, blockpath, wand_dead, loaded_guys, freeze_guys,
     loaded_enemies, drawguys, watch;
bool darkroom = false, BSZ, COOLSCROLL;
bool Udown, Ddown, Ldown, Rdown, Adown, Bdown, Edown, Sdown, LBdown, RBdown,
     Mdown,
     fixed_door = false, hookshot_used = false, hookshot_frozen = false,
     pull_link = false, add_chainlink = false, del_chainlink = false,
     checklink = true, didpit = false, hs_fix = false,
     castnext = false, add_df1asparkle, add_df1bsparkle, add_nl1asparkle,
     add_nl1bsparkle, add_nl2asparkle, add_nl2bsparkle,
     is_on_conveyor, activated_timed_warp = false;

int  add_asparkle = 0, add_bsparkle = 0;

short  visited[6];
byte   guygrid[176];
mapscr tmpscr[2];
mapscr tmpscr2[6];
mapscr tmpscr3[6];
gamedata game;

int cheat = 0;       // 0 = none; 1,2,3,4 = cheat level

// quest file data
zquestheader QHeader;
byte         quest_rules[QUESTRULES_SIZE];
byte         midi_flags[MIDIFLAGS_SIZE];
word         map_count;
MsgStr       *MsgStrings;
DoorComboSet *DoorComboSets;
dmap         *DMaps;
miscQdata    QMisc;
mapscr       *TheMaps;

char qst_name[256];
gamedata *saves = NULL;

/**********************************/
/*********** Misc Data ************/
/**********************************/

const char startguy[8] = {-13, -13, -13, -14, -15, -18, -21, -40};
const char gambledat[12 * 6] =
{
   20, -10, -10, 20, -10, -10, 20, -40, -10, 20, -10, -40,
   50, -10, -10, 50, -10, -10, 50, -40, -10, 50, -10, -40,
   -10, 20, -10, -10, 20, -10, -40, 20, -10, -10, 20, -40,
   -10, 50, -10, -10, 50, -10, -40, 50, -10, -10, 50, -40,
   -10, -10, 20, -10, -10, 20, -10, -40, 20, -40, -10, 20,
   -10, -10, 50, -10, -10, 50, -10, -40, 50, -40, -10, 50
};
const byte stx[4][9] =
{
   { 48, 80, 80, 96, 112, 144, 160, 160, 192},
   { 48, 80, 80, 96, 128, 144, 160, 160, 192},
   { 80, 80, 128, 128, 160, 160, 192, 192, 208},
   { 32, 48, 48, 80, 80, 112, 112, 160, 160}
};
const byte sty[4][9] =
{
   {112, 64, 128, 96, 80, 96, 64, 128, 112},
   { 48, 32, 96, 64, 80, 64, 32, 96, 48},
   { 32, 128, 64, 96, 64, 96, 48, 112, 80},
   { 80, 48, 112, 64, 96, 64, 96, 32, 128}
};

const byte ten_rupies_x[10] = {120, 112, 128, 96, 112, 128, 144, 112, 128, 120};
const byte ten_rupies_y[10] = {49, 65, 65, 81, 81, 81, 81, 97, 97, 113};

music tunes[MAXMUSIC] =
{
   // (title)                (s) (ls) (le) (l) (vol) (midi)
   { "Zelda 1 - Dungeon",     0,  -1,  -1,  1,  176,  NULL },
   { "Zelda 1 - Ending",      0, 129, 225,  1,  160,  NULL },
   { "Zelda 1 - Game Over",   0,  -1,  -1,  1,  224,  NULL },
   { "Zelda 1 - Level 9",     0,  -1,  -1,  1,  255,  NULL },
   { "Zelda 1 - Overworld",   0,  17,  -1,  1,  208,  NULL },
   { "Zelda 1 - Title",       0,  -1,  -1,  0,  100,  NULL },
   { "Zelda 1 - Triforce",    0,  -1,  -1,  0,  168,  NULL },
};

void dointro(void)
{
   if (game.visited[currdmap] != 1)
   {
      dmapmsgclk = 0;
      game.visited[currdmap] = 1;
      introclk = intropos = 0;
   }
}

bool bad_version(int version)
{
   // minimum zquest version allowed for any quest file
   if (version < 0x183)
      return true;

   return false;
}

void addLwpn(int x, int y, int id, int type, int power, int dir)
{
   Lwpns.add(new weapon((fix)x, (fix)y, id, type, power, dir));

}

void ALLOFF(void)
{
   clear_bitmap(msgdisplaybuf);
   anymsg = false;
   clear_bitmap(pricesdisplaybuf);
   anyprice = false;
   if (items.idCount(iPile))
      loadlvlpal(DMaps[currdmap].color);
   items.clear();
   guys.clear();
   Lwpns.clear();
   Ewpns.clear();
   chainlinks.clear();
   decorations.clear();
   if (Link.getNayrusLoveShieldClk())
      Link.setNayrusLoveShieldClk(Link.getNayrusLoveShieldClk());
   Link.resetflags(false);
   Link.reset_hookshot();
   linkedmsgclk = 0;
   add_asparkle = 0;
   add_bsparkle = 0;
   add_df1asparkle = false;
   add_df1bsparkle = false;
   add_nl1asparkle = false;
   add_nl1bsparkle = false;
   add_nl2asparkle = false;
   add_nl2bsparkle = false;
   mblock2.clk = 0;
   msgstr = 0;
   fadeclk = -1;

   lensclk = lenscnt = 0;
   drawguys = Udown = Ddown = Ldown = Rdown = Adown = Bdown = Sdown = true;
   if (watch)
      Link.setClock(false);
   watch = freeze_guys = loaded_guys = loaded_enemies = wand_dead = blockpath =
            false;
   stop_sfx(SFX_BRANG);
   for (int i = 0; i < 176; i++)
      guygrid[i] = 0;
   sle_clk = 0;
   blockmoving = false;
}

fix LinkX(void)
{
   return Link.getX();
}

fix LinkY(void)
{
   return Link.getY();
}

int LinkHClk(void)
{
   return Link.getHClk();
}

int LinkNayrusLoveShieldClk(void)
{
   return Link.getNayrusLoveShieldClk();
}

int LinkLStep(void)
{
   return Link.getLStep();
}

fix GuyX(int j)
{
   return guys.getX(j);
}

fix GuyY(int j)
{
   return guys.getY(j);
}

int GuyID(int j)
{
   return guys.getID(j);
}

int GuyMisc(int j)
{
   return guys.getMisc(j);
}

bool GuySuperman(int j)
{
   if ((j >= guys.Count()) || (j < 0))
      return true;
   return ((enemy *)guys.spr(j))->superman;
}

int GuyCount(void)
{
   return guys.Count();
}
void StunGuy(int j)
{
   ((enemy *)guys.spr(j))->stunclk = 160;
}

fix LinkModifiedX(void)
{
   return Link.getModifiedX();
}

fix LinkModifiedY(void)
{
   return Link.getModifiedY();
}

int LinkDir(void)
{
   return Link.getDir();
}

void add_grenade(int wx, int wy, int size)
{
   if (size)
   {
      Lwpns.add(new weapon((fix)wx, (fix)wy, wSBomb, 0, 16 * DAMAGE_MULTIPLIER,
                           LinkDir()));
      Lwpns.spr(Lwpns.Count() - 1)->id = wSBomb;
   }
   else
   {
      Lwpns.add(new weapon((fix)wx, (fix)wy, wBomb, 0, 4 * DAMAGE_MULTIPLIER,
                           LinkDir()));
      Lwpns.spr(Lwpns.Count() - 1)->id = wBomb;
   }
   Lwpns.spr(Lwpns.Count() - 1)->clk = 41;
}

fix distance(int x1, int y1, int x2, int y2)
{
   return (fix)sqrt(pow(abs(x1 - x2), 2) + pow(abs(y1 - y2), 2));
}

bool getClock(void)
{
   return Link.getClock();
}

void setClock(bool state)
{
   Link.setClock(state);
}

void CatchBrang(void)
{
   Link.Catch();
}

/**************************/
/***** Main Game Code *****/
/**************************/

void reset_status(void)
{
   Link.setDontDraw(false);
   show_subscreen_dmap_dots = true;
   show_subscreen_numbers = true;
   show_subscreen_items = true;
   show_subscreen_life = true;
   introclk = intropos = 0;
   reset_combo_animations();
}

int init_game(void)
{
   didpit = false;
   Link.unfreeze();
   Link.reset_hookshot();
   Link.reset_ladder();
   linkedmsgclk = 0;
   blockmoving = false;
   add_asparkle = 0;
   add_bsparkle = 0;
   add_df1asparkle = false;
   add_df1bsparkle = false;
   add_nl1asparkle = false;
   add_nl1bsparkle = false;
   add_nl2asparkle = false;
   add_nl2bsparkle = false;
   srand(frame);
   for (int x = 0; x < MAXITEMS; x++)
   {
      lens_hint_item[x][0] = 0;
      lens_hint_item[x][1] = 0;
   }

   for (int x = 0; x < MAXWPNS; x++)
   {
      lens_hint_weapon[x][0] = 0;
      lens_hint_weapon[x][1] = 0;
   }

   /* copy saved data to RAM data */
   game = saves[currgame];
   cheat = 0;

   BSZ = get_bit(quest_rules, qr_BSZELDA);
   setuplinktiles(zinit.linkwalkstyle);
   COOLSCROLL = get_bit(quest_rules, qr_COOLSCROLL);
   if (!game.hasplayed)
      game.continue_dmap = zinit.start_dmap;
   currdmap = warpscr = worldscr = game.continue_dmap;
   if (game.continue_scrn >= 0x80)
   {
      if ((DMaps[currdmap].type & dmfTYPE) == dmOVERW)
         homescr = currscr = DMaps[currdmap].cont;

      else
         homescr = currscr = DMaps[currdmap].cont + DMaps[currdmap].xoff;
   }
   else
      homescr = currscr = game.continue_scrn;
   lastentrance = currscr;
   lastentrance_dmap = currdmap;
   currmap = DMaps[currdmap].map;
   dlevel = DMaps[currdmap].level;
   sle_x = sle_y = newscr_clk = opendoors = Bwpn = Bpos = 0;
   activated_timed_warp = false;
   fadeclk = -1;
   game.maps[(currmap << 7) + currscr] |= mVISITED; // mark as visited

   for (int i = 0; i < 6; i++)
      visited[i] = -1;
   game.lvlitems[9] &= ~liBOSS;

   ALLOFF();
   whistleclk = -1;
   clockclk = 0;
   currcset = DMaps[currdmap].color;
   darkroom = false;
   loadscr(0, currscr, up);
   putscr(scrollbuf, 0, 0, &tmpscr[0]);
   Link.init();
   Link.resetflags(true);

   loadfullpal();
   ringcolor();
   loadlvlpal(DMaps[currdmap].color);

   if (!game.hasplayed)
   {
      game.maxlife = zinit.hc * HP_PER_HEART;
      if (zinit.sword > 0)
         game.items[itype_sword] = zinit.sword;
      if (zinit.boomerang > 0)
         game.items[itype_brang] = zinit.boomerang;
      game.items[itype_bomb] = zinit.bombs;
      if (zinit.arrow > 0)
         game.items[itype_arrow] = zinit.arrow;
      if (zinit.candle > 0)
         game.items[itype_candle] = zinit.candle;
      if (zinit.whistle > 0)
         game.items[itype_whistle] = zinit.whistle;
      if (zinit.potion > 0)
         game.items[itype_potion] = zinit.potion;
      if (zinit.ring > 0)
      {
         game.items[itype_ring] = zinit.ring;
         ringcolor();
      }
      game.keys = zinit.keys;
      game.maxbombs = zinit.max_bombs;
      game.items[itype_wallet] = zinit.wallet;
      game.items[itype_sbomb] = zinit.super_bombs;
      game.HCpieces = zinit.hcp;
      game.rupies = zinit.rupies;
      if (zinit.letter > 0)
         game.items[itype_letter] = zinit.letter;
      if (zinit.bait)
         game.items[itype_bait] = zinit.bait;
      if (zinit.wand)
         game.items[itype_wand] = zinit.wand;
      if (zinit.dins_fire)
         game.items[itype_dinsfire] = i_dinsfire;
      if (zinit.farores_wind)
         game.items[itype_faroreswind] = i_faroreswind;
      if (zinit.nayrus_love)
         game.items[itype_nayruslove] = i_nayruslove;
      if (zinit.bracelet > 0)
         game.items[itype_bracelet] = zinit.bracelet;
      if (zinit.bow > 0)
         game.items[itype_bow] = zinit.bow;
      if (zinit.shield > 0)
         game.items[itype_shield] = zinit.shield;
      if (zinit.raft > 0)
         game.items[itype_raft] = zinit.raft;
      if (zinit.ladder > 0)
         game.items[itype_ladder] = zinit.ladder;
      if (zinit.book > 0)
         game.items[itype_book] = zinit.book;
      if (zinit.key > 0)
         game.items[itype_magickey] = zinit.key;
      if (zinit.amulet > 0)
         game.items[itype_amulet] = zinit.amulet;
      if (zinit.flippers > 0)
         game.items[itype_flippers] = zinit.flippers;
      if (zinit.boots > 0)
         game.items[itype_boots] = zinit.boots;
      if (zinit.hookshot > 0)
         game.items[itype_hookshot] = zinit.hookshot;
      if (zinit.lens > 0)
         game.items[itype_lens] = zinit.lens;
      if (zinit.hammer > 0)
         game.items[itype_hammer] = zinit.hammer;


      for (int i = 0; i < MAXLEVELS; i++)
      {
         game.lvlitems[i] = 0;
         game.lvlitems[i] |= get_bit(zinit.map, i) ? liMAP : 0;
         game.lvlitems[i] |= get_bit(zinit.compass, i) ? liCOMPASS : 0;
         game.lvlitems[i] |= get_bit(zinit.boss_key, i) ? liBOSSKEY : 0;
      }

      game.maxmagic = zinit.max_magic * MAGICPERBLOCK;
      game.magic = zinit.magic * MAGICPERBLOCK;
      game.magicdrainrate = get_bit(zinit.misc, idM_DOUBLEMAGIC) ? 1 : 2;
      game.canslash = get_bit(zinit.misc, idM_CANSLASH) ? 1 : 0;

   }

   for (int x = 0; x < 4; x++)
      swordhearts[x] = zinit.sword_hearts[x];

   if (!game.hasplayed)
      game.life = zinit.start_heart * HP_PER_HEART;

   else
   {
      if (get_bit(zinit.misc, idM_CONTPERCENT))
         game.life = ((game.maxlife * zinit.cont_heart / 100) / HP_PER_HEART) *
                     HP_PER_HEART;
      else
         game.life = zinit.cont_heart * HP_PER_HEART;
   }
   game.hasplayed = 1;

   if (get_bit(quest_rules, qr_CONTFULL))
      game.life = game.maxlife;

   selectBwpn(0, 0);
   selectAwpn(0);
   reset_subscr_items();

   Link.setDontDraw(false);
   show_subscreen_dmap_dots = true;
   show_subscreen_items = true;
   show_subscreen_numbers = true;
   show_subscreen_life = true;

   is_playing = true;
   lighting(2, Link.getDir());
   map_bkgsfx();
   openscreen();
   show_subscreen_numbers = true;
   show_subscreen_life = true;
   dointro();
   loadguys();

   if (isdungeon() && currdmap > 0)
      Link.stepforward(12);

   if (!zc_state)
      play_DmapMusic();

   return 0;
}

int cont_game()
{
   didpit = false;
   Link.unfreeze();
   Link.reset_hookshot();
   Link.reset_ladder();
   linkedmsgclk = 0;
   blockmoving = 0;
   add_asparkle = 0;
   add_bsparkle = 0;
   add_df1asparkle = false;
   add_df1bsparkle = false;
   add_nl1asparkle = false;
   add_nl1bsparkle = false;
   add_nl2asparkle = false;
   add_nl2bsparkle = false;
   currdmap = lastentrance_dmap;
   homescr = currscr = lastentrance;
   currmap = DMaps[currdmap].map;
   dlevel = DMaps[currdmap].level;

   for (int i = 0; i < 6; i++)
      visited[i] = -1;
   if (dlevel == 0)
      game.lvlitems[9] &= ~liBOSS;

   ALLOFF();
   whistleclk = -1;
   currcset = DMaps[currdmap].color;
   darkroom = false;
   loadscr(0, currscr, up);
   putscr(scrollbuf, 0, 0, &tmpscr[0]);

   loadfullpal();
   ringcolor();
   loadlvlpal(DMaps[currdmap].color);

   Link.init();

   if (get_bit(zinit.misc, idM_CONTPERCENT))
      game.life = ((game.maxlife * zinit.cont_heart / 100) / HP_PER_HEART) *
                  HP_PER_HEART;

   else
      game.life = zinit.cont_heart * HP_PER_HEART;

   if (get_bit(quest_rules, qr_CONTFULL))
      game.life = game.maxlife;

   is_playing = true;
   lighting(2, Link.getDir());
   map_bkgsfx();
   openscreen();
   show_subscreen_numbers = true;
   show_subscreen_life = true;
   loadguys();

   if (!zc_state)
   {
      play_DmapMusic();
      if (isdungeon())
         Link.stepforward(12);
      newscr_clk = frame;
      activated_timed_warp = false;
   }
   return 0;
}

void resume_game(void)
{
   music_resume();
   resume_all_sfx();
   is_playing = true;
   // restore subscreen status
   tmpscr->flags3 = oldflags3;
}

void restart_level(void)
{
   blackscr(16, true);
   if (dlevel)
   {
      currdmap = lastentrance_dmap;
      homescr = currscr = lastentrance;
   }
   else
   {
      if ((DMaps[currdmap].type & dmfTYPE) == dmOVERW)
         homescr = currscr = DMaps[currdmap].cont;

      else
         homescr = currscr = DMaps[currdmap].cont + DMaps[currdmap].xoff;
   }

   currmap = DMaps[currdmap].map;
   dlevel = DMaps[currdmap].level;
   for (int i = 0; i < 6; i++)
      visited[i] = -1;

   ALLOFF();
   whistleclk = -1;
   darkroom = false;
   loadscr(0, currscr, up);
   putscr(scrollbuf, 0, 0, &tmpscr[0]);

   loadfullpal();
   ringcolor();
   loadlvlpal(DMaps[currdmap].color);
   Link.init();
   lighting(2, Link.getDir());
   map_bkgsfx();
   openscreen();
   show_subscreen_numbers = true;
   show_subscreen_life = true;
   loadguys();

   if (!zc_state)
   {
      play_DmapMusic();
      if (isdungeon())
         Link.stepforward(12);
      newscr_clk = frame;
      activated_timed_warp = false;
   }
}

void putintro(void)
{
   if (!strcmp("                                                                        ",
                DMaps[currdmap].intro))
   {
      introclk = intropos = 72;
      return;
   }

   if (intropos >= 72)
      return;

   if (((introclk++) % 6 < 5) && ((!cAbtn() && !cBbtn())
                                  || (!get_bit(quest_rules, qr_ALLOWFASTMSG))))
      return;

   dmapmsgclk = 51;
   if (intropos == 0)
   {
      while (DMaps[currdmap].intro[intropos] == ' ')
         ++intropos;
   }

   sfx(SFX_MSG);


   // set the msg flag to we know we have a msg to show
   anymsg = true;
   textprintf_ex(msgdisplaybuf, zfont, ((intropos % 24) << 3) + 32,
                 ((intropos / 24) << 3) + 40, CSET(0) + 1, 0,
                 "%c", DMaps[currdmap].intro[intropos]);

   ++intropos;

   if (DMaps[currdmap].intro[intropos] == ' '
         && DMaps[currdmap].intro[intropos + 1] == ' ')
      while (DMaps[currdmap].intro[intropos] == ' ')
         ++intropos;

   if (intropos >= 72)
      dmapmsgclk = 50;

}

void do_magic_casting(void)
{
   static int tempx, tempy;
   static byte linktilebuf[256];
   int ltile = 0;
   int lflip = 0;
   switch (magictype)
   {
      case mgc_none:
         magiccastclk = 0;
         break;
      case mgc_dinsfire:
      {
         int flamemax = 32;
         if (magiccastclk == 0)
         {
            Lwpns.add(new weapon(LinkX(), LinkY(), wPhantom, pDINSFIREROCKET, 0, up));
            weapon *w1 = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
            w1->step = 4;
            linktile(&Link.tile, &Link.flip, ls_hold2, Link.getDir(), zinit.linkwalkstyle);
            casty = Link.getY();
         }
         if (magiccastclk == 64)
         {
            Lwpns.add(new weapon((fix)LinkX(), (fix)(-32), wPhantom, pDINSFIREROCKETRETURN,
                                 0, down));
            weapon *w1 = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
            w1->step = 4;
            linktile(&Link.tile, &Link.flip, ls_hold2, Link.getDir(), zinit.linkwalkstyle);
            castnext = false;
         }
         if (castnext)
         {
            linktile(&Link.tile, &Link.flip, ls_cast, Link.getDir(), zinit.linkwalkstyle);
            for (int flamecounter = ((-1) * (flamemax / 2)) + 1;
                  flamecounter < ((flamemax / 2) + 1); flamecounter++)
            {
               Lwpns.add(new weapon((fix)LinkX(), (fix)LinkY(), wFire, 3,
                                    8 * DAMAGE_MULTIPLIER, 0));
               weapon *w = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
               w->step = 2;
               w->angular = true;
               w->angle = (flamecounter * PI / (flamemax / 2));
            }
            castnext = false;
            magiccastclk = 128;
         }

         if ((magiccastclk++) == 226)
            magictype = mgc_none;
      }
      break;
      case mgc_faroreswind:
      {
         if (magiccastclk == 0)
         {
            linktile(&ltile, &lflip, ls_stab, down, zinit.linkwalkstyle);
            unpack_tile(ltile, lflip, true);
            memcpy(linktilebuf, unpackbuf, 256);
            tempx = Link.getX();
            tempy = Link.getY();
            linktile(&Link.tile, &Link.flip, ls_pound, down, zinit.linkwalkstyle);
         }
         if (magiccastclk >= 0 && magiccastclk < 64)
         {
            Link.setX(tempx + ((rand() % 3) - 1));
            Link.setY(tempy + ((rand() % 3) - 1));
         }
         if (magiccastclk == 64)
         {
            Link.setX(tempx);
            Link.setY(tempy);
            linktile(&Link.tile, &Link.flip, ls_stab, down, zinit.linkwalkstyle);
         }
         if (magiccastclk == 96)
         {
            Link.setDontDraw(true);
            for (int i = 0; i < 16; ++i)
            {
               for (int j = 0; j < 16; ++j)
               {
                  if (linktilebuf[i * 16 + j])
                  {
                     particles.add(new pFaroresWindDust(Link.getX() + j, Link.getY() + i, 5, 6,
                                                        linktilebuf[i * 16 + j], rand() % 96));
                     int k = particles.Count() - 1;
                     particle *p = (particle *)(particles.spr(k));
                     p->angular = true;
                     p->angle = rand();
                     p->step = (((double)j) / 8);
                     p->yofs = Link.getYOfs();
                  }
               }
            }
         }
         if ((magiccastclk++) == 226)
         {
            restart_level();
            magictype = mgc_none;
            Link.setDontDraw(false);
         }
      }
      break;
      case mgc_nayruslove:
      {
         if (magiccastclk == 0)
         {
            Lwpns.add(new weapon(LinkX(), LinkY(), wPhantom, pNAYRUSLOVEROCKET1, 0, left));
            weapon *w1 = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
            w1->step = 4;
            Lwpns.add(new weapon(LinkX(), LinkY(), wPhantom, pNAYRUSLOVEROCKET2, 0, right));
            w1 = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
            w1->step = 4;
            linktile(&Link.tile, &Link.flip, ls_cast, Link.getDir(), zinit.linkwalkstyle);
            castx = Link.getX();
         }
         if (magiccastclk == 64)
         {
            int d = max(LinkX(), 256 - LinkX()) + 32;
            Lwpns.add(new weapon((fix)(LinkX() - d), (fix)LinkY(), wPhantom,
                                 pNAYRUSLOVEROCKETRETURN1, 0, right));
            weapon *w1 = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
            w1->step = 4;
            Lwpns.add(new weapon((fix)(LinkX() + d), (fix)LinkY(), wPhantom,
                                 pNAYRUSLOVEROCKETRETURN2, 0, left));
            w1 = (weapon *)(Lwpns.spr(Lwpns.Count() - 1));
            w1->step = 4;
            linktile(&Link.tile, &Link.flip, ls_cast, Link.getDir(), zinit.linkwalkstyle);
            castnext = false;
         }
         if (castnext)
         {
            linktile(&Link.tile, &Link.flip, ls_hold2, Link.getDir(), zinit.linkwalkstyle);
            Link.setNayrusLoveShieldClk(512);
            castnext = false;
            magiccastclk = 128;
         }

         if ((magiccastclk++) == 160)
            magictype = mgc_none;
      }
      break;
      default:
         magiccastclk = 0;
         break;
   }
}

void update_hookshot(void)
{
   int hs_x, hs_y, hs_dx, hs_dy;
   bool check_hs = false;
   int dist_bx, dist_by, hs_w;
   chainlinks.animate();

   //find out where the head is and make it
   //easy to reference
   if (Lwpns.idFirst(wHookshot) > -1)
      check_hs = true;
   if (check_hs)
   {
      hs_x = Lwpns.spr(Lwpns.idFirst(wHookshot))->x;
      hs_y = Lwpns.spr(Lwpns.idFirst(wHookshot))->y;
      hs_dx = hs_x - hs_startx;
      hs_dy = hs_y - hs_starty;
      //extending
      if (((weapon *)Lwpns.spr(Lwpns.idFirst(wHookshot)))->misc == 0)
      {
         if (chainlinks.Count() < zinit.hookshot_links)        //extending chain
         {
            if (abs(hs_dx) >= hs_xdist + 8)
            {
               hs_xdist = abs(hs_x - hs_startx);
               chainlinks.add(new weapon((fix)hs_x, (fix)hs_y, wHSChain, 0, 0, Link.getDir()));
            }
            else if (abs(hs_dy) >= hs_ydist + 8)
            {
               hs_ydist = abs(hs_y - hs_starty);
               chainlinks.add(new weapon((fix)hs_x, (fix)hs_y, wHSChain, 0, 0, Link.getDir()));
            }
         }                                                     //stretching chain
         else
         {
            dist_bx = (abs(hs_dx) - (8 * chainlinks.Count())) / (chainlinks.Count() + 1);
            dist_by = (abs(hs_dy) - (8 * chainlinks.Count())) / (chainlinks.Count() + 1);
            hs_w = 8;
            if (hs_dx < 0)
            {
               dist_bx = 0 - dist_bx;
               hs_w = -8;
            }
            if (hs_dy < 0)
            {
               dist_by = 0 - dist_by;
               hs_w = -8;
            }
            for (int counter = 0; counter < chainlinks.Count(); counter++)
            {
               if (Link.getDir() > down)                         //chain is moving horizontally
                  chainlinks.spr(counter)->x = hs_startx + hs_w + dist_bx + (counter *
                                               (hs_w + dist_bx));

               else
                  chainlinks.spr(counter)->y = hs_starty + hs_w + dist_by + (counter *
                                               (hs_w + dist_by));
            }
         }
      }                                                       //retracting
      else if (((weapon *)Lwpns.spr(Lwpns.idFirst(wHookshot)))->misc == 1)
      {
         dist_bx = (abs(hs_dx) - (8 * chainlinks.Count())) / (chainlinks.Count() + 1);
         dist_by = (abs(hs_dy) - (8 * chainlinks.Count())) / (chainlinks.Count() + 1);
         hs_w = 8;
         if (hs_dx < 0)
         {
            dist_bx = 0 - dist_bx;
            hs_w = -8;
         }
         if (hs_dy < 0)
         {
            dist_by = 0 - dist_by;
            hs_w = -8;
         }
         if (Link.getDir() >
               down)                             //chain is moving horizontally
         {
            if (abs(hs_dx) - (8 * chainlinks.Count()) > 0)      //chain is stretched
            {
               for (int counter = 0; counter < chainlinks.Count(); counter++)
                  chainlinks.spr(counter)->x = hs_startx + hs_w + dist_bx + (counter *
                                               (hs_w + dist_bx));
            }
            else
            {
               if (abs(hs_x - hs_startx) <= hs_xdist - 8)
               {
                  hs_xdist = abs(hs_x - hs_startx);
                  if (pull_link == false)
                     chainlinks.del(chainlinks.idLast(wHSChain));

                  else
                     chainlinks.del(chainlinks.idFirst(wHSChain));
               }
            }
         }                                                     //chain is moving vertically
         else
         {
            if (abs(hs_dy) - (8 * chainlinks.Count()) > 0)      //chain is stretched
            {
               for (int counter = 0; counter < chainlinks.Count(); counter++)
                  chainlinks.spr(counter)->y = hs_starty + hs_w + dist_by + (counter *
                                               (hs_w + dist_by));
            }
            else
            {
               if (abs(hs_y - hs_starty) <= hs_ydist - 8)
               {
                  hs_ydist = abs(hs_y - hs_starty);
                  if (pull_link == false)
                     chainlinks.del(chainlinks.idLast(wHSChain));

                  else
                     chainlinks.del(chainlinks.idFirst(wHSChain));
               }
            }
         }
      }
   }
}

void game_loop(void)
{
   if (fadeclk >= 0)
   {
      if (fadeclk == 0 && currscr < 128)
         blockpath = false;
      --fadeclk;
   }

   animate_combos();
   mblock2.animate(0);
   items.animate();
   items.check_conveyor();
   guys.animate();
   roaming_item();
   dragging_item();
   Ewpns.animate();
   checklink = true;
   for (int i = 0; i < 1; i++)
   {
      if (Link.animate(0))
      {
         if (!zc_state)
            zc_state = ZC_GAMEOVER;
         return;
      }
      checklink = false;
   }
   do_magic_casting();
   Lwpns.animate();
   decorations.animate();
   particles.animate();
   update_hookshot();
   if (conveyclk <= 0)
      conveyclk = 3;
   --conveyclk;
   check_collisions();
   dryuplake();
   cycle_palette();
   nosecretsounds = tmpscr->flags3 & fNOSECRETSOUND;
   draw_screen(tmpscr, 0, 0);

   if (linkedmsgclk == 1)
   {
      //4 is for iwMore
      if (wpnsbuf[iwMore].tile != 0)
         putweapon(framebuf, zinit.msg_more_x, zinit.msg_more_y + 56, wPhantom, 4, up,
                   lens_hint_weapon[wPhantom][0], lens_hint_weapon[wPhantom][1]);
   }

   putintro();

   if (dmapmsgclk > 0)
   {
      Link.Freeze();
      if (dmapmsgclk <= 50)
         --dmapmsgclk;
   }
   if (dmapmsgclk == 1)
   {
      if (!tmpscr[currscr >= 128 ? 1 : 0].str)
      {
         //these are to cancel out any keys that Link may
         //be pressing so he doesn't attack at the end of
         //a message if he was scrolling through it quickly.
         rAbtn();
         rBbtn();

         Link.unfreeze();
      }
      dmapmsgclk = 0;
      clear_bitmap(msgdisplaybuf);
      anymsg = false;
   }

   if (!dmapmsgclk)
      putmsg();

   domoney();
   domagic();

   if (lensclk)
   {
      draw_lens_over();
      --lensclk;
   }
}

void free_bitmap_buffers(void)
{
   if (framebuf)
      destroy_bitmap(framebuf);
   if (scrollbuf)
      destroy_bitmap(scrollbuf);
   if (tempbuf)
      destroy_bitmap(tempbuf);
   if (msgdisplaybuf)
      destroy_bitmap(msgdisplaybuf);
   if (pricesdisplaybuf)
      destroy_bitmap(pricesdisplaybuf);
}

int alloc_bitmap_buffers(void)
{
   bool success = true;

   framebuf = NULL; scrollbuf = NULL; tempbuf = NULL; msgdisplaybuf = NULL;
   pricesdisplaybuf = NULL;

   if(!(framebuf = create_bitmap(256, 224)))
      RETURN_ERROR;
   
   if(!(scrollbuf = create_bitmap(512, 406)))
      RETURN_ERROR;

   if(!(tempbuf = create_bitmap(256, 224)))
      RETURN_ERROR;

   if(!(msgdisplaybuf = create_bitmap(256, 168)))
      RETURN_ERROR;

   if(!(pricesdisplaybuf = create_bitmap(256, 168)))
      RETURN_ERROR;

   clear_bitmap(scrollbuf);
   clear_bitmap(framebuf);
   clear_bitmap(msgdisplaybuf);
   clear_bitmap(pricesdisplaybuf);
   
error:
   if (!success)
      zc_error("Error allocating bitmap buffers memory.");

   return success;
}

bool zc_init(const char *qpath)
{
   bool success = true;
   int res;
   char temp[MAX_STRLEN];
   
   zc_message("Zelda Classic %s (Build %d)", VerStr(ZELDA_VERSION), VERSION_BUILD);
   zc_message("Armageddon Games web site: http://www.armageddongames.com");
   zc_message("Zelda Classic web site: http://www.zeldaclassic.com");

   /* Allocate buffers needed to load the qst file data. */
   if (!alloc_qst_buffers())
      RETURN_ERROR;

   resolve_password(datapwd);
   packfile_password(datapwd);
   
   sprintf(temp, "%s%c%s", system_path, OTHER_PATH_SEPARATOR, SYSTEM_FILE);
   
   if ((data = load_datafile(temp)) == NULL)
      RETURN_ERROR_M("Error loading " SYSTEM_FILE " system datafile.");
   
   sprintf(temp, "zcdata.dat %s Build %d", VerStr(ZCDAT_VERSION), ZCDAT_BUILD);
   
   if (strncmp((char *)data[_SIGNATURE].dat, temp, 24))
      RETURN_ERROR_M("Not a valid " SYSTEM_FILE " file.");

   /* Load the qst file and confirm it is valid. */
   if ((res = loadquest(qpath, &QHeader, &QMisc, tunes + MUSIC_COUNT)))
   {
      zc_error("Error loading quest. %s.", qst_error[res]);
      RETURN_ERROR;
   }

   packfile_password(NULL);

   /* Setting up base assets */
   sfxdata = (DATAFILE *)data[SFX].dat;
   mididata = (DATAFILE *)data[MUSIC].dat;
   font = (FONT *)data[FONT_GUI].dat;
   zfont = (FONT *)data[FONT_ZELDA].dat;
   zc_palette = RAMpal;

   if (!alloc_bitmap_buffers())
      RETURN_ERROR;
   
   if (!zc_initsound())
      RETURN_ERROR;

   /* keep the qst filename to use it later on save file */
   strcpy(qst_name, get_filename(qpath));
   
   if (load_savedgames() != 0)
      RETURN_ERROR_M("Error loading saved games.");

error:
   if (!success)
      zc_deinit();

   return success;
}

void zc_gameloop(void *arg)
{
   /* make the main thread wait
    * until one frame is done */
   slock_lock(mutex);
   
   // play the game
   while (zc_state != ZC_EXIT)
   {
      titlescreen();

      while (!zc_state)
      {
         game_loop();
         advanceframe();
      }

      // save subscreen status
      oldflags3 = tmpscr->flags3;
      tmpscr->flags3 = 0;
      is_playing = false;

      switch (zc_state)
      {
         case ZC_QUIT:       zc_quit();     break;
         case ZC_GAMEOVER:   zc_gameover(); break;
         case ZC_WON:        zc_ending();   break;
      }

      if (zc_state != ZC_RESUME)
      {
         kill_sfx();
         music_stop();
      }
   }
}

void zc_deinit(void)
{
   zc_deinitsound();
   free_savedgames();
   if (data)
   {
      unload_datafile(data);
      data = NULL;
   }
   free_bitmap_buffers();
   free_qst_buffers();
   reset_midis(tunes + MUSIC_COUNT); /* free midi memory. */
}

/*** end of zelda.cpp ***/
