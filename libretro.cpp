#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <memalign.h>
#include "libretro.h"
#include "alport.h"
#include "zc/zc_exports.h"

#define SCR_WIDTH    256
#define SCR_HEIGHT   224

//#define WANT_BPP32
#ifdef WANT_BPP32
typedef uint32_t bpp_t;
static unsigned STRIDE_SHIFT = 2;
static enum retro_pixel_format RETRO_PIX_FORMAT = RETRO_PIXEL_FORMAT_XRGB8888;
#define RETRO_PIX_NAME "XRGB8888"
#else
typedef uint16_t bpp_t;
static unsigned STRIDE_SHIFT = 1;
static enum retro_pixel_format RETRO_PIX_FORMAT = RETRO_PIXEL_FORMAT_RGB565;
#define RETRO_PIX_NAME "RGB565"
#endif

static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static retro_log_printf_t log_cb;

float sampling_rate;
char game_path[MAX_STRLEN];
char *save_path;
char *system_path;

/* threads stuff */
static sthread_t *zc_thread;
slock_t *mutex;
scond_t *cond;

static bpp_t lr_palette[PAL_SIZE];
static bpp_t *framebuf;
static short *soundbuf;

void zc_log(bool err, const char *format, ...)
{
   char buf[MAX_STRLEN];

   va_list ap;
   va_start(ap, format);
   vsprintf(buf, format, ap);
   va_end(ap);
   log_cb(err ? RETRO_LOG_ERROR : RETRO_LOG_INFO, "%s\n", buf);
}

static void extract_directory(char *buf, const char *path, size_t size)
{
   strncpy(buf, path, size - 1);
   buf[size - 1] = '\0';

   char *base = strrchr(buf, '/');

   if (base)
      *base = '\0';
   else
      strncpy(buf, ".", size);
}

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}

void retro_init(void)
{
   mutex = slock_new();
   cond = scond_new();
}

void retro_deinit(void)
{
   scond_free(cond);
   slock_free(mutex);
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = "Zelda Classic v2.10";
   info->library_version  = "Alpha 1";
   info->need_fullpath    = true;
   info->valid_extensions = "qst";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->timing.fps = TIMING_FPS;
   info->timing.sample_rate = sampling_rate;
   info->geometry.base_width = SCR_WIDTH;
   info->geometry.base_height = SCR_HEIGHT;
   info->geometry.max_width = SCR_WIDTH;
   info->geometry.max_height = SCR_HEIGHT;
   info->geometry.aspect_ratio = 4.0f / 3.0f;
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   static const struct retro_variable vars[] =
   {
      { "zc_samplerate", "Sample Rate (requires restart); 22050|32000|44100|48000" },
      { "zc_mix_quality", "Sound Quality (requires restart); Normal|High|Low" },
      { "zc_master_vol", "Master Volume; 16|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15" },
      { "zc_sfx_vol", "SFX Volume; 16|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15" },
      { "zc_music_vol", "Music Volume; 16|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15" },
      { "zc_pan_style", "Sound Pan Style; 1/2|3/4|Full|Mono" },
      { "zc_heart_beep", "Enable Low Health Beep; true|false" },
      { "zc_trans_layers", "Show Transparent Layers; true|false" },
      { NULL, NULL },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void *)vars);

   static struct retro_log_callback logging;

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

void retro_reset(void)
{
   zc_action(ZC_RESET);
}

static void update_input(void)
{
   input_poll_cb();

   DUkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP);
   DDkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN);
   DLkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT);
   DRkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT);

   Akey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
   Bkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
   Mkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X);
   Lkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
   Rkey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);
   Ekey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT);
   Skey = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START);
}

void update_palette(RGB *p)
{
   int i;

#ifdef WANT_BPP32
   bpp_t *pa = (bpp_t *)p;
   /* Just need to shift it in order to make it 0-255 range 
    * (allegro palette is 0-63 range). */
   for (i = 0; i < PAL_SIZE; i++)
      lr_palette[i] = pa[i] << 2;
#else
   for (i = 0; i < PAL_SIZE; i++)
      lr_palette[i] = ((p[i].r & 0x3E) << 10) | (p[i].g << 5) | (p[i].b >> 1);
#endif
}

