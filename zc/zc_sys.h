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

bool zc_initsound(void);
void zc_deinitsound(void);
void draw_lens_under(void);
void draw_lens_over(void);
void advanceframe(void);
void update_video_frame(void);

void eat_buttons(void);

bool Up(void);
bool Down(void);
bool Left(void);
bool Right(void);
bool DrunkUp(void);
bool DrunkDown(void);
bool DrunkLeft(void);
bool DrunkRight(void);

bool rUp(void);
bool rDown(void);
bool rLeft(void);
bool rRight(void);

bool cAbtn(void);
bool cBbtn(void);
bool cEbtn(void);
bool cSbtn(void);
bool cLbtn(void);
bool cRbtn(void);
bool cMbtn(void);
bool cCbtn(void);
bool DrunkcBbtn(void);

bool rAbtn(void);
bool rBbtn(void);
bool rEbtn(void);
bool rSbtn(void);
bool rLbtn(void);
bool rRbtn(void);
bool rMbtn(void);
bool rCbtn(void);
bool DrunkrAbtn(void);
bool DrunkrBbtn(void);

void zapout(void);
void zapin(void);
void wavyout(void);
void wavyin(void);
void blackscr(int fcnt, bool showsubscr);
void black_opening(BITMAP *dest, int x, int y, int a, int max_a);
void close_black_opening(int x, int y, bool wait);
void open_black_opening(int x, int y, bool wait);
void openscreen(void);
int TriforceCount(void);

bool item_disabled(int item_type, int item);
bool can_use_item(int item_type, int item);
bool has_item(int item_type, int item);
int high_item(int jmax, int item_type, bool consecutive, int itemcluster,
              bool usecluster);
int current_item(int item_type, bool consecutive);
int high_flag(int i, int item_type, bool consecutive);
int item_tile_mod(void);

void jukebox(int index);
void play_DmapMusic(void);
void music_pause(void);
void music_resume(void);
void music_stop(void);
void sfx_cleanup(void);
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
void pause_all_sfx(void);
void resume_all_sfx(void);
void stop_sfx(int index);
void kill_sfx(void);
int  pan(int x);

char *time_str(uint32_t time);

int  vbound(int x, int low, int high);
float vbound(float x, float low, float high);
bool isinRect(int x, int y, int rx1, int ry1, int rx2, int ry2);

int encode_file_007(const char *srcfile, const char *destfile, unsigned int key,
                    const char *header, int method);
int decode_file_007(const char *srcfile, const char *destfile,
                    const char *header, int method, bool packed);

int  get_bit(uint8_t *bitstr, int bit);
void set_bit(uint8_t *bitstr, int bit, uint8_t val);

int anim_3_4(int clk, int speed);
#endif                                                      // _ZC_SYS_H_
