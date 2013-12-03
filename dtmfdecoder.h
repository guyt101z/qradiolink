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

#ifndef DTMFDECODER_H
#define DTMFDECODER_H


#include <QObject>
#include <QtDebug>
#include <QCoreApplication>
#include <QDateTime>
#include "audiointerface.h"
#include "speech.h"
#include "config_defines.h"

class DtmfDecoder : public QObject
{
    Q_OBJECT
public:
    explicit DtmfDecoder(QObject *parent = 0);
    ~DtmfDecoder();
    void stop();
    void process(bool p);
    
signals:
    void finished();
    void haveCall(QVector<char>*call);
    void haveCommand(QVector<char>*command);
    
public slots:
    void run();
    void resetInput();

private:
    bool _stop;
    /**
     * @brief makeTone
     * @param samplerate
     * @param frequency
     * @param length
     * @param gain
     * @return
     */
    float *makeTone(int samplerate, float frequency, int length, float gain=1.0);
    /**
     * @brief goertzel
     * @param x
     * @param N
     * @param frequency
     * @param samplerate
     * @return
     */
    float goertzel(float *x, int N, float frequency, int samplerate);
    /**
     * @brief goertzel_magnitude
     * @param data
     * @param numSamples
     * @param TARGET_FREQUENCY
     * @param SAMPLING_RATE
     * @return
     */
    float goertzel_magnitude(float* data, int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE );
    /**
     * @brief power
     * @param value
     * @return
     */
    float power(float value);

    /**
     * @brief decoding DTMF audio tones
     * @param buf
     * @param BUFSIZE
     * @param SAMP_RATE
     * @param treshhold_audio_power
     * @return
     */
    char decode(float *buf, int buffer_size, int samp_rate, float treshhold_audio_power, float tone_difference);

    /**
     * @brief Statistical analysis of char buffer
     */
    void analyse(int analysis_buffer);
    float _dtmf_frequencies[9];
    QVector<char> *_dtmf_sequence;
    QVector<char> *_dtmf_command;
    char _current_letter;
    char _previous_letter;
    bool _processing;
    bool _receiving;
    const char _digit_map[4][4]=
        {
        { '1', '2', '3', 'A' },
        { '4', '5', '6', 'B' },
        { '7', '8', '9', 'C' },
        { '*', '0', '#', 'D' },
        };
};

#endif // DTMFDECODER_H
