//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zc_sys.h
//
//  System functions, input handlers, GUI stuff, etc.
//  for Zelda Classic.
//
//--------------------------------------------------------

#ifndef _ZC_SYS_H_
#define _ZC_SYS_H_

#include "zdefs.h"

void Z_init_sound();
void draw_lens_under();
void draw_lens_over();
void f_Quit(int type);
void advanceframe();
void updatescr();
void syskeys();

void eat_buttons();

bool Up();
bool Down();
bool Left();
bool Right();
bool DrunkUp();
bool DrunkDown();
bool DrunkLeft();
bool DrunkRight();

bool rUp();
bool rDown();
bool rLeft();
bool rRight();
bool DrunkrUp();
bool DrunkrDown();
bool DrunkrLeft();
bool DrunkrRight();

bool cAbtn();
bool cBbtn();
bool cEbtn();
bool cSbtn();
bool cLbtn();
bool cRbtn();
bool cMbtn();
bool DrunkcAbtn();
bool DrunkcBbtn();
bool DrunkcSbtn();
bool DrunkcLbtn();
bool DrunkcRbtn();
bool DrunkcMbtn();

bool rAbtn();
bool rBbtn();
bool rEbtn();
bool rSbtn();
bool rLbtn();
bool rRbtn();
bool rMbtn();
bool DrunkrAbtn();
bool DrunkrBbtn();
bool DrunkrSbtn();
bool DrunkrLbtn();
bool DrunkrRbtn();
bool DrunkrMbtn();

void zapout();
void zapin();
void wavyout();
void wavyin();
void blackscr(int fcnt, bool showsubscr);
void black_opening(BITMAP *dest, int x, int y, int a, int max_a);
void close_black_opening(int x, int y, bool wait);
void open_black_opening(int x, int y, bool wait);
void openscreen();
int  TriforceCount();

bool item_disabled(int item_type, int item);
bool can_use_item(int item_type, int item);
bool has_item(int item_type, int item);
int high_item(int jmax, int item_type, bool consecutive, int itemcluster,
              bool usecluster);
int current_item(int item_type, bool consecutive);
int high_flag(int i, int item_type, bool consecutive);
int item_tile_mod();
int dmap_tile_mod();

void jukebox(int index);
void jukebox(int index, int loop);
void play_DmapMusic();
void music_pause();
void music_resume();
void music_stop();
void master_volume(int dv, int mv);
void sfx_cleanup();
bool sfx_init(int index);
void sfx(int index, int pan, bool loop);

inline void sfx(int index)
{
   sfx(index, 128, false);
}
inline void sfx(int index, int pan)
{
   sfx(index, pan, false);
}

void cont_sfx(int index);
void stop_sfx(int index);
void adjust_sfx(int index, int pan, bool loop);
void pause_sfx(int index);
void resume_sfx(int index);
void pause_all_sfx();
void resume_all_sfx();
void stop_sfx(int index);
void kill_sfx();
int  pan(int x);

char *time_str(dword time);

int  vbound(int x, int low, int high);
float vbound(float x, float low, float high);
bool isinRect(int x, int y, int rx1, int ry1, int rx2, int ry2);

extern char datapwd[8];
void resolve_password(char *pwd);

int encode_file_007(const char *srcfile, const char *destfile, unsigned int key,
                    const char *header, int method);
int decode_file_007(const char *srcfile, const char *destfile,
                    const char *header, int method, bool packed);

int  get_bit(byte *bitstr, int bit);
void set_bit(byte *bitstr, int bit, byte val);

void Z_error(const char *format, ...);
void Z_message(const char *format, ...);

int anim_3_4(int clk, int speed);
#endif                                                      // _ZC_SYS_H_
