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

#include "DroneCANProtocol.h"

#include "dronecan_packet.hpp"


DroneCANPacket::DroneCANPacket()
{
    memset(data, 0, sizeof(data));
}


//! \return the packet data pointer from the packet
uint8_t* getDroneCANPacketData(void* pkt)
{
    DroneCANPacket* ptr = (DroneCANPacket*) pkt;

    return ptr->data;
}

//! \return the packet data pointer from the packet, const
const uint8_t* getDroneCANPacketDataConst(const void* pkt)
{
    DroneCANPacket* ptr = (DroneCANPacket*) pkt;

    return (const uint8_t*) ptr->data;
}

//! Complete a packet after the data have been encoded
void finishDroneCANPacket(void* pkt, int size, uint32_t packetID)
{
    DroneCANPacket* ptr = (DroneCANPacket*) pkt;

    ptr->id = packetID;
    ptr->length = size;
}


//! \return the size of a packet from the packet header
int getDroneCANPacketSize(const void* pkt)
{
    DroneCANPacket* ptr = (DroneCANPacket*) pkt;

    return ptr->length;
}

//! \return the ID of a packet from the packet header
uint32_t getDroneCANPacketID(const void* pkt)
{
    DroneCANPacket* ptr = (DroneCANPacket*) pkt;

    return ptr->id;
}
