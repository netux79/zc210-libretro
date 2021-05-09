#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <rthreads/rthreads.h>

#include "libretro.h"
#include "alport.h"
#include "zc/zcdata.h"

/* name of the system file required to run zelda classic */
#define SYSTEM_FILE "zcdata.dat"

#define TIMING_FPS   60.0
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

static char save_path[4096];
static char game_path[4096];
static char system_path[4096];
static bpp_t *framebuf;
static short *soundbuf;
static struct retro_log_callback logging;
static retro_log_printf_t log_cb;
static float sampling_rate = 44100.0f;
static bool enable_audio = true;
static int music_vol = 255;
static bpp_t lr_palette[PAL_SIZE];

static unsigned x_coord;
static unsigned y_coord;
static bool butA = FALSE, butB = FALSE, butX = FALSE, butY = FALSE;
static bool butL = FALSE, butR = FALSE, butR2 = FALSE, butL2 = FALSE;

static FONT *font;
static DATAFILE *systemdf;
static DATAFILE *mididf;
static DATAFILE *sfxdf;
static RGB *pal;
static BITMAP *canvas;
static const char *midi_name = NULL;
static const char *sfx_name = NULL;
static int midiId = 0;
static int sfxId = 0;

/* threads stuff*/
slock_t *mutex;
scond_t *cond;
sthread_t *video_thread;
static bool running = false;

static void extract_directory(char* buf, const char* path, size_t size)
{
   strncpy(buf, path, size - 1);
   buf[size - 1] = '\0';

   char* base = strrchr(buf, '/');

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
   /* Main libretro framebuffer */
   framebuf = (bpp_t *)calloc(SCR_WIDTH * SCR_HEIGHT, sizeof(bpp_t));
   /* working buffer for the allegro port library (8-bit depth) */
   canvas = create_bitmap(SCR_WIDTH, SCR_HEIGHT);
   
   mutex = slock_new();
   cond = scond_new();
}

void retro_deinit(void)
{
   scond_free(cond);
   slock_free(mutex);

   free(framebuf);
   framebuf = NULL;
   
   destroy_bitmap(canvas);
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
   info->valid_extensions = "dat";
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   info->timing.fps = TIMING_FPS,
   info->timing.sample_rate = sampling_rate,

   info->geometry.base_width = SCR_WIDTH;
   info->geometry.base_height = SCR_HEIGHT;
   info->geometry.max_width = SCR_WIDTH;
   info->geometry.max_height = SCR_HEIGHT;
   info->geometry.aspect_ratio = 4.0f / 3.0f;
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   static const struct retro_variable vars[] = {
      { "zc_musicvol", "Music Volume; 16|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15" },
      { "zc_samplerate", "Sample Rate (require restart); 22050|32000|44100" },
      { "zc_audio_enable", "Enable Audio; true|false" },
      { NULL, NULL },
   };

   cb(RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars);

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;

   static const struct retro_controller_description controllers[] = {
      { "Dummy Controller #1", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 0) },
      { "Dummy Controller #2", RETRO_DEVICE_SUBCLASS(RETRO_DEVICE_JOYPAD, 1) },
      { "Augmented Joypad", RETRO_DEVICE_JOYPAD }, // Test overriding generic description in UI.
   };

   static const struct retro_controller_info ports[] = {
      { controllers, 3 },
      { NULL, 0 },
   };

   cb(RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports);
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
   x_coord = 0;
   y_coord = 0;
}

static void update_input(void)
{
   int dir_x = 0;
   int dir_y = 0;

   input_poll_cb();
   
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))
      dir_y--;
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))
      dir_y++;
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))
      dir_x--;
   if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))
      dir_x++;
   
   /* A play selected sound, B stop sounds */
   butA = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A);
   butB = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B);
   
   /* X = play midi, Y = pause midi */
   butX = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X);
   butY = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y);
   
   /* L / R select midi music */
   butL = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L);
   butR = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R);

   /* L2 / R2 select sfx sound */
   butL2 = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2);
   butR2 = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2);

   x_coord = (x_coord + dir_x) & 31;
   y_coord = (y_coord + dir_y) & 31;
}

