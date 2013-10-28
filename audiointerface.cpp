#include "audiointerface.h"

AudioInterface::AudioInterface(QObject *parent, unsigned sample_rate, unsigned channels) :
    QObject(parent)
{
    static const pa_sample_spec ss = {
      .format = PA_SAMPLE_S16LE,
      .rate = sample_rate,
      .channels = channels
    };
    ///
    _s_rec = NULL;
    _s_play = NULL;
    _error=0;

    if (!(_s_rec = pa_simple_new(NULL, "yolink", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &_error)))
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(_error));
    }
    if (!(_s_play = pa_simple_new(NULL, "yolink", PA_STREAM_PLAYBACK, NULL, "play", &ss, NULL, NULL, &_error)))
    {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(_error));
    }
}

AudioInterface::~AudioInterface()
{
    if (_s_rec)
      pa_simple_free(_s_rec);
    if (_s_play)
      pa_simple_free(_s_play);
}

int AudioInterface::read(short *buf, short bufsize)
{
    if(!_s_rec)
        return 1;
    if (pa_simple_read(_s_rec, buf, bufsize, &_error) < 0)
    {
        fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(_error));
        return 1;
    }
    return 0;
}

int AudioInterface::write(short *buf, short bufsize)
{
    if(!_s_play)
        return 1;
    if(pa_simple_write(_s_play, buf, bufsize, &_error) < 0)
    {
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(_error));
        return 1;
    }
    return 0;
}
