#ifndef DATABASEAPI_H
#define DATABASEAPI_H

#include <QtSql>
#include <QVector>
#include <QString>
#include <math.h>
#include "station.h"

class DatabaseApi
{
public:
    DatabaseApi();
    ~DatabaseApi();
    Station* get_station_by_radio_id(QString radio_id);
    Station* get_station_by_id(int id);

private:
    QSqlDatabase _db;
};

#endif // DATABASEAPI_H
