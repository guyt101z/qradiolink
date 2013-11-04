#ifndef DTMFDECODER_H
#define DTMFDECODER_H

#define INT16_MAX 0x7FFF
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MAX (2147483647)
#define INT32_MIN (-2147483647 - 1)
#define UINT16_MAX 65535
#define UINT8_MAX 255


#include <QObject>
#include <QtDebug>
#include "audiointerface.h"
#include "dtmf.h"

class DtmfDecoder : public QObject
{
    Q_OBJECT
public:
    explicit DtmfDecoder(QObject *parent = 0);
    ~DtmfDecoder();
    void stop();
    
signals:
    void finished();
    
public slots:
    void run();

private:
    bool _stop;
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
    float goertzel_magnitude(float* data, int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE );
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
    float _dtmf_frequencies[8];
    QVector<char> *_dtmf_sequence;
    char _current_letter;
    const char _digit_map[4][4]=
        {
        { '1', '2', '3', 'A' },
        { '4', '5', '6', 'B' },
        { '7', '8', '9', 'C' },
        { '*', '0', '#', 'D' },
        };
};

#endif // DTMFDECODER_H
