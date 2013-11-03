#include "dtmfdecoder.h"

#include <cmath>
#include <cstdlib>

using std::rand;
// math stuff
using std::cos;
using std::abs;
using std::exp;
using std::log10;


//#define BUFSIZE 4096
//#define SAMP_RATE 8000

DtmfDecoder::DtmfDecoder(QObject *parent) :
    QObject(parent)
{
    _stop=false;

}

DtmfDecoder::~DtmfDecoder()
{

}

void tone_detected(void *user_data, int code, int level, int delay)
{
    qDebug() << "callback";
    qDebug() << "tone_detected code:" << (char)code << " level:"<<level<<" delay:"<<delay;
}

void DtmfDecoder::stop()
{
    _stop = true;
}

void DtmfDecoder::run()
{

    int BUFSIZE = 256;
    int SAMP_RATE = 8000;
    float treshhold_audio_power = 10.0; // dB
    AudioInterface *audio= new AudioInterface(0,SAMP_RATE,1);
    // old detector
    //Dtmf *dtmf = new Dtmf(NULL,512);
    // spandsp library
    //dtmf_rx_state_t * dtmf = dtmf_rx_init(NULL, NULL, NULL);
    //dtmf_tx_state_t * dtmf_t = dtmf_tx_init(NULL);
    //dtmf_rx_set_realtime_callback(dtmf, tone_detected , NULL);

    _dtmf_frequencies[0]= 697.0;
    _dtmf_frequencies[1]= 770.0;
    _dtmf_frequencies[2]= 852.0;
    _dtmf_frequencies[3]= 941.0;
    _dtmf_frequencies[4]= 1209.0;
    _dtmf_frequencies[5]= 1336.0;
    _dtmf_frequencies[6]= 1477.0;
    _dtmf_frequencies[7]= 1633.0;
    while(true)
    {
        if(_stop)
            break;
        //short *buf =new short[BUFSIZE];
        float *buf = new float[BUFSIZE];

        audio->read(buf, BUFSIZE);
        buf = buf;
        int tones[2];
        tones[0] = 0.0;
        tones[1] = 0.0;
        float largest_tone_power = 0.0;
        for(int i =0;i<4;++i)
        {
            float tone_power = power(goertzel(buf, BUFSIZE, _dtmf_frequencies[i], SAMP_RATE));
            if(tone_power < largest_tone_power) continue;
            if(tone_power < treshhold_audio_power) continue;

            for(int k = 0;k<4;++k)
            {
                if(k==i)
                    continue;
                float other_dtmf_tone_power = power(goertzel(buf, BUFSIZE, _dtmf_frequencies[k], SAMP_RATE));
                if(tone_power > (other_dtmf_tone_power + 10))
                {
                    tones[0] = (int)_dtmf_frequencies[i];
                    largest_tone_power = tone_power;
                }
                else
                {
                    tones[0] = 0;
                }

            }



        }
        largest_tone_power =0.0;
        for(int i =4;i<8;++i)
        {
            float tone_power = power(goertzel(buf, BUFSIZE, _dtmf_frequencies[i], SAMP_RATE));
            if(tone_power < largest_tone_power) continue;
            if(tone_power < treshhold_audio_power) continue;

            for(int k = 4;k<7;++k)
            {
                if(k==i)
                    continue;
                float other_dtmf_tone_power = power(goertzel(buf, BUFSIZE, _dtmf_frequencies[k], SAMP_RATE));
                if(tone_power > (other_dtmf_tone_power + 10))
                {
                    tones[1] = (int)_dtmf_frequencies[i];
                    largest_tone_power = tone_power;
                }
                else
                {
                    tones[1] = 0;
                }

           }

        }

        char letter;
        switch(tones[0])
        {
        case 697:
            switch(tones[1])
            {
            case 1209:
                letter = '1';
                break;
            case 1336:
                letter = '2';
                break;
            case 1477:
                letter = '3';
                break;
            case 1633:
                letter = 'A';
                break;
            default:
                letter = ' ';
            }

            break;
        case 770:
            switch(tones[1])
            {
            case 1209:
                letter = '4';
                break;
            case 1336:
                letter = '5';
                break;
            case 1477:
                letter = '6';
                break;
            case 1633:
                letter = 'B';
                break;
            default:
                letter = ' ';
            }
            break;
        case 852:
            switch(tones[1])
            {
            case 1209:
                letter = '7';
                break;
            case 1336:
                letter = '8';
                break;
            case 1477:
                letter = '9';
                break;
            case 1633:
                letter = 'C';
                break;
            default:
                letter = ' ';
            }
            break;
        case 941:
            switch(tones[1])
            {
            case 1209:
                letter = '*';
                break;
            case 1336:
                letter = '0';
                break;
            case 1477:
                letter = '#';
                break;
            case 1633:
                letter = 'D';
                break;
            default:
                letter = ' ';
            }
            break;
        default:
            letter = ' ';
        }
        if(letter!=' ')
        qDebug() << QString(letter);

        //sleep(15);

        //float *buf = makeTone(44100,600,BUFSIZE,0.5);
        //audio->write(buf,BUFSIZE);
        /*
        dtmf_rx(dtmf, buf, BUFSIZE/2);
        char digitsbuf[255];
        digitsbuf[0] = '\0';
        int rc = dtmf_rx_get(dtmf, digitsbuf, sizeof(digitsbuf)-1);
        if (rc > 0) {
            printf("Rx digit: %s\n", digitsbuf);
        }
        */

        //char digits[50];
        //dtmf_rx_get(dtmf, digits, 50);


        /*
        char *dtmf_out = new char[3];
        dtmf_out[0] ='1';
        dtmf_out[1] ='2';
        dtmf_out[2] ='3';
        //dtmf_tx_set_timing 	( 	dtmf_t, 200,200 ) 	;
        dtmf_tx_put(dtmf_t,"123",-1);
        dtmf_tx(dtmf_t,buf,3);
        audio->write(buf,BUFSIZE*sizeof(int16_t));
        dtmf_tx_free(dtmf_t);
        */



        //dtmf->decode(buf);
        //qDebug() << digits;
        delete[] buf;
        //usleep(50);

    }
    //dtmf_rx_free(dtmf);
    finish:
    delete audio;
    //delete dtmf;
    emit finished();
}

