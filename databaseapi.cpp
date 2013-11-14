#include "databaseapi.h"

DatabaseApi::DatabaseApi()
{
    _db =  QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName("qradiolink.db");
    if(!_db.open())
    {
        qDebug() << "Could not connect to database!";
    }
}

DatabaseApi::~DatabaseApi()
{
    _db.close();
}
