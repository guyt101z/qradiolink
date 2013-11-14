#include "audioclient.h"

#define NUM_CALLS 4
#define DEFAULT_SERVER "fgcom.flightgear.org"
#define IAX_DELAY 300 // delay between calls in milliseconds
#define TEST_FREQ 910.00



AudioClient::AudioClient(QObject *parent) :
    QObject(parent)
{
    _initialized = false;
    _enabled = false;
    _register =true;
}

AudioClient::~AudioClient()
{
}

static AudioClient* static_instance = NULL;

static int iaxc_callback( iaxc_event e )
{
    switch( e.type )
    {
        case IAXC_EVENT_TEXT:
            static_instance->iaxTextEvent(e.ev.text);
        break;
    case IAXC_EVENT_STATE:
        static_instance->iaxCallEvent(e.ev.call);
        default:
            return 0;
    }
    return 1;
}

void AudioClient::iaxTextEvent(struct iaxc_ev_text text)
{
    if( (text.type == IAXC_TEXT_TYPE_STATUS ||
        text.type == IAXC_TEXT_TYPE_IAX) )
    {
        qDebug() << text.message;
    }
}

void AudioClient::iaxCallEvent(struct iaxc_ev_call_state state)
{

        qDebug() << state.remote;

}

void AudioClient::setProperties(QString username, QString password, QString server)
{
    _username = username;
    _password = password;
    _server = server;
}


void AudioClient::init()
{



    if( iaxc_initialize(NUM_CALLS))
    {
        qDebug() << "Audio: cannot initialize iaxclient";
        _enabled = false;
        return;
    }

    static_instance = this;
    iaxc_set_event_callback( iaxc_callback );
    // FIXME: To be implemented in IAX audio driver
    //iaxc_mic_boost_set( _micBoost_node->getIntValue() );
    QString app = "QRadioLink";
    QString callsign = "YR443";
    iaxc_set_callerid( callsign.toUtf8(), app.toUtf8() );
    iaxc_set_formats (IAXC_FORMAT_SPEEX, IAXC_FORMAT_SPEEX);
    iaxc_set_speex_settings(1, 5, 0, 1, 0, 3);
    iaxc_set_filters(IAXC_FILTER_AGC | IAXC_FILTER_DENOISE);
    iaxc_set_silence_threshold(-20.0);
    iaxc_set_audio_output(0);
    iaxc_start_processing_thread ();
    iaxc_millisleep (5);
    // Now IAXClient is initialized
    _initialized = true;
    if ( _register )
    {
        int regId = iaxc_register( const_cast<char*>(_username.toUtf8().constData()),
        const_cast<char*>(_password.toUtf8().constData()),
        const_cast<char*>(_server.toUtf8().constData()) );
        if( regId == -1 )
        {
            qDebug() << "Audio: cannot register iaxclient";
            return;
        }
    }


    struct iaxc_audio_device *devs;
    int nDevs, input, output, ring;
    iaxc_audio_devices_get(&devs,&nDevs, &input, &output, &ring);
    for(int i=0; i<nDevs; i++ )
    {

        // devID

        qDebug() << devs[i].devID;
        // name

        qDebug() << devs[i].name;
        // input capability

        if( devs[i].capabilities & IAXC_AD_INPUT )
            qDebug() << "IAXC_AD_INPUT";
        else
            qDebug() << "no IAXC_AD_INPUT";
        // output capability

        if( devs[i].capabilities & IAXC_AD_OUTPUT )
            qDebug() << "IAXC_AD_OUTPUT";
        else
            qDebug() << "no IAXC_AD_OUTPUT";
    // use default device at start
        if( devs[i].capabilities & IAXC_AD_INPUT_DEFAULT )
            int input_id=devs[i].devID;
        if( devs[i].capabilities & IAXC_AD_OUTPUT_DEFAULT )
            int output_id = devs[i].devID;
    }
    //TODO: set volumes in a separate function
    iaxc_input_level_set( 0.9 );
    int volume =0.9;
    iaxc_output_level_set( volume );
    iaxc_millisleep(50);

    const double freq = 910.0;
    int currentFreqKhz = 10 * static_cast<int>(freq * 100 + 0.25);

    //std::string num = computePhoneNumber(freq, "KSFO");
    //std::string num = "adrian:supersecret@localhost/0190909090910000";
    //std::string num = "adrian:supersecret@localhost/600";


}

void AudioClient::makeCall(std::string number)
{
    int call0, call1;
    std::string num = "adrian:supersecret@localhost/";
    num.append(number);
    qDebug() << QString(num.c_str());
    if( !num.empty() )
    {
        call0 = iaxc_call(num.c_str());
    }
    if( call0 == -1 )
        qDebug() << "Audio: cannot call " << num.c_str();

}

void AudioClient::sendDTMF(char letter)
{
    iaxc_send_dtmf(letter);
}

void AudioClient::haveCall(QVector<char> *dtmf)
{
    std::string number;
    for(int i=0;i<dtmf->size();i++)
    {
        if((dtmf->at(i)!='*') && (dtmf->at(i)!='C'))
        {
            number.push_back(dtmf->at(i));
        }
    }
    emit readyInput();
    makeCall(number);


    dtmf->clear();
}


std::string AudioClient::computePhoneNumber(const double& freq, const std::string& icao) const
{
    if( icao.empty() )
        return std::string();
    char phoneNumber[256];
    char exten[32];
    char tmp[5];
    /*Convert ICAO to ASCII */
    sprintf( tmp, "%4s", icao.c_str() );
    /*Built the phone number */
    sprintf( exten, "%02d%02d%02d%02d%02d%06d",  01, tmp[0], tmp[1], tmp[3], (int) (freq * 1000 + 0.5) );
    exten[16] = '\0';
    snprintf( phoneNumber, sizeof (phoneNumber), "%s:%s@%s/%s",
              _username.toStdString().c_str(), _password.toStdString().c_str(), _server.toStdString().c_str(), exten);
    return phoneNumber;
}
