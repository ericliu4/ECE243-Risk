#include "../resources/audio.h"
#include "structs.h"

//set up audio
struct audio_t *const audiop = (struct audio_t *)0xff203040;



void playVictory(){
    int count = 0;

    audiop->control = 0x8; // clear the output FIFOs
    audiop->control = 0x0; // resume input conversion
    while (count < 11352)
    {
        // output data if there is space in the output FIFOs
        if ((audiop->wsrc != 0) && (audiop->wslc != 0))
        {
            audiop->ldata = victory[count];
            audiop->rdata = victory[count++];
        }
    }
}

void playWompWomp(){
    int count = 0;

    audiop->control = 0x8; // clear the output FIFOs
    audiop->control = 0x0; // resume input conversion
    while (count < 28000)
    {
        // output data if there is space in the output FIFOs
        if ((audiop->wsrc != 0) && (audiop->wslc != 0))
        {
            audiop->ldata = womp_womp[count];
            audiop->rdata = womp_womp[count++];
        }
    }

}


void audio_playback_mono(int *samples, int n) {
    int count = 0;

    audiop->control = 0x8; // clear the output FIFOs
    audiop->control = 0x0; // resume input conversion
    while (count < 10000)
    {
        // output data if there is space in the output FIFOs
        if ((audiop->wsrc != 0) && (audiop->wslc != 0))
        {
            audiop->ldata = samples[count];
            audiop->rdata = samples[count++];
        }
    }
}