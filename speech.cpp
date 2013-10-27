#include "speech.h"

Speech::Speech(QObject *parent) :
    QObject(parent)
{
}

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events)
{
  return 0;
}

void Speech::speak(char* text)
{
  int (*fp)(short*,int,espeak_EVENT*);
  fp = SynthCallback;
  espeak_Initialize(AUDIO_OUTPUT_SYNCH_PLAYBACK,1024,NULL,0);

  espeak_SetSynthCallback( SynthCallback);
  char * pbu = new char[120];
  espeak_Synth(text,
    sizeof(text),
    0,
    POS_CHARACTER,
    0,
    espeakCHARS_AUTO,
    NULL,
    pbu);
  espeak_Terminate();
}
