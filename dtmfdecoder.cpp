#include "dtmfdecoder.h"

#include <cmath>
#include <cstdlib>

using std::rand;
// math stuff
using std::cos;
using std::abs;
using std::exp;
using std::log10;



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
    _dtmf_frequencies[8]= 1800.0;
    _dtmf_sequence = new QVector<char>;
    _dtmf_command = new QVector<char>;
    _current_letter = ' ';
    _previous_letter= ' ';
    _processing = true;
    _receiving = false;
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

void DtmfDecoder::process(bool p)
{
    _processing = p;
}

void DtmfDecoder::resetInput()
{
    process(true);
}

void DtmfDecoder::run()
{
    float cw_tone_freq = 900.0;
    int buffer_size = 512;
    int samp_rate = 8000;
    float treshhold_audio_power = 12.0; // dB
    float tone_difference = 5.0; //dB
    int analysis_buffer = 20;
    char call_key='C';
    char call_direct_key='D';
    char command_key='#';
    char clear_key = '*';
    AudioInterface *audio= new AudioInterface(0,samp_rate,1);

    while(true)
    {
        usleep(1000);
        QCoreApplication::processEvents();
        if(_stop)
            break;
        if(!_processing)
        {
            usleep(100000);
            continue;
        }

        float *buf = new float[buffer_size];

        audio->read(buf, buffer_size);
        /*
        float sum=0;
        for(int x=0;x<buffer_size;x++)
        {
            if(buf[x] >= 0)
                sum += buf[x];
            else
                sum -= buf[x];
        }
        */
        if(1> 2)
        {
            _receiving = true;
            //qDebug() << "sum: " << sum/treshhold_audio_power;
        }
        else
        {
            _receiving = false;
        }


        //float *tone= makeTone(samp_rate,cw_tone_freq,buffer_size);
        //audio->write(tone,buffer_size);
        char letter = decode(buf,buffer_size,samp_rate, treshhold_audio_power, tone_difference);
        // fill a buffer of decoded letters

        if(_dtmf_sequence->size()>analysis_buffer)
        {
            _dtmf_sequence->remove(0);
        }
        _dtmf_sequence->append(letter);
        // make a statistical analysis of the buffer
        analyse(analysis_buffer);

        delete[] buf;
        if(_current_letter==' ')
        {
            _previous_letter = _current_letter;
            continue;
        }
        if(_previous_letter==_current_letter) continue;

        qDebug() << QString(_current_letter);

        if(_current_letter==clear_key)
        {
            _dtmf_command->clear();

        }
        else if((_current_letter==call_key) || _current_letter==call_direct_key)
        {

            _dtmf_command->append(_current_letter);
            _processing =false;

            emit haveCall(_dtmf_command);
        }
        else if(_current_letter==command_key)
        {
            _dtmf_command->append(_current_letter);
            _processing =false;
            emit haveCommand(_dtmf_command);
        }
        else
        {
            _dtmf_command->append(_current_letter);
        }

        _previous_letter=_current_letter;
        //_current_letter=' ';


        //float *cw_sound = makeTone(samp_rate,cw_tone_freq,buffer_size,1);
        //audio->write(cw_sound,buffer_size);


    }

    finish:
    delete audio;
    //delete dtmf;
    emit finished();
}