static void render_video(void)
{
   /* Try rendering straight into VRAM if we can. */
   bpp_t *buf = NULL;
   unsigned int stride = 0;
   unsigned int i = 0;
   struct retro_framebuffer fb = {0};
   unsigned char *canvp = NULL;

   fb.width = SCR_WIDTH;
   fb.height = SCR_HEIGHT;
   fb.access_flags = RETRO_MEMORY_ACCESS_WRITE;
   if (environ_cb(RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER, &fb)
         && fb.format == RETRO_PIX_FORMAT)
   {
      buf = (bpp_t *)fb.data;
      stride = fb.pitch >> STRIDE_SHIFT;
   }
   else
   {
      buf = framebuf;
      stride = SCR_WIDTH;
   }

   /* update the Libretro palette if required */
   if (zc_sync_pal)
   {
      zc_sync_pal = false;
      update_palette(zc_palette);
   }

   /* Blit the ZC canvas into the libretro framebuffer */
   canvp = (unsigned char *) zc_canvas->dat;
   for (i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
      buf[i] =  lr_palette[canvp[i]];

   video_cb(buf, SCR_WIDTH, SCR_HEIGHT, stride << STRIDE_SHIFT);
}

static void check_variables(bool startup = false)
{
   struct retro_variable var = {0};
   int old_musicv = music_vol;
   int old_masterv = master_vol;
   int old_sfxv = sfx_vol;

   var.key = "zc_master_vol";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      master_vol = atoi(var.value);
      if (master_vol > 0)
         master_vol = (master_vol << 4) - 1;
   }

   var.key = "zc_sfx_vol";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      sfx_vol = atoi(var.value);
      if (sfx_vol > 0)
         sfx_vol = (sfx_vol << 4) - 1;
   }

   var.key = "zc_music_vol";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      music_vol = atoi(var.value);
      if (music_vol > 0)
         music_vol = (music_vol << 4) - 1;
   }

   var.key = "zc_pan_style";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      if (!strcmp(var.value, "Mono"))
         pan_style = 0;
      else if (!strcmp(var.value, "3/4"))
         pan_style = 2;
      else if (!strcmp(var.value, "Full"))
         pan_style = 3;
      else
         pan_style = 1; /* 1/2 */
   }

   var.key = "zc_heart_beep";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      heart_beep = !strcmp(var.value, "true") ? true : false;

   var.key = "zc_trans_layers";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      trans_layers = !strcmp(var.value, "true") ? true : false;

   if (startup)
   {
      /* For these we require restart */
      var.key = "zc_samplerate";
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
         sampling_rate = strtof(var.value, NULL);

      var.key = "zc_mix_quality";
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
      {
         if (!strcmp(var.value, "Low"))
            mix_quality = 0;
         else if (!strcmp(var.value, "High"))
            mix_quality = 2;
         else
            mix_quality = 1; /* Normal */
      }
   }
   else
   {
      /* Want to exercise these on run-time after setup only */
      /* Apply master volume */
      if (master_vol != old_masterv)
         mixer_set_volume(master_vol);

      /* Apply music volume if updated. */
      if (music_vol != old_musicv)
         update_music_volume();

      /* Apply sound volume if updated. */
      if (sfx_vol != old_sfxv)
         update_sfx_volume();
   }
}

static void render_audio(void)
{
   mixer_mix(soundbuf);
   audio_batch_cb(soundbuf, sampling_rate / TIMING_FPS);
}

void retro_run(void)
{
   bool updated = false;

   slock_lock(mutex);

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();

   update_input();

   /* wake up gameloop thread and
    * wait for it to run one
    * frame of audio and video */
   scond_signal(cond);
   scond_wait(cond, mutex);

   render_audio();
   render_video();

   slock_unlock(mutex);
}

bool retro_load_game(const struct retro_game_info *info)
{
   struct retro_input_descriptor desc[] =
   {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,   "Left"   },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,     "Up"     },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,   "Down"   },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT,  "Right"  },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,      "B"      },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,      "A"      },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,      "Map"    },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,      "L"      },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,      "R"      },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START,  "Start"  },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
      { 0 },
   };

   environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);

   enum retro_pixel_format fmt = RETRO_PIX_FORMAT;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, RETRO_PIX_NAME " is not supported.\n");
      return false;
   }

   extract_directory(game_path, info->path, sizeof(game_path));

   if (!environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &save_path)
         || !save_path)
   {
      log_cb(RETRO_LOG_INFO, "Defaulting save directory to %s.\n", game_path);
      save_path = game_path;
   }

   if (!environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &system_path)
         || !system_path)
   {
      log_cb(RETRO_LOG_INFO, "Defaulting system directory to %s.\n", game_path);
      system_path = game_path;
   }

   check_variables(true);

   /* Main libretro buffers, done after check_variables to respect settings */
   framebuf = (bpp_t *)calloc(SCR_WIDTH * SCR_HEIGHT, sizeof(bpp_t));
   soundbuf = (short *)memalign_alloc_aligned(sampling_rate / TIMING_FPS * 2 * 
                                              sizeof(short));

   /* init zelda classic engine */
   if (!zc_init(info->path))
      return false;

   /* Create a thread to generate 1-frame of video & audio */
   zc_state = ZC_RUN;
   zc_thread = sthread_create(zc_gameloop, NULL);

   return true;
}

void retro_unload_game(void)
{
   /* stop zc game loop thread 
    * from running */
   zc_state = ZC_EXIT;
   scond_signal(cond);
   sthread_join(zc_thread);

   zc_deinit();

   memalign_free(soundbuf);
   free(framebuf);
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info,
                             size_t num)
{
   return false;
}

size_t retro_serialize_size(void)
{
   return 0;
}

bool retro_serialize(void *data_, size_t size)
{
   return false;
}

bool retro_unserialize(const void *data_, size_t size)
{
   return false;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

