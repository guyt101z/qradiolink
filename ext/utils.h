#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QString>
#include <QtEndian>
#include <string>
#include <sys/time.h>
#include "config_defines.h"

void genRandomStr(char *str, const int len);
void addPreamble(quint8 *buffer, quint16 type, quint32 len);
void getPreamble(quint8 *buffer, int *type, int *len);

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

#endif // UTILS_H