char DtmfDecoder::decode(float *buf,int buffer_size,int samp_rate, float treshhold_audio_power, float tone_difference)
{
    int tones[2];
    tones[0] = 0;
    tones[1] = 0;
    int first=-99;
    float largest_tone_power = 0.0;
    for(int i =0;i<4;i++)
    {

        float tone_power = power(goertzel(buf, buffer_size, _dtmf_frequencies[i], samp_rate));
        if(tone_power > 60) continue; // error
        for(int j=-5;j<6;j++)
        {
            float tone_power1 = power(goertzel(buf, buffer_size, _dtmf_frequencies[i]+(float)j, samp_rate));
            if(tone_power1>tone_power) tone_power = tone_power1;
        }
        if(tone_power < largest_tone_power) continue;
        if(tone_power < treshhold_audio_power) continue;


        //qDebug() << i << " " << tone_power << " "<< _dtmf_frequencies[i];
        tones[0] = (int)_dtmf_frequencies[i];
        largest_tone_power = tone_power;
        first = i;
    }
    if(first<0)
        return ' ';
    float first_test_tone_power = power(goertzel(buf, buffer_size, _dtmf_frequencies[first]+100, samp_rate));
    if((fabs(largest_tone_power - first_test_tone_power) < 10) )
        return ' ';
    float first_tone_power=0.0;
    if(tones[0]!=0)
        first_tone_power=largest_tone_power;
    largest_tone_power =0.0;
    int second=-99;
    for(int i =4;i<9;i++)
    {

        float tone_power = power(goertzel(buf, buffer_size, _dtmf_frequencies[i], samp_rate));
        if(tone_power > 60) continue; // error
        for(int j=-5;j<6;j++)
        {
            float tone_power1 = power(goertzel(buf, buffer_size, _dtmf_frequencies[i]+(float)j, samp_rate));
            if(tone_power1>tone_power) tone_power = tone_power1;
        }
        if(tone_power < largest_tone_power) continue;
        if(tone_power < treshhold_audio_power) continue;


        //qDebug() << i << " " << tone_power << " "<< _dtmf_frequencies[i];
        tones[1] = (int)_dtmf_frequencies[i];
        largest_tone_power = tone_power;
        second = i;
    }
    if(second<0)
        return ' ';
    float second_test_tone_power = power(goertzel(buf, buffer_size, _dtmf_frequencies[second]+100, samp_rate));
    if((fabs(largest_tone_power - second_test_tone_power) < 10))
        return ' ';

    float second_tone_power=0.0;
    if(tones[1]!=0)
        second_tone_power = largest_tone_power;

    if(fabs(first_tone_power - second_tone_power) > 8)
        return ' ';


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
        case 0:
            letter = ' ';
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
        case 0:
            letter = ' ';
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
        case 0:
            letter = ' ';
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
        case 0:
            letter = ' ';
        default:
            letter = ' ';
        }
        break;
    case 0:
        letter = ' ';
        break;
    default:
        letter = ' ';
    }
    return letter;
}

void DtmfDecoder::analyse(int analysis_buffer)
{
    // wait until the buffer is full
    if(_dtmf_sequence->size()<analysis_buffer)
        return;

    int x1,x2,x3,x4,x5,x6,x7,x8,x9,x0,xa,xb,xc,xd,xs,xq,xx;
    x1=x2=x3=x4=x5=x6=x7=x8=x9=x0=xa=xb=xc=xd=xs=xq=xx=0;
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
        if(letter == '*')
            xs++;
        if(letter == '#')
            xq++;
    }


    if(xx > round(_dtmf_sequence->size()/2))
    {
        for(int i =analysis_buffer-1;i>analysis_buffer-5;i--)
        {
            if(_dtmf_sequence->at(i)!=' ')
                //wait for another iteration
                return;


        }
        _current_letter = ' ';
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
        if(xs > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '*';
            return;
        }
        if(xq > round(_dtmf_sequence->size()/2))
        {
            _current_letter = '#';
            return;
        }
        // no letter has prevalence, wait for another iteration
        return;
    }
}



/**
  Written by: Espen Riskedal, espenr@ii.uib.no, july-2002
  */


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
    Skn = 2*cos(2*PI*frequency/(float)samplerate)*Skn1 - Skn2 + x[i];
    }

    float WNk = exp(-2*PI*frequency/(float)samplerate); // this one ignores complex stuff
    //float WNk = exp(-2*j*PI*k/N);
    return (Skn - WNk*Skn1);
}

float DtmfDecoder::power(float value) {
    if(fabs(value-0.0)<=0.0001) return 0;
    return 20*log10(fabs(value));
}

// this one doesn't ignore complex stuff
float DtmfDecoder::goertzel_magnitude(float* data, int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE )
{
    int     k,i;
    float   floatnumSamples;
    float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;

    float   scalingFactor = numSamples / 2.0;

    floatnumSamples = (float) numSamples;
    k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
    omega = (2.0 * M_PI * k) / floatnumSamples;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    q0=0;
    q1=0;
    q2=0;

    for(i=0; i<numSamples; i++)
    {
        q0 = coeff * q1 - q2 + data[i];
        q2 = q1;
        q1 = q0;
    }

    // calculate the real and imaginary results
    // scaling appropriately
    real = (q1 - q2 * cosine);
    imag = (q2 * sine);

    magnitude = sqrtf(real*real + imag*imag);
    return magnitude;
}

