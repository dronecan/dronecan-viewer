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

#ifndef _DC_CAN_H_
#define _DC_CAN_H_

#include <qthread.h>
#include <qcanbus.h>
#include <qpluginloader.h>


/**
 * @brief The DroneCANAdapter class provides a device-agnostic interface to the CAN bus.
 *
 * The DroneCANAdapter class is threaded, and manages queued Tx and Rx CAN frames.
 *
 * Multi-frame messages are automatically handled, and presented to the decode thread once parsed.
 */
class DroneCANInterface : public QThread
{
    Q_OBJECT

public:
    DroneCANInterface(QObject *parent = nullptr);
    virtual ~DroneCANInterface();

    static QStringList GetPlugins();
    static QStringList GetDevices(const QString pluginName, QString *errorMsg = nullptr);

public slots:
    virtual void run() override;
    void stop();
};


#endif // _DC_CAN_H_
