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

#include <memory.h>

#include "dronecan_device.hpp"

#include "debug.hpp"


DroneCANDevice::DroneCANDevice()
{
    initialize();

    DCDebug << "Creating new DroneCAN device";
}


DroneCANDevice::DroneCANDevice(uint16_t vid, uint16_t pid, uint32_t sn)
{
    initialize();

    id.vid = vid;
    id.pid = pid;
    id.serial = sn;

    DCDebug << "Creating new DroneCAN device";
}


DroneCANDevice::~DroneCANDevice()
{
    DCDebug << "Deleting DroneCAN device";
}


void DroneCANDevice::initialize()
{
    memset(&id, 0, sizeof(id));
    memset(&firmware, 0, sizeof(firmware));
    memset(&strings, 0, sizeof(strings));
}



QString DroneCANDevice::getManufacturerString() const
{
    return QString(QByteArray((const char*) strings.manf, 64));
}


QString DroneCANDevice::getUserString() const
{
    return QString(QByteArray((const char*) strings.user, 64));
}


/**
 * @brief DroneCANDevice::getUID - Return a unique identifier for this device
 * @return
 */
uint64_t DroneCANDevice::getUID() const
{
    uint64_t uid = 0;

    uid |= ((uint64_t) id.vid << 48);                  // 16-bit vendor ID
    uid |= ((uint64_t) id.pid << 32);                  // 16-bit product ID
    uid |= ((uint64_t) id.serial & 0xFFFFFF00);        // 32-bit serial number (masked as 24-bits)

    return uid;
}


QString DroneCANDevice::getVendorString() const
{
    QString s = DroneCAN_Vendors_EnumLabel(id.vid);

    return s;
}


QString DroneCANDevice::getVendorDescription() const
{
    QString s = DroneCAN_Vendors_EnumTitle(id.vid);

    return s;
}
