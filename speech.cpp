// Written by Adrian Musceac YO8RZZ at gmail dot com, started October 2013.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "speech.h"

Speech::Speech(QObject *parent) :
    QObject(parent)
{
    //_audio = new AudioInterface;
    int heap_size = 210000;
    int load_init_files = 1;
    festival_initialize(load_init_files,heap_size);
}

void Speech::fspeak(char* text)
{

    festival_say_text(text);
    festival_wait_for_spooler();
}

/*


static Speech* static_instance = NULL;

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events)
{
    if(wav==NULL)
        return 0;

    static_instance->_audio->write_short(wav,numsamples);
    return 0;
}




void Speech::speak(char* text)
{
    static_instance = this;
    int (*fp)(short*,int,espeak_EVENT*);
    fp = SynthCallback;
    espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS,512,NULL,0);
    const char *langNativeString = "ro";
    espeak_VOICE voice;
    voice.languages = langNativeString;
    voice.name = "romanian";
    voice.variant = 2;
    voice.gender = 0;
    espeak_SetVoiceByProperties(&voice);

    espeak_SetParameter(espeakWORDGAP,20,0);
    espeak_SetParameter(espeakRATE, 120, 0);
    espeak_SetParameter(espeakVOLUME, 100, 0);
    espeak_SetParameter(espeakRANGE, 50, 0);
    espeak_SetSynthCallback( SynthCallback);
    char * pbu = new char[120];
    espeak_Synth(text,
                 1024,
        0,
        POS_CHARACTER,
        0,
        espeakCHARS_AUTO,
        NULL,
        pbu);
    //espeak_Synchronize();
    espeak_Terminate();
}

void Speech::speakChar(char c)
{
    int (*fp)(short*,int,espeak_EVENT*);
    fp = SynthCallback;
    espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS,512,NULL,0);
    espeak_SetParameter(espeakRATE, 40, 0);
    espeak_SetParameter(espeakVOLUME, 100, 0);
    espeak_SetParameter(espeakRANGE, 100, 0);
    espeak_SetSynthCallback( SynthCallback);

    espeak_Char(c);
    espeak_Terminate();
}

*/
