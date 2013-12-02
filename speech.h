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

#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>
#include "audiointerface.h"
//#include <espeak/speak_lib.h>
#include <festival/festival.h>

class Speech : public QObject
{
    Q_OBJECT
public:
    explicit Speech(QObject *parent = 0);
    void fspeak(char* text);
    /*
    void speak(char* text);
    void speakChar(char c);
    */
    AudioInterface *_audio;
    
signals:
    
public slots:

private:

    
};
//int SynthCallback(short *wav, int numsamples, espeak_EVENT *events);

#endif // SPEECH_H