static void set_alpalette(RGB *p)
{
   int i;

#ifdef WANT_BPP32
   bpp_t *pa = (bpp_t *)p;
   for (i = 0; i < PAL_SIZE; i++)
       // Just need to shift it in order to make it 0-255 range (allegro palette is 0-63 range).
       lr_palette[i] = pa[i] << 2;
#else
   for (i = 0; i < PAL_SIZE; i++)
       lr_palette[i] = ((p[i].r & 0x1F) << 11) | ((p[i].g & 0x3F) << 5) | (p[i].b & 0x1F);
#endif
}

static void zc_gameloop(void *arg)
{
   while (running) 
   {
      slock_lock(mutex);

      // Paint the checkered screen into the canvas.
      for (unsigned y = 0; y < SCR_HEIGHT; y++)
      {
         unsigned index_y = ((y - y_coord) >> 4) & 1;
         for (unsigned x = 0; x < SCR_WIDTH; x++)
         {
            unsigned index_x = ((x - x_coord) >> 4) & 1;
            *(canvas->line[y] + x) = (index_y ^ index_x) ? 14 : 1;
         }
      }
      // Write MIDI name
      textout_ex(canvas, font, midi_name, 50, 50, 1, 3);
      // Write SFX name
      textout_ex(canvas, font, sfx_name, 50, 60, 1, 3);

      scond_wait(cond, mutex);
      slock_unlock(mutex);
   }
}

static void render_checkered(void)
{
   /* Try rendering straight into VRAM if we can. */
   bpp_t *buf = NULL;
   unsigned stride = 0;
   struct retro_framebuffer fb = {0};
   fb.width = SCR_WIDTH;
   fb.height = SCR_HEIGHT;
   fb.access_flags = RETRO_MEMORY_ACCESS_WRITE;
   if (environ_cb(RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER, &fb) && fb.format == RETRO_PIX_FORMAT)
   {
      buf = (bpp_t *)fb.data;
      stride = fb.pitch >> STRIDE_SHIFT;
   }
   else
   {
      buf = framebuf;
      stride = SCR_WIDTH;
   }
   
   // Blit the canvas into the libretro framebuffer
   unsigned char *canvp = (unsigned char *) canvas->dat;
   for (unsigned x = 0; x < SCR_WIDTH * SCR_HEIGHT; x++)
   {
      buf[x] =  lr_palette[canvp[x]];
   }

   video_cb(buf, SCR_WIDTH, SCR_HEIGHT, stride << STRIDE_SHIFT);
}

static void check_variables(bool firsttime = false)
{
   struct retro_variable var = {0};
   int old_mv = music_vol;

   var.key = "zc_musicvol";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      music_vol = atoi(var.value);
      if (music_vol > 0)
         music_vol = (music_vol << 4) - 1;
   }

   var.key = "zc_audio_enable";
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
   {
      enable_audio = !strcmp(var.value, "true") ? true : false;
      log_cb(RETRO_LOG_INFO, "Key -> Val: %s -> %s.\n", var.key, var.value);
   }

   if (firsttime)
   {
      /* For this we require restart */
      var.key = "zc_samplerate";
      if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value)
         sampling_rate = strtof(var.value, NULL);
   }
   else
   {
      /* Apply sound volume if changed.
       * we just want to exercise this on run-time after setup */
       if (music_vol != old_mv)
         midi_set_volume(music_vol);
   }
}

static void audio_callback(void)
{
   if (!enable_audio)
      return;

   /* process midi music */
   midi_fill_buffer();

   mixer_mix(soundbuf);
   
   audio_batch_cb(soundbuf, sampling_rate / TIMING_FPS);
}

