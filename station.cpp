#include "station.h"

Station::Station()
{
    _id = 0;
    _callsign="";
    _radio_id = "";
    _ip="127.0.0.1";
    _hostname="localhost";
    _in_call=0;
    _conference_id="";
    _called_by=0;
    _call_time=0;
    _repeater=0;
    _local=0;
    _active=1;
    _waiting = 0;
}
