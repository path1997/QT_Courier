#include "monitoringpaczek.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MonitoringPaczek w;
    w.show();
    return a.exec();
}
