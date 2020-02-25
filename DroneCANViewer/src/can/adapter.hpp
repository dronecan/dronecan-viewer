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
#include <qmutex.h>
#include <qpluginloader.h>

#include <qsettings.h>

#include <memory>

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

    // TODO - Make this a shared pointer?
    QCanBusDevice* getAdapter(void);

    static QStringList GetPlugins();
    static QStringList GetDevices(const QString pluginName, QString *errorMsg = nullptr);

    bool open(QString pluginName = QString(), QString interfaceName = QString());
    bool close(void);
    bool isOpen(void);

    QString connectionString(void);

    void resetCounters(void);

    void loadSettings(QSettings &settings);
    void saveSettings(QSettings &settings);

    uint64_t getRxCount(void) const { return rxCount; }
    uint64_t getTxCount(void) const { return txCount; }

public slots:
    virtual void run() override;
    void stop();

    void startLogging(void);
    void stopLogging(void);
    bool isLogging(void);

    void onFramesReceived(void);

protected:

    void configureInterface(void);


    bool running = false;

    QString lastError;

    QCanBusDevice *adapter = nullptr;


    QString adapterName;
    QString deviceName;

    // Frame counters
    uint64_t rxCount = 0;
    uint64_t txCount = 0;

    QMutex canMutex;
    QMutex logMutex;

    // Pending log data
    QList<QStringList> pendingLogData;
};


#endif // _DC_CAN_H_
