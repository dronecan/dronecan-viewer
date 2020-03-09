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

#include "dronecan_node.hpp"

#include "debug.hpp"


DroneCANNode::DroneCANNode()
{
    initialize();

    DCDebug << "Creating new DroneCAN device";
}


DroneCANNode::DroneCANNode(uint16_t vid, uint16_t pid, uint32_t sn)
{
    initialize();

    id.vid = vid;
    id.pid = pid;
    id.serial = sn;

    DCDebug << "Creating new DroneCAN device";
}


DroneCANNode::~DroneCANNode()
{
    DCDebug << "Deleting DroneCAN device";
}


void DroneCANNode::initialize()
{
    memset(&id, 0, sizeof(id));
    memset(&firmware, 0, sizeof(firmware));
    memset(&strings, 0, sizeof(strings));
}



QString DroneCANNode::getManufacturerString() const
{
    return QString(QByteArray((const char*) strings.manf, 64));
}


QString DroneCANNode::getUserString() const
{
    return QString(QByteArray((const char*) strings.user, 64));
}


/**
 * @brief DroneCANNode::getUID - Return a unique identifier for this device
 * @return
 */
uint64_t DroneCANNode::getUID() const
{
    uint64_t uid = 0;

    uid |= ((uint64_t) id.vid << 48);                  // 16-bit vendor ID
    uid |= ((uint64_t) id.pid << 32);                  // 16-bit product ID
    uid |= ((uint64_t) id.serial & 0xFFFFFF00);        // 32-bit serial number (masked as 24-bits)

    return uid;
}


QString DroneCANNode::getVendorString() const
{
    QString s = DroneCAN_Vendors_EnumLabel(id.vid);

    return s;
}


QString DroneCANNode::getVendorDescription() const
{
    QString s = DroneCAN_Vendors_EnumTitle(id.vid);

    return s;
}


QString DroneCANNode::getFirmwareVersionString() const
{
    QStringList v;

    v << QString::number(firmware.version.versionMajor);
    v << QString::number(firmware.version.versionMinor);
    v << QString::number(firmware.version.versionSub);

    return v.join(".");
}


DroneCAN_Parameter_t DroneCANNode::getParameter(int index)
{
    if (parameters.contains(index))
    {
        return parameters[index];
    }

    // Not found - return a null parameter

    DroneCAN_Parameter_t param;

    memset(&param, 0, sizeof(param));

    param.format = DRONECAN_PARAMETER_NULL;

    return param;
}


void DroneCANNode::setParameter(int index, DroneCAN_Parameter_t param)
{
    parameters[index] = param;
}
