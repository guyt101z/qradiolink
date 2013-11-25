#ifndef STATION_H
#define STATION_H

#include <QString>

class Station
{
public:
    Station();
    int _id;
    QString _callsign;
    QString _radio_id;
    QString _ip;
    QString _hostname;
    int _in_call;
    QString _conference_id;
    int _called_by;     // calling station id
    int _call_time;     // time of call
    int _repeater;
    int _local;
    int _active;
};

#endif // STATION_H
