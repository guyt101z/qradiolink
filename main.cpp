#include <QCoreApplication>


int main(int argc, char *argv[])
{
    int detector(char* name);
    QCoreApplication a(argc, argv);
    detector("test");
    return a.exec();
}
