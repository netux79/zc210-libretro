#ifndef ALPORT_MIDI_H
#define ALPORT_MIDI_H

#ifdef __cplusplus
extern "C" {
#endif


void destroy_midi(void *midi);
void *load_midi_object(PACKFILE *f, long size);
void midi_deinit(void);
void midi_fill_buffer(void);
float midi_get_length(void);
int midi_get_volume(void);
int midi_init(int rate, float delta);
int midi_isplaying(void);
void midi_pause(void);
int midi_play(void *midi, int loop);
void midi_resume(void);
void midi_seek(float offset);
void midi_set_volume(int volume);
void midi_stop(void);
float midi_time(void);
void *read_midi(PACKFILE *f);

#ifdef __cplusplus
}
#endif

#endif          /* ifndef ALPORT_MIDI_H */
