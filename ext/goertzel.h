#ifndef GOERTZEL_H
#define GOERTZEL_H

#include "config_defines.h"
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

#endif // GOERTZEL_H
