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


#ifndef DRONECAN_DEVICE_HPP
#define DRONECAN_DEVICE_HPP

#include <qstring.h>
#include <qmap.h>

#include "DroneCANProtocol.h"
#include "dronecan_system_packets.h"

#include "dronecan_packet.hpp"
#include "dronecan_id.h"
#include "dronecan_param.h"

class DroneCANNode
{
public:
    DroneCANNode();
    DroneCANNode(uint16_t vid, uint16_t pid, uint32_t sn);
    virtual ~DroneCANNode();

    QString getManufacturerString(void) const;
    QString getUserString(void) const;

    uint64_t getUID(void) const;

    uint16_t getVID(void) const { return id.vid; }
    uint16_t getPID(void) const { return id.pid; }
    uint32_t getSerialNumber(void) const { return id.serial; }

    QString getVendorString(void) const;
    QString getVendorDescription(void) const;

    QString getFirmwareVersionString(void) const;

    DroneCAN_Parameter_t getParameter(int index);
    void setParameter(int index, DroneCAN_Parameter_t param);
    int getParameterCount(void) { return parameters.count(); }

    bool decodePacket(DroneCANPacket &packet);

protected:

    // Device system information
    DroneCAN_UniqueId_t id;

    // Device manufacturer strings
    struct
    {
        //! Manufacturer string
        char manf[64];

        //! User string
        char user[64];
    } strings;

    // Device firmware information
    struct
    {
        DroneCAN_FirmwareDate_t date;
        DroneCAN_FirmwareVersion_t version;
        DroneCAN_FirmwareSettings_t settings;
    } firmware;

    // Device hardware information
    struct
    {

    } hardware;

    void initialize();

    // Map of public parameters available for this device
    QMap<int, DroneCAN_Parameter_t> parameters;
};

#endif // DRONECAN_DEVICE_HPP
