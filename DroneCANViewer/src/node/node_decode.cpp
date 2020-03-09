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


#include "dronecan_node.hpp"

#include "DroneCANProtocol.h"

#include "dronecan_system_packets.h"
#include "dronecan_parameter_packets.h"


/**
 * @brief DroneCANNode::decodePacket - Decode a packet from a given node
 * At this stage it has been determined that the given packet originated from the given node
 * @param packet
 * @return
 */
bool DroneCANNode::decodePacket(DroneCANPacket &packet)
{
    bool valid = true;

    uint16_t paramIdx = 0;

    DroneCAN_Parameter_t param;

    // Device status
    if (decodeDroneCAN_DeviceStatusPacket(&packet))
    {
        // TODO
    }

    // Decode device ID strings
    else if (decodeDroneCAN_ManufacturerStringPacket(&packet, strings.manf)) {}
    else if (decodeDroneCAN_UserStringPacket(&packet, strings.user)) {}

    // Decode device firmware information packets
    else if (decodeDroneCAN_FirmwareDatePacketStructure(&packet, &firmware.date)) {}
    else if (decodeDroneCAN_FirmwareVersionPacketStructure(&packet, &firmware.version)) {}
    else if (decodeDroneCAN_FirmwareSettingsPacketStructure(&packet, &firmware.settings)) {}

    // Node parameter data
    else if (decodeDroneCAN_ParameterInfoPacket(&packet, &paramIdx, &param.format, param.name))
    {
        // TODO - What now?
    }
    else if (decodeDroneCAN_ParameterNullPacket(&packet, &paramIdx))
    {
    }
    else if (decodeDroneCAN_ParameterValueBytesPacket(&packet, &paramIdx, param.value_bytes))
    {
    }
    else if (decodeDroneCAN_ParameterValueFloatPacket(&packet, &paramIdx, &param.value_float))
    {
    }
    else if (decodeDroneCAN_ParameterValueS8Packet(&packet, &paramIdx, &param.value_sint8))
    {
    }
    else if (decodeDroneCAN_ParameterValueS16Packet(&packet, &paramIdx, &param.value_sint16))
    {
    }
    else if (decodeDroneCAN_ParameterValueS32Packet(&packet, &paramIdx, &param.value_sint32))
    {
    }
    else if (decodeDroneCAN_ParameterValueU8Packet(&packet, &paramIdx, &param.value_uint8))
    {
    }
    else if (decodeDroneCAN_ParameterValueU16Packet(&packet, &paramIdx, &param.value_uint16))
    {
    }
    else if (decodeDroneCAN_ParameterValueU32Packet(&packet, &paramIdx, &param.value_uint32))
    {
    }

    // No valid packet was able to be decoded
    else
    {
        valid = false;
    }

    if (valid)
    {
        // TODO - Validate the given packet type
    }

    return valid;
}
