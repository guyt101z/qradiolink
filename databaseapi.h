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

private:
    QSqlDatabase _db;
};

#endif // DATABASEAPI_H
