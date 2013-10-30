/*
* iaxclient: a cross-platform IAX softphone library
*
* Copyrights:
* Copyright (C) 2008 - Brandon Kruse
* Copyright (C) 2008 - Digium, Inc.
* Copyright (C) 2013 - Adrian Musceac
* 
* Contributors:
* Brandon Kruse
* Adrian Musceac
* 
* This program is free software, distributed under the terms of
* the GNU Lesser (Library) General Public License.
*
*/

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "iaxclient_lib.h"

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

static  pa_sample_spec ss;



/*
* This callback is given to indicate that a sound needs to be added to the
* current list of sounds to play. When a sound is active (after this is
* called but before stop_sound), the audio for this sound should be mixed
* with the normal output audio, and any other current sounds.
*
* It seems like this code would end up being almost identical to the code
* in the portaudio interface. Maybe it should be moved up and not handled
* by the audio interface at all.
*/
static int pulse_play_sound(struct iaxc_sound *inSound, int ring)
{
  return 0;
}

/*
* This is called after play_sound to indicate that a sound should no longer
* be played.
*/
static int pulse_stop_sound(int soundID)
{
  return 0;
}


/*
* Start audio I/O.
*/
static int pulse_start(struct iaxc_audio_driver *d)
{
  return 0;
}

/*
* Stop audio I/O.
*/
static int pulse_stop(struct iaxc_audio_driver *d)
{
  return 0;
}


static int pulse_input(struct iaxc_audio_driver *d, void *samples, int *nSamples)
{
  
  int error;
  pa_simple *p = (pa_simple*)(d->priv);
  if(pa_simple_read(p, samples, *nSamples*sizeof(short) , &error) <0)
  {
    fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
  }
  // always return 0, since pulse gives us a continuos stream of data
  return 0;
}

static int pulse_output(struct iaxc_audio_driver *d, void *samples, int nSamples)
{
  int error;

  pa_simple *p = (pa_simple*)(d->priv2);
  if(pa_simple_write(p, samples, nSamples*sizeof(short), &error) <0)
  {
    fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
  }
  // this is just a bogus return
  return 0;

}

static int pulse_select_devices(struct iaxc_audio_driver *d, int input, int output, int ring)
{
  return 0;
}

int pulse_selected_devices(struct iaxc_audio_driver *d, int *input, int *output, int *ring)
{
  *input = 0;
  *output = 0;
  *ring = 0;
  return 0;
}

static int pulse_destroy(struct iaxc_audio_driver *d)
{
  
  if (d->priv)
  {
    pa_simple *p = (pa_simple*)(d->priv);
    pa_simple_flush(p,NULL);
  }

  
  if (d->priv)
  {
    pa_simple *p = (pa_simple*)(d->priv);
    pa_simple_free(p);
  }

  return 0;
}

static double pulse_input_level_get(struct iaxc_audio_driver *d)
{
  return -1;
}

static double pulse_output_level_get(struct iaxc_audio_driver *d)
{
  return -1;
}

static int pulse_input_level_set(struct iaxc_audio_driver *d, double level)
{
  return -1;
}

static int pulse_output_level_set(struct iaxc_audio_driver *d, double level)
{
  return -1;
}


int pulse_initialize(struct iaxc_audio_driver *d, int sample_rate)
{
  pa_simple *sp = NULL;
  pa_simple *sr = NULL;
  
  if (!sample_rate || sample_rate < 8000)
    sample_rate = 8000;

  ss.channels = 1; 
  ss.rate = sample_rate; 
  ss.format = PA_SAMPLE_S16LE; 

  // record stream pointer
  sr = pa_simple_new(NULL,
    "FGCom",
    PA_STREAM_RECORD, 
    NULL, 
    "Iax2-Client", 
    &ss, 
    NULL, 
    NULL, 
    NULL 
  );
  // playback stream pointer
  sp = pa_simple_new(NULL, 
    "FGCom", 
    PA_STREAM_PLAYBACK, 
    NULL, 
    "Iax2-Client", 
    &ss, 
    NULL, 
    NULL, 
    NULL 
  );
  d->priv = sr;
  d->priv2 = sp;
  d->initialize = pulse_initialize; /* This function */
  d->destroy = pulse_destroy; /* Implemented to flush buffers and then free resources */
  d->select_devices = pulse_select_devices; /* Bogey function, pulse audio connects via resource thread */
  d->selected_devices = pulse_selected_devices; /* Same as above */
  d->start = pulse_start; 
  d->stop = pulse_stop; 
  d->output = pulse_output; /* playback stream */
  d->input = pulse_input; /* record stream */
  d->input_level_get = pulse_input_level_get;
  d->input_level_set = pulse_input_level_set;
  d->output_level_get = pulse_output_level_get;
  d->output_level_set = pulse_output_level_set;
  d->play_sound = pulse_play_sound;
  d->stop_sound = pulse_stop_sound;

  return 0;
}