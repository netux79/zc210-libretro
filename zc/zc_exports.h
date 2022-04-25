#ifndef _ZC_EXPORTS_H_
#define _ZC_EXPORTS_H_

/* hack to avoid conflict with Windows BITMAP typedef */
#define BITMAP WINDOWS_BITMAP
#include <rthreads/rthreads.h>
#undef BITMAP

#define TIMING_FPS   60.0
#define MAX_STRLEN   1024

/* mapping of ZC logging functions to libretro calls */
#define zc_error(...)      zc_log(true, __VA_ARGS__)
#define zc_message(...)    zc_log(false, __VA_ARGS__)

/* game states */
enum {ZC_RUN, ZC_QUIT, ZC_RESET, ZC_EXIT, ZC_GAMEOVER, ZC_CONTINUE, ZC_WON, ZC_RESUME, ZC_PAUSE};

extern BITMAP *zc_canvas;
extern RGB *zc_palette;
extern bool zc_sync_pal;

extern char *save_path;
extern char *system_path;

/* settings */
extern float sampling_rate;
extern int mix_quality;
extern int master_vol;
extern int sfx_vol;
extern int music_vol;
extern int pan_style;
extern bool trans_layers;
extern bool allow_cheats;
extern bool heart_beep;
extern char sfx_file[16];
extern char sf2_file[16];

/* input keys */
extern int Akey, Bkey, Ekey, Skey, Lkey, Rkey, Mkey, Ckey;
extern int DUkey, DDkey, DLkey, DRkey;

/* thread stuff */
extern slock_t *mutex;
extern scond_t *cond;
extern int zc_state;

/* ZC methods */
bool zc_init(const char *qpath);
void zc_deinit(void);
void zc_gameloop(void *arg);
void zc_action(int state);
void update_music_volume(void);
void update_sfx_volume(void);

/* Libretro methods */
void zc_log(bool err, const char *format, ...);

#endif
