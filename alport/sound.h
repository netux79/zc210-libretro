#ifndef ALPORT_SOUND_H
#define ALPORT_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct SAMPLE
{
   int bits;                           /* 8 or 16 */
   int stereo;                         /* sample type flag */
   int freq;                           /* sample frequency */
   unsigned long len;                  /* length (in samples) */
   unsigned long loop_start;           /* loop start position */
   unsigned long loop_end;             /* loop finish position */
   void *data;                         /* sample data */
} SAMPLE;


#define PLAYMODE_PLAY      0
#define PLAYMODE_LOOP      1
#define PLAYMODE_FORWARD   0

#define MIXER_MAX_SFX      64


void *load_sample_object(PACKFILE *f, long size);
void unload_sample(SAMPLE *s);
SAMPLE *create_sample(int bits, int stereo, int freq, int len);
void destroy_sample(SAMPLE *spl);
int mixer_get_quality(void);
int mixer_get_frequency(void);
int mixer_get_voices(void);
int mixer_get_buffer_length(void);
int mixer_get_volume();
int mixer_init(int bufsize, int freq, int quality, int voices);
void mixer_exit(void);
void mixer_mix(signed short *buf);
void mixer_set_volume(int volume);
int allocate_voice(const SAMPLE *spl);
void deallocate_voice(int voice);
void reallocate_voice(int voice, const SAMPLE *spl);
void release_voice(int voice);
int voice_get_position(int voice);
SAMPLE *voice_check(int voice);
int voice_get_volume(int voice);
int voice_get_pan(int voice);
void voice_set_volume(int voice, int volume);
void voice_set_playmode(int voice, int playmode);
void voice_set_position(int voice, int position);
void voice_set_pan(int voice, int pan);
void voice_start(int voice);
void voice_stop(int voice);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_PALETTE_H */
