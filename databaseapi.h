#ifndef DATABASEAPI_H
#define DATABASEAPI_H

#include <QtSql>
#include <QVector>
#include <QString>
#include <math.h>
#include "station.h"
#include "server.h"

class DatabaseApi
{
public:
    DatabaseApi();
    ~DatabaseApi();
    Station* get_station_by_radio_id(QString radio_id);
    Station* get_station_by_id(int id);
    Station* get_local_station();
    void update_station_parameters(Station *s);
    QVector<Server*> get_servers(int active = 1);

private:
    QSqlDatabase _db;
};

#endif // DATABASEAPI_H
