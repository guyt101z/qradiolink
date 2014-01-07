#include "utils.h"

void addPreamble(quint8 *buffer, quint16 type, quint32 len)
{
    buffer[1] = (type) & 0xff;
    buffer[0] = (type >> 8) & 0xff;

    buffer[5] = (len) & 0xff;
    buffer[4] = (len >> 8) & 0xff;
    buffer[3] = (len >> 16) & 0xff;
    buffer[2] = (len >> 24) & 0xff;
}

void getPreamble(quint8 *buffer, int *type, int *len)
{
    quint16 msgType;
    quint32 msgLen;

    msgType = buffer[1] | (buffer[0] << 8);
    msgLen = buffer[5] | (buffer[4] << 8) | (buffer[3] << 16) | (buffer[2] << 24);
    *type = (int)msgType;
    *len = (int)msgLen;
}

void genRandomStr(char *str, const int len)
{
    static const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(0));
    for (int i = 0; i < len; ++i)
    {
        str[i] = letters[rand() % (sizeof(letters) - 1)];
    }

    str[len] = 0;
}

#include <cmath>
#include <cstdlib>

using std::rand;
// math stuff
using std::cos;
using std::abs;
using std::exp;
using std::log10;
/**
  Written by: Espen Riskedal, espenr@ii.uib.no, july-2002
  */


/** Generates a tone of the specified frequency
*  Gotten from: http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&oe=UTF-8&safe=off&selm=3c641e%243jn%40uicsl.csl.uiuc.edu
*/
float *makeTone(int samplerate, float frequency, int length, float gain) {
    //y(n) = 2 * cos(A) * y(n-1) - y(n-2)
    //A= (frequency of interest) * 2 * PI / (sampling frequency)
    //A is in radians.
    // frequency of interest MUST be <= 1/2 the sampling frequency.
    float *tone = new float[length];
    float A = frequency*2*PI/samplerate;

    for (int i=0; i<length; i++)
    {
        if (i > 1) tone[i]= 2*cos(A)*tone[i-1] - tone[i-2];
        else if (i > 0) tone[i] = 2*cos(A)*tone[i-1] - (cos(A));
        else tone[i] = 2*cos(A)*cos(A) - cos(2*A);
    }

    for (int i=0; i<length; i++) tone[i] = tone[i]*gain;

    return tone;
}

float goertzel(float *x, int N, float frequency, int samplerate) {
    float Skn, Skn1, Skn2;
    Skn = Skn1 = Skn2 = 0;

    for (int i=0; i<N; i++)
    {
        if(x[i] > 1.0)
            x[i]=0;
        if(x[i] < -1.0)
            x[i]=0;
        Skn2 = Skn1;
        Skn1 = Skn;
        Skn = 2*cos(2*PI*frequency/(float)samplerate)*Skn1 - Skn2 + x[i];
    }

    float WNk = exp(-2*PI*frequency/(float)samplerate); // this one ignores complex stuff
    //float WNk = exp(-2*j*PI*k/N);
    return (Skn - WNk*Skn1);
}

float power(float value) {
    if(fabs(value-0.0)<=0.0001) return 0;
    return 20*log10(fabs(value));
}

// this one doesn't ignore complex stuff
float goertzel_magnitude(float* data, int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE )
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
    real = (q1 - q2 * cosine) / scalingFactor;
    imag = (q2 * sine);

    magnitude = sqrtf(real*real + imag*imag);
    return magnitude;
}

