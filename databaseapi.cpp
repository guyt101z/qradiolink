// Written by Adrian Musceac YO8RZZ at gmail dot com, started October 2013.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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


Station* DatabaseApi::get_station_by_radio_id(QString radio_id)
{
    Station *station=new Station;
    QSqlQuery query(_db);
    query.prepare("SELECT * FROM stations WHERE radio_id=:radio_id LIMIT 1");
    query.bindValue(":radio_id", radio_id);
    query.exec();

    int id_idx = query.record().indexOf("id");
    int callsign_idx = query.record().indexOf("callsign");
    int radio_id_idx = query.record().indexOf("radio_id");
    int ip_idx = query.record().indexOf("ip");
    int hostname_idx = query.record().indexOf("hostname");
    int in_call_idx = query.record().indexOf("in_call");
    int conference_id_idx = query.record().indexOf("conference_id");
    int called_by_idx = query.record().indexOf("called_by");
    int call_time_idx = query.record().indexOf("call_time");
    int repeater_idx = query.record().indexOf("repeater");
    int local_idx = query.record().indexOf("local");
    int active_idx = query.record().indexOf("active");
    if(query.next())
    {
        station->_id = query.value(id_idx).toInt();
        station->_callsign = query.value(callsign_idx).toString();
        station->_radio_id = query.value(radio_id_idx).toString();
        station->_ip = query.value(ip_idx).toString();
        station->_hostname = query.value(hostname_idx).toString();
        station->_in_call = query.value(in_call_idx).toInt();
        station->_conference_id = query.value(conference_id_idx).toString();
        station->_called_by = query.value(called_by_idx).toInt();
        station->_call_time = query.value(call_time_idx).toInt();
        station->_repeater = query.value(repeater_idx).toInt();
        station->_local = query.value(local_idx).toInt();
        station->_active = query.value(active_idx).toInt();
    }

    return station;
}

Station* DatabaseApi::get_station_by_id(int id)
{
    Station *station=new Station;
    QSqlQuery query(_db);
    query.prepare("SELECT * FROM stations WHERE id=:id LIMIT 1");
    query.bindValue(":id", id);
    query.exec();

    int id_idx = query.record().indexOf("id");
    int callsign_idx = query.record().indexOf("callsign");
    int radio_id_idx = query.record().indexOf("radio_id");
    int ip_idx = query.record().indexOf("ip");
    int hostname_idx = query.record().indexOf("hostname");
    int in_call_idx = query.record().indexOf("in_call");
    int conference_id_idx = query.record().indexOf("conference_id");
    int called_by_idx = query.record().indexOf("called_by");
    int call_time_idx = query.record().indexOf("call_time");
    int repeater_idx = query.record().indexOf("repeater");
    int local_idx = query.record().indexOf("local");
    int active_idx = query.record().indexOf("active");
    if(query.next())
    {
        station->_id = query.value(id_idx).toInt();
        station->_callsign = query.value(callsign_idx).toString();
        station->_radio_id = query.value(radio_id_idx).toString();
        station->_ip = query.value(ip_idx).toString();
        station->_hostname = query.value(hostname_idx).toString();
        station->_in_call = query.value(in_call_idx).toInt();
        station->_conference_id = query.value(conference_id_idx).toString();
        station->_called_by = query.value(called_by_idx).toInt();
        station->_call_time = query.value(call_time_idx).toInt();
        station->_repeater = query.value(repeater_idx).toInt();
        station->_local = query.value(local_idx).toInt();
        station->_active = query.value(active_idx).toInt();
    }

    return station;
}

Station* DatabaseApi::get_local_station()
{
    Station *station=new Station;
    QSqlQuery query(_db);
    query.prepare("SELECT * FROM stations WHERE local=1 LIMIT 1");
    query.exec();

    int id_idx = query.record().indexOf("id");
    int callsign_idx = query.record().indexOf("callsign");
    int radio_id_idx = query.record().indexOf("radio_id");
    int ip_idx = query.record().indexOf("ip");
    int hostname_idx = query.record().indexOf("hostname");
    int in_call_idx = query.record().indexOf("in_call");
    int conference_id_idx = query.record().indexOf("conference_id");
    int called_by_idx = query.record().indexOf("called_by");
    int call_time_idx = query.record().indexOf("call_time");
    int repeater_idx = query.record().indexOf("repeater");
    int local_idx = query.record().indexOf("local");
    int active_idx = query.record().indexOf("active");
    if(query.next())
    {
        station->_id = query.value(id_idx).toInt();
        station->_callsign = query.value(callsign_idx).toString();
        station->_radio_id = query.value(radio_id_idx).toString();
        station->_ip = query.value(ip_idx).toString();
        station->_hostname = query.value(hostname_idx).toString();
        station->_in_call = query.value(in_call_idx).toInt();
        station->_conference_id = query.value(conference_id_idx).toString();
        station->_called_by = query.value(called_by_idx).toInt();
        station->_call_time = query.value(call_time_idx).toInt();
        station->_repeater = query.value(repeater_idx).toInt();
        station->_local = query.value(local_idx).toInt();
        station->_active = query.value(active_idx).toInt();
    }

    return station;
}



void DatabaseApi::update_station_parameters(Station *s)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE stations SET "
                  "in_call =:in_call, conference_id=:conference_id, "
                  "called_by = :called_by, call_time=:call_time "
                  " WHERE id=:id");
    query.bindValue(":in_call", s->_in_call);
    query.bindValue(":conference_id", s->_conference_id);
    query.bindValue(":called_by", s->_called_by);
    query.bindValue(":call_time", s->_call_time);
    query.bindValue(":id", s->_id);
    query.exec();
}

QVector<Server*> DatabaseApi::get_servers(int active)
{
    QVector<Server *> servers;

    QSqlQuery query(_db);
    query.prepare("SELECT * FROM servers WHERE active=:active");
    query.bindValue(":active", active);
    query.exec();

    int id_idx = query.record().indexOf("id");
    int ip_idx = query.record().indexOf("ip");
    int hostname_idx = query.record().indexOf("hostname");
    int connected_idx = query.record().indexOf("connected");
    int active_idx = query.record().indexOf("active");
    int username_idx = query.record().indexOf("username");
    int password_idx = query.record().indexOf("password");
    while(query.next())
    {
        Server *server = new Server;
        server->_id = query.value(id_idx).toInt();
        server->_ip = query.value(ip_idx).toString();
        server->_hostname = query.value(hostname_idx).toString();
        server->_connected = query.value(connected_idx).toInt();
        server->_active = query.value(active_idx).toInt();
        server->_username = query.value(username_idx).toString();
        server->_password = query.value(password_idx).toString();
        servers.push_back(server);
    }
    return servers;
}

Server* DatabaseApi::get_server_by_id(int id)
{
    Server *server = new Server;

    QSqlQuery query(_db);
    query.prepare("SELECT * FROM servers WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();

    int id_idx = query.record().indexOf("id");
    int ip_idx = query.record().indexOf("ip");
    int hostname_idx = query.record().indexOf("hostname");
    int connected_idx = query.record().indexOf("connected");
    int active_idx = query.record().indexOf("active");
    int username_idx = query.record().indexOf("username");
    int password_idx = query.record().indexOf("password");
    if(query.next())
    {

        server->_id = query.value(id_idx).toInt();
        server->_ip = query.value(ip_idx).toString();
        server->_hostname = query.value(hostname_idx).toString();
        server->_connected = query.value(connected_idx).toInt();
        server->_active = query.value(active_idx).toInt();
        server->_username = query.value(username_idx).toString();
        server->_password = query.value(password_idx).toString();
    }

    return server;
}
