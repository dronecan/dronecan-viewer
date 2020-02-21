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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "debug.hpp"

namespace DroneCAN {

// By default, display warning or higher
static uint8_t DBG_LEVEL = QtDebugMsg;


void setDebugLevel(uint8_t level)
{
    DBG_LEVEL = level;
}


void debugHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
    // Quickly decide if we are going to display the message or not
    if (DBG_LEVEL > msgType)
    {
        return;
    }

    QByteArray message = msg.toLocal8Bit();

    const char *file = context.file ? context.file : "";
    const char *func = context.function ? context.function : "";

    QString prefix;

    QTime t = QTime::currentTime();

    QString time = t.toString() + "." + QString::number(t.msec());

    switch (msgType)
    {
    default:
        break;
    case QtFatalMsg:
        prefix = "Fatal:";
        break;
    case QtCriticalMsg:
        prefix = "Critical:";
        break;
    case QtWarningMsg:
        prefix = "Warning:";
        break;
    case QtInfoMsg:
        prefix = "Info:";
        break;
    case QtDebugMsg:
        prefix = "Debug:";
        break;
    }

    if (!prefix.isEmpty())
    {
        QString out = time + QString(" ") + prefix + QString(" ") + QString(message);

        std::cout << out.toStdString() << std::endl;

        // TODO - Print file and function information if it exists
        Q_UNUSED(file);
        Q_UNUSED(func);
    }
}

}
