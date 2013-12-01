#include "audioclient.h"





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
        break;


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
    if(state.state & IAXC_CALL_STATE_TRANSFER) qDebug() << "transferring..";
    if((state.state & IAXC_CALL_STATE_RINGING) &&
            !(state.state & IAXC_CALL_STATE_OUTGOING) )
    {
        iaxc_answer_call(state.callNo);
    }
    if(state.state & IAXC_CALL_STATE_COMPLETE)
    {
        emit callAnswered();
    }

    if(state.state == IAXC_CALL_STATE_FREE)
    {
        emit callEnded();
    }

}

void AudioClient::setProperties(QString username, QString password, QString server)
{
    _username = username;
    _password = password;
    _server = server;
}

void AudioClient::end()
{
    iaxc_shutdown();
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
    iaxc_input_level_set( 0.96 );
    int volume =0.96;
    iaxc_output_level_set( volume );
    iaxc_millisleep(50);
    //iaxc_set_filters(IAXC_FILTER_DENOISE | IAXC_FILTER_AGC | IAXC_FILTER_ECHO);
    //iaxc_debug_iax_set(2);



    //std::string num = "adrian:supersecret@localhost/0190909090910000";
    //std::string num = "adrian:supersecret@localhost/600";


}

void AudioClient::makeCall(std::string number)
{
    int call0, call1;
    std::string num = _username.toStdString().append(":").append(_password.toStdString()).append("@").append(_server.toStdString()).append("/");
    num.append(number);

    if( !num.empty() )
    {
        call0 = iaxc_call(num.c_str());
    }
    if( call0 == -1 )
        qDebug() << "Audio: cannot call " << num.c_str();

}

void AudioClient::disconnectCall()
{

    iaxc_dump_all_calls();

}

void AudioClient::sendDTMF(char letter)
{
    iaxc_send_dtmf(letter);
}