static void play_sounds(void)
{
   static bool bA = FALSE, bX = FALSE, bY = FALSE;
   static bool bL = FALSE, bR = FALSE, bL2 = FALSE, bR2 = FALSE;
   static int oldMidiId = -1;
   static int voc;
   
   // play de sounds when a button is pressed
   if (bA != butA) {
      if (butA) {
         voc = allocate_voice((const SAMPLE *)sfxdf[sfxId].dat);
         voice_start(voc);
         release_voice(voc);
      }
      bA = butA;
   }

   if (bX != butX) {
      if (butX) midi_pause();
      bX = butX;
   }
   
   if (bY != butY) {
      if (butY) {
         if (oldMidiId != midiId) {
            midi_play(mididf[midiId].dat, TRUE);
            oldMidiId = midiId;
         } else {
            midi_resume();
         }
      }
      bY = butY;
   }
   
   if (bL != butL) {
      if (butL) {
         midiId--;
         if (midiId < 0) {
            midiId = MUSIC_COUNT - 1;
         }
         midi_name = get_datafile_property(&mididf[midiId], DAT_NAME);
      }
      bL = butL;
   }

   if (bR != butR) {
      if (butR) {
         midiId++;
         if (midiId >= MUSIC_COUNT) {
            midiId = 0;
         }
         midi_name = get_datafile_property(&mididf[midiId], DAT_NAME);
      }
      bR = butR;
   }
   
   if (bL2 != butL2) {
      if (butL2) {
         sfxId--;
         if (sfxId < 0) {
            sfxId = SFX_COUNT - 1;
         }
         sfx_name = get_datafile_property(&sfxdf[sfxId], DAT_NAME);
      }
      bL2 = butL2;
   }

   if (bR2 != butR2) {
      if (butR2) {
         sfxId++;
         if (sfxId >= SFX_COUNT) {
            sfxId = 0;
         }
         sfx_name = get_datafile_property(&sfxdf[sfxId], DAT_NAME);
      }
      bR2 = butR2;
   }   
}

void retro_run(void)
{
   bool updated = false;

   slock_lock(mutex);
   
   update_input();
   play_sounds();
   render_checkered();
   
   /* wake up core thread */
   slock_unlock(mutex);
   scond_signal(cond);
   
   audio_callback();

   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();
}

static bool load_system_file(const char * path)
{
   char sysfile[4096];
   
   /*  calculate the system file full path */
   sprintf(sysfile, "%s/%s", path, SYSTEM_FILE);
   
   /* Load the system datafile */
   packfile_password("longtan");
   systemdf = load_datafile(sysfile);
   if (!systemdf)
   {
      return false;
   }
   packfile_password(NULL);
   
   /* Set the palette */
   pal = (RGB *)systemdf[PAL_GUI].dat;
   /* Set the font */
   font = (FONT *)systemdf[FONT_GUI].dat;
   /* The MIDI list */
   mididf = (DATAFILE *)systemdf[MUSIC].dat;
   /* The SFX list */
   sfxdf = (DATAFILE *)systemdf[SFX].dat;
   
   /* Get the first MIDI name */
   midi_name = get_datafile_property(mididf, DAT_NAME);
   /* Get the first SFX name */
   sfx_name = get_datafile_property(sfxdf, DAT_NAME);

   /* Setup our libretro palette */
   set_alpalette(pal);
   
   return true;
}

bool retro_load_game(const struct retro_game_info *info)
{
   const char *dir = NULL;
   
   struct retro_input_descriptor desc[] = {
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B,     "B" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A,     "A" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X,     "Map" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y,     "Y" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L,     "L" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R,     "R" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2,    "L2" },
      { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2,    "R2" },
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
   if (environ_cb(RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &dir) && dir)
      strcpy(save_path, dir);
   else
      strcpy(save_path, game_path);

   if (environ_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &dir) && dir)
      strcpy(system_path, dir);
   else
      strcpy(system_path, game_path);
   
   check_variables(true);
   
   soundbuf = (short *)malloc(sampling_rate / TIMING_FPS * 2 * sizeof(short));
   
   /* Init mixer */
   if (!mixer_init(sampling_rate / TIMING_FPS, sampling_rate, 1, MIXER_MAX_SFX))
   {
      return false;
   }

   /* Setup the midi processor */
   if (!midi_init(sampling_rate, 1 / TIMING_FPS))
   {
      return false;
   }
   /* Apply music volume */
   midi_set_volume(music_vol);
   
   /* load system file zcdata.dat from system folder */
   load_system_file(system_path);
   
   /* Create a thread to generate 1-frame of video */
   running = true;
   video_thread = sthread_create(zc_gameloop, NULL);
   sthread_detach(video_thread);

   return true;
}

void retro_unload_game(void)
{
   /* stop gameloop thread from running */
   running = false;
   scond_signal(cond);

   midi_deinit();
   unload_datafile(systemdf);
   mixer_exit();
   free(soundbuf);
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
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

