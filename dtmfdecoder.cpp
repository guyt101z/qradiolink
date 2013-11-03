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
    _dtmf_frequencies[0]= 697.0;
    _dtmf_frequencies[1]= 770.0;
    _dtmf_frequencies[2]= 852.0;
    _dtmf_frequencies[3]= 941.0;
    _dtmf_frequencies[4]= 1209.0;
    _dtmf_frequencies[5]= 1336.0;
    _dtmf_frequencies[6]= 1477.0;
    _dtmf_frequencies[7]= 1633.0;
    _dtmf_sequence = new QVector<char>;
    _current_letter = ' ';
}

DtmfDecoder::~DtmfDecoder()
{

}

void tone_detected(void *user_data, int code, int level, int delay)
{
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
    int analysis_buffer = 20;
    AudioInterface *audio= new AudioInterface(0,SAMP_RATE,1);
    // old detector
    //Dtmf *dtmf = new Dtmf(NULL,512);

    /* spandsp
    dtmf_rx_state_t * dtmf = dtmf_rx_init(NULL, NULL, NULL);
    dtmf_tx_state_t * dtmf_t = dtmf_tx_init(NULL);
    dtmf_rx_set_realtime_callback(dtmf, tone_detected , NULL);
    */


    while(true)
    {
        if(_stop)
            break;
        //short *buf =new short[BUFSIZE];
        float *buf = new float[BUFSIZE];

        audio->read(buf, BUFSIZE);

        char letter = decode(buf,BUFSIZE,SAMP_RATE, treshhold_audio_power);
        // fill a buffer of decoded letters
        if(_dtmf_sequence->size()>20)
        {
            _dtmf_sequence->remove(0);
        }
        _dtmf_sequence->append(letter);
        analyse(analysis_buffer);
        // make a statistical analysis of the buffer
        if(_current_letter!=' ')
            qDebug() << QString(_current_letter);


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
    /* spandsp
    dtmf_rx_free(dtmf);
    */
    finish:
    delete audio;
    //delete dtmf;
    emit finished();
}

void DtmfDecoder::analyse(int analysis_buffer)
{
    // wait until the buffer is full
    if(_dtmf_sequence->size()<analysis_buffer)
        return;

    int x1,x2,x3,x4,x5,x6,x7,x8,x9,x0,xa,xb,xc,xd,xx = 0;
    for(int o=0;o<_dtmf_sequence->size();++o)
    {
        char letter = _dtmf_sequence->at(o);
        if(letter == ' ')
            xx++;
        if(letter == '1')
            x1++;
        if(letter == '2')
            x2++;
        if(letter == '3')
            x3++;
        if(letter == '4')
            x4++;
        if(letter == '5')
            x5++;
        if(letter == '6')
            x6++;
        if(letter == '7')
            x7++;
        if(letter == '8')
            x8++;
        if(letter == '9')
            x9++;
        if(letter == '0')
            x0++;
        if(letter == 'A')
            xa++;
        if(letter == 'B')
            xb++;
        if(letter == 'C')
            xc++;
        if(letter == 'D')
            xd++;
    }
    if(_dtmf_sequence->last()==' ')
    {
        for(int i =analysis_buffer-1;i>analysis_buffer-5;--i)
        {
            if(_dtmf_sequence->at(i)!=' ')
                //wait for another iteration
                return;


        }
        //end of a letter
        //_current_letter = ' ';
        return;
    }
    else
    {
        if(x1 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '1';
            return;
        }
        if(x2 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '2';
            return;
        }
        if(x3 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '3';
            return;
        }
        if(x4 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '4';
            return;
        }
        if(x5 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '5';
            return;
        }
        if(x6 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '6';
            return;
        }
        if(x7 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '7';
            return;
        }
        if(x8 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '8';
            return;
        }
        if(x9 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '9';
            return;
        }
        if(x0 > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '0';
            return;
        }
        if(xa > round(_dtmf_sequence->size()/2))
        {
            _current_letter = 'A';
            return;
        }
        if(xb > round(_dtmf_sequence->size()/2))
        {
            _current_letter = 'B';
            return;
        }
        if(xc > round(_dtmf_sequence->size()/2))
        {
            _current_letter = 'C';
            return;
        }
        if(xd > round(_dtmf_sequence->size()/2))
        {
            _current_letter = 'D';
            return;
        }
        // no letter has prevalence, wait for another iteration
        return;
    }
}

char DtmfDecoder::decode(float *buf,int BUFSIZE,int SAMP_RATE, float treshhold_audio_power)
{
    int tones[2];
    tones[0] = 0;
    tones[1] = 0;
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
    return letter;
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
