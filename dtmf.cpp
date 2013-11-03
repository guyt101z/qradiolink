#include "dtmf.h"

#define BUFSIZE 8
const unsigned FRAME_SIZE = 2;

Dtmf::Dtmf(QObject *parent, unsigned frame_size) :
    QObject(parent)
{
    _dialButtons[0] = '1';
    _dialButtons[1] = '2';
    _dialButtons[2] = '3';
    _dialButtons[3] = 'A';
    _dialButtons[4] = '4';
    _dialButtons[5] = '5';
    _dialButtons[6] = '6';
    _dialButtons[7] = 'B';
    _dialButtons[8] = '7';
    _dialButtons[9] = '8';
    _dialButtons[10] = '9';
    _dialButtons[11] = 'C';
    _dialButtons[12] = '*';
    _dialButtons[13] = '0';
    _dialButtons[14] = '#';
    _dialButtons[15] = 'D';
    _dtmfDetector = new DtmfDetector(frame_size);
    _dtmfDetector->zerosIndexDialButton();

}

Dtmf::~Dtmf()
{
    delete _dtmfDetector;
}

void Dtmf::decode(short int* buf)
{


    _dtmfDetector->zerosIndexDialButton();
    _dtmfDetector->dtmfDetecting(buf);

    //char *arr = _dtmfDetector->getDialButtonsArray();
    //std::cerr << arr << std::endl;
    /*
    char text[1024];
    if(arr[0]==NULL) continue;
    strcat (text, "I have received letter ");
    strcat (text ,&arr[0]);
    //strcat (text ,"\0");
    speak(text);
    */
    //delete[] buf;
    //if(c!=' ')
      //printf("%s",c);
    //printf("%s",_dialButtons[dtmfDetector.getIndexdialButtons()]);
    //if(c!='\0')
      //fprintf(stderr, __FILE__": char: %s\n",c);
}

short* Dtmf::encode()
{



    DtmfGenerator dtmfGenerator(FRAME_SIZE, 400, 20);
    dtmfGenerator.dtmfGeneratorReset();
    dtmfGenerator.transmitNewDialButtonsArray(_dialButtons, 16);
    short *audio_buffer =new short[1024];
    AudioInterface audio(0,8000,2);
    int si = 0;
    while(!dtmfGenerator.getReadyFlag())
    {

        short *buf =new short[BUFSIZE];
        dtmfGenerator.dtmfGenerating(buf);
        //audio.write(buf, BUFSIZE);
        std::copy(buf,buf+sizeof(buf),audio_buffer+si);
        si +=sizeof(buf) + 1;
    }
    qDebug() << si;
    return audio_buffer;

}

short* Dtmf::transmitCode(short* buf)
{
    /// callsign Morse
    for(int j=0;j<6;j++)
    {
      DtmfGenerator *d;
      if((j%2)==0)
      {
        d = new DtmfGenerator(FRAME_SIZE, 200, 200);
      }
      else
      {
        d= new DtmfGenerator(FRAME_SIZE, 80, 200);
      }
      d->dtmfGeneratorReset();
      char call[1];
      call[0]= 'D';
      d->transmitNewDialButtonsArray(call, 1);
      while(!d->getReadyFlag())
      {
        d->dtmfGenerating(buf);
        //pa_simple_write(s2, buf, BUFSIZE, &error);
      }
      usleep(20000);
      delete d;
    }
    return buf;

}

void Dtmf::decodeAudio(AudioInterface *audio)
{

    short int buf[BUFSIZE];
    //audio->read(buf,BUFSIZE);

    decode(buf);
}
