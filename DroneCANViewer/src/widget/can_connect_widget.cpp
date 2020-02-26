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

#include "can_connect_widget.hpp"

#include <qcanbus.h>
#include "adapter.hpp"


CANConnectDialog::CANConnectDialog(QWidget *parent) : QDialog(parent)
{
    ui.setupUi(this);

    setWindowTitle(tr("Connect CAN"));

    setWindowModality(Qt::ApplicationModal);

    connect(ui.cancel, SIGNAL(released()), this, SLOT(reject()));
    connect(ui.connect, SIGNAL(released()), this, SLOT(accept()));

    connect(ui.refreshDevices, SIGNAL(released()), this, SLOT(refreshDrivers()));
    connect(ui.driverSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(driverSelected(int)));

    // Add some baud rates

    ui.bitrateSelect->addItem("1000");

    refreshDrivers();
}


QString CANConnectDialog::getDeviceName()
{
    return ui.deviceSelect->currentText();
}


QString CANConnectDialog::getDriverName()
{
    return ui.driverSelect->currentText();
}


void CANConnectDialog::refreshDrivers()
{
    QStringList plugins = DroneCANInterface::GetPlugins();

    ui.driverSelect->clear();

    ui.driverSelect->addItem(tr("No driver selected..."));

    ui.driverSelect->setCurrentIndex(0);

    for (QString plugin : plugins)
    {
        auto devices = DroneCANInterface::GetDevices(plugin);

        // Only add the driver if devices are available
        if (!devices.isEmpty())
        {
            ui.driverSelect->addItem(plugin);
        }
    }

    updateConnectButton();
}


void CANConnectDialog::driverSelected(int idx)
{
    ui.deviceSelect->clear();

    if (idx == 0)
    {
        ui.deviceSelect->addItem(tr("Select CAN driver"));
        ui.deviceSelect->setEnabled(false);
    }
    else
    {
        ui.deviceSelect->setEnabled(true);

        QString driver = ui.driverSelect->currentText();

        QStringList devices = DroneCANInterface::GetDevices(driver);

        for (auto dev : devices)
        {
            ui.deviceSelect->addItem(dev);
        }
    }

    updateConnectButton();
}


void CANConnectDialog::updateConnectButton()
{
    bool enable = true;

    if (ui.driverSelect->currentIndex() == 0)
    {
        enable = false;
    }

    if (ui.deviceSelect->count() == 0)
    {
        enable = false;
    }

    if (!ui.deviceSelect->isEnabled())
    {
        enable = false;
    }

    ui.connect->setEnabled(enable);
}
