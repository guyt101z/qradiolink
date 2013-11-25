#ifndef SERVER_H
#define SERVER_H

#include <QString>

class Server
{
public:
    Server();
    int _id;
    QString _ip;
    QString _hostname;
    int _connected;
    int _active;

};

#endif // SERVER_H
