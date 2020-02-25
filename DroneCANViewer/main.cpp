/**

Copyright (c) 2020 DroneCAN

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

**/

#include <QApplication>
#include <qcommandlineparser.h>

#include "mainwindow.hpp"
#include "version.hpp"
#include "debug.hpp"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(DroneCAN::debugHandler);

    QApplication a(argc, argv);

    a.setOrganizationDomain("https://dronecan.org");
    a.setOrganizationName("DroneCAN");
    a.setApplicationVersion(DroneCAN::Version::version);
    a.setApplicationName("DroneCAN Viewer");
    a.setApplicationDisplayName("DroneCAN Viewer");

    QCommandLineParser parser;

    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("DroneCAN Viewer");

    QCommandLineOption dbgLevel(QStringList() << "d" << "debug", QCoreApplication::translate("main", "Set the debug level"), "2");

    parser.addOption(dbgLevel);

    parser.process(a);

    bool ok = false;

    // Set the debug level
    int dbgValue = parser.value(dbgLevel).toInt(&ok);

    if (ok)
    {
        DroneCAN::setDebugLevel(dbgValue);
    }

    // Add the local plugins directory to the search path
    QString pluginsDir = a.applicationDirPath() + "/plugins/";

    QCoreApplication::addLibraryPath(pluginsDir);

    DCDebug << "Starting DroneCANViewer in" << a.applicationDirPath();

    MainWindow w;

    w.show();

    return a.exec();
}
