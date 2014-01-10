#include "settings.h"

Settings::Settings()
{
    _id = 0;
    _use_mumble = 1;
    _mumble_tcp = 0;
    _use_codec2 = 0;
    _audio_treshhold = -15;
    _voice_activation = 0.5;
    _voice_activation_timeout = 50;
    _voice_server_port = 64738;
    _local_udp_port = 4938;
    _control_port = 4939;
    _opus_bitrate = 8000;
    _codec2_bitrate =1200;
    _enable_vox = 1;
    _enable_agc = 1;
}
