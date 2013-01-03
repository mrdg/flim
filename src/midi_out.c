#include "midi_out.h"

#include <pthread.h>

#define NOTE_ON  0x90
#define NOTE_OFF 0x80

struct midi_out {
    PmError error;
    PmStream *stream;
    PmDeviceID device;
    pthread_mutex_t lock;
};

void handle_error(PmError error, char *message)
{
    if (error != pmNoError) {
        printf(message, Pm_GetErrorText(error));
        Pm_Terminate();
        exit(1);
    }
}

struct midi_out *create_midi_out()
{
    struct midi_out *output = malloc(sizeof(struct midi_out));
    assert(output != NULL);

    output->error = Pm_Initialize();
    handle_error(output->error, "Error initializing portmidi: %s\n");

    output->device = Pm_GetDefaultOutputDeviceID();

    output->error = Pm_OpenOutput(&output->stream, output->device,
                                  NULL, 1024, NULL, NULL, 1);
    handle_error(output->error, "Error opening stream %s \n");

    pthread_mutex_init(&output->lock, NULL);

    return output;
}

void flm_midi_note_on(struct midi_out *output, int pitch, int velocity)
{
    PmEvent event[1];
    event[0].timestamp = Pt_Time(NULL);
    event[0].message   = Pm_Message(NOTE_ON, pitch, velocity);

    pthread_mutex_lock(&output->lock);
    output->error = Pm_Write(output->stream, event, 1);
    pthread_mutex_unlock(&output->lock);

    handle_error(output->error, "Error writing to stream: %s\n");
}

void flm_midi_note_off(struct midi_out *output, int pitch, int velocity)
{
    PmEvent event[1];
    event[0].timestamp = Pt_Time(NULL);
    event[0].message   = Pm_Message(NOTE_OFF, pitch, 0);

    pthread_mutex_lock(&output->lock);
    output->error = Pm_Write(output->stream, event, 1);
    pthread_mutex_unlock(&output->lock);
    handle_error(output->error, "Error writing to stream: %s\n");
}

void destroy_midi_out(struct midi_out *output) 
{

    output->error = Pm_Close(output->stream);
    handle_error(output->error, "Error closing stream: %s \n");
   
    Pm_Terminate();
    free(output);
}
