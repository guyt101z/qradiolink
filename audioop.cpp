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


#include "audioop.h"
#include <cstdlib>

AudioOp::AudioOp(QObject *parent) :
    QObject(parent)
{

}


void AudioOp::stop()
{
    _stop=true;
}


void AudioOp::run()
{
    int audiobuffer_size = 640; //40 ms
    double treshhold = -15;
    double hyst = 0.2;
    bool treshhold_set = false;
    bool hyst_active = false;
    int hyst_counter = 0;
    _audio = new AudioInterface;
    while(true)
    {
        usleep(10000);
        short audiobuffer[audiobuffer_size];
        _audio->read_short(audiobuffer,audiobuffer_size);

        float sum=1.0;
        short max = 0;
        for(int j=0;j< audiobuffer_size;j++)
        {
            sum += static_cast<float>(audiobuffer[j]*audiobuffer[j]);
            max = (max > abs(audiobuffer[j])) ? max : audiobuffer[j];
        }

        float rms = sqrt(sum/(static_cast<float>(audiobuffer_size)));
        double power = 20*log10(rms/32768.0f);
        if(!treshhold_set)
        {
            treshhold = power;
            treshhold_set = true;
        }

        if((power > treshhold+hyst))
        {
            if(!hyst_active)
            {
                hyst -= 5.0;
                hyst_active = true;
            }
            else
            {
                hyst_counter++;
            }

            emit audioData(audiobuffer,audiobuffer_size);
        }

        if((hyst_active) && (hyst_counter> 50)) // 2 second drop-down period
        {
            hyst +=5.0;
            hyst_active = false;
            hyst_counter = 0;
        }

        QCoreApplication::processEvents();
        if(_stop)
            break;
    }
    emit finished();
}

void AudioOp::pcmAudio(short *pcm, short samples)
{
    _audio->write_short(pcm,samples*sizeof(short));
    delete[] pcm;
}