/**
  Written by: Espen Riskedal, espenr@ii.uib.no, july-2002
  */
#define PI 3.14159265358979323844

/** Generates a tone of the specified frequency
*  Gotten from: http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&oe=UTF-8&safe=off&selm=3c641e%243jn%40uicsl.csl.uiuc.edu
*/
float *DtmfDecoder::makeTone(int samplerate, float frequency, int length, float gain) {
    //y(n) = 2 * cos(A) * y(n-1) - y(n-2)
    //A= (frequency of interest) * 2 * PI / (sampling frequency)
    //A is in radians.
    // frequency of interest MUST be <= 1/2 the sampling frequency.
    float *tone = new float[length];
    float A = frequency*2*PI/samplerate;

    for (int i=0; i<length; i++) {
    if (i > 1) tone[i]= 2*cos(A)*tone[i-1] - tone[i-2];
    else if (i > 0) tone[i] = 2*cos(A)*tone[i-1] - (cos(A));
    else tone[i] = 2*cos(A)*cos(A) - cos(2*A);
    }

    for (int i=0; i<length; i++) tone[i] = tone[i]*gain;

    return tone;
}

float DtmfDecoder::goertzel(float *x, int N, float frequency, int samplerate) {
    float Skn, Skn1, Skn2;
    Skn = Skn1 = Skn2 = 0;

    for (int i=0; i<N; i++) {
    Skn2 = Skn1;
    Skn1 = Skn;
    Skn = 2*cos(2*PI*frequency/samplerate)*Skn1 - Skn2 + x[i];
    }

    float WNk = exp(-2*PI*frequency/samplerate); // this one ignores complex stuff
    //float WNk = exp(-2*j*PI*k/N);
    return (Skn - WNk*Skn1);
}

float DtmfDecoder::power(float value) {
    return 20*log10(abs(value));
}
