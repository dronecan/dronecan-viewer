#include "mainwindow.hpp"
#include <QApplication>

#include "debug.hpp"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(DroneCAN::debugHandler);

    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}
