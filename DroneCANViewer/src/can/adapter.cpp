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


/**
 * @brief DroneCANInterface::getAdapter - Get a pointer to the current adapter
 * @return
 */
QCanBusDevice* DroneCANInterface::getAdapter()
{
    // TODO - Return a shared pointer here instead of a raw pointer!
    return adapter;
}


/**
 * @brief DroneCANInterface::open - Connect to a CAN adapter
 * @param pluginName - Name of the plugin. If empty, the first available plugin is used
 * @param interfaceName - Name of the device. If empty, the first available device is used
 * @return true if connection was successful
 */
bool DroneCANInterface::open(QString pluginName, QString interfaceName)
{
    if (isOpen())
    {
        lastError = tr("CAN interface already connected");
        return false;
    }

    bool found = false;

    QString error;

    QStringList plugins = GetPlugins();

    for (QString plugin : plugins)
    {
        if (!pluginName.isEmpty() && (pluginName.toLower() != plugin.toLower()))
        {
            continue;
        }

        QStringList devices = GetDevices(plugin);

        /*
         * Iterate through each device provided by the interface.
         * If we are requesting a specific device string, look for a match.
         * Otherwise, try to connect to the first device we see.
         */
        for (QString device : devices)
        {
            if (!interfaceName.isEmpty() && (interfaceName.toLower() != device.toLower()))
            {
                continue;
            }

            DCDebug << "Attempting to open CAN device ->" << plugin << ":" << device;

            adapter = QCanBus::instance()->createDevice(plugin, device, &error);

            if (adapter)
            {
                // TODO
                // configureInterface(adapter)

                if (adapter->connectDevice())
                {
                    adapterName = plugin;
                    deviceName = device;
                    found = true;
                    break;
                }
                else
                {
                    delete adapter;
                    adapter = nullptr;
                }
            }
            else
            {
                if (!error.isEmpty())
                {
                    DCDebug << "Error connecting to device -" << error;
                }

                adapter = nullptr;
            }

            if (found)
            {
                break;
            }
        }
    }

    if (found)
    {
        DCDebug << "Successfully opened CAN device -" << adapterName << ":" << deviceName;

        resetCounters();

        return true;
    }
    else
    {
        adapter = nullptr;

        lastError = tr("No CAN devices found");

        DCDebug << "Could not find any CAN interfaces to open";

        return false;
    }
}


/**
 * @brief DroneCANInterface::close - Disconnect the connected CAN adapter
 * @return
 */
bool DroneCANInterface::close()
{
    bool result = false;

    if (adapter)
    {
        if (adapter->state() == QCanBusDevice::ConnectedState)
        {
            result = true;
            adapter->disconnectDevice();
        }

        delete adapter;
    }

    // Clear the pointer
    adapter = nullptr;

    // Ensure logging is halted
    stopLogging();

    return result;
}


/**
 * @brief DroneCANInterface::isOpen - Tests if the CAN interface is currently connected
 * @return
 */
bool DroneCANInterface::isOpen()
{
    return (adapter != nullptr) && (adapter->state() == QCanBusDevice::ConnectedState);
}


void DroneCANInterface::startLogging()
{
    // TODO
}


void DroneCANInterface::stopLogging()
{
    // TODO
}


void DroneCANInterface::resetCounters()
{
    rxCount = 0;
    txCount = 0;
}


/**
 * @brief DroneCANInterface::saveSettings - Save CAN interface settings to a workspace file
 * @param settings
 */
void DroneCANInterface::saveSettings(QSettings &settings)
{
    settings.beginGroup("can");

    settings.endGroup();
}


/**
 * @brief DroneCANInterface::loadSettings - Load CAN interface settings from a workspace file
 * @param settings
 */
void DroneCANInterface::loadSettings(QSettings &settings)
{
    settings.beginGroup("can");

    settings.endGroup();
}
