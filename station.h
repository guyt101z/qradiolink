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
    bool _in_call;
    QString _conference_id;
    int _called_by;     // calling station id
    int _call_time;     // time of call
    bool _repeater;
    bool _local;
    bool _active;
};

#endif // STATION_H
