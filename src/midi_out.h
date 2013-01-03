#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#ifndef __PORTTIME
#define __PORTTIME
#include "porttime.h"
#endif

#ifndef __PORTMIDI
#define __PORTMIDI
#include "portmidi.h"
#endif

struct note_data {
    int pitch;
    int velocity;
};

struct midi_out * create_midi_out();
void destroy_midi_out(struct midi_out *output);
int note(struct midi_out *output, int pitch, int duration, int velocity);

void flm_midi_note_on(struct midi_out *output, int pitch, int velocity);
void flm_midi_note_off(struct midi_out *output, int pitch, int velocity);

