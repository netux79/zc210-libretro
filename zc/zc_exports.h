#ifndef _ZC_EXPORTS_H_
#define _ZC_EXPORTS_H_

#include <rthreads/rthreads.h>

#define TIMING_FPS   60.0
#define MAX_STRLEN   1024

/* mapping of ZC logging functions to libretro calls */
#define zc_error(...)      zc_log(true, __VA_ARGS__)
#define zc_message(...)    zc_log(false, __VA_ARGS__)

/* game states */
enum {qRUN, qQUIT, qRESET, qEXIT, qGAMEOVER, qCONT, qWON, qRESUME};

extern BITMAP *zc_canvas;
extern RGB *zc_palette;

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
extern bool heart_beep;

/* input keys */
extern int Akey, Bkey, Ekey, Skey, Lkey, Rkey, Mkey;
extern int DUkey, DDkey, DLkey, DRkey;

/* thread stuff */
extern slock_t *mutex;
extern scond_t *cond;
extern int zc_state;

/* ZC methods */
bool zc_init(const char *qpath);
void zc_deinit(void);
void zc_gameloop(void *arg);

/* Libretro methods */
void set_alpalette(RGB *p);
void zc_log(bool err, const char *format, ...);

#endif
