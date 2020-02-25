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

#include "debug.hpp"
#include "adapter.hpp"


DroneCANInterface::DroneCANInterface(QObject *parent) : QThread(parent)
{
    DCDebug << "DroneCANInterface created";
}


DroneCANInterface::~DroneCANInterface()
{
    DCDebug << "DroneCANInterface destroyed";
}


void DroneCANInterface::run()
{
    running = true;

    DCDebug << "DroneCANInterface starting";

    while (running)
    {
        msleep(1000);
    }

    DCDebug << "DroneCANInterface complete";
}


void DroneCANInterface::stop()
{
    running = false;
}


/**
 * @brief DroneCANInterface::GetPlugins - List the available CAN adapter plugins
 * @return QStringList containing names of detected plugins
 */
QStringList DroneCANInterface::GetPlugins()
{
    return QCanBus::instance()->plugins();
}


/**
 * @brief DroneCANInterface::GetDevices - List the devices available for the provided plugin
 * @param pluginName - Name of the CAN adapter plugin
 * @param errorMsg - Optional pointer to store an error message
 * @return - List of available devices
 */
QStringList DroneCANInterface::GetDevices(const QString pluginName, QString *errorMsg)
{
    auto available = QCanBus::instance()->availableDevices(pluginName, errorMsg);

    QStringList devices;

    for (auto info : available)
    {
        if (info.isVirtual()) continue;

        devices.append(info.name());
    }

    return devices;
}
