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

#include "dronecan_transfer.hpp"


DroneCANTransfer::DroneCANTransfer(uint8_t txferId)
{
    memset(data, 0, DRONECAN_MAX_TRANSFER_SIZE);

    length = 0;

    transferId = txferId;
}


/**
 * @brief DroneCANTransfer::encodeFrame - encode transfer into a QCanBusFrame object
 * @param frame - reference to the frame
 * @return true if there is a frame to send (false if the transfer is complete)
 */
bool DroneCANTransfer::encodeFrame(QCanBusFrame &frame, QString *errorMsg)
{
    // DroneCAN only uses 29-bit identifiers
    frame.setExtendedFrameFormat(true);

    frame.setBitrateSwitch(false);
    frame.setFlexibleDataRateFormat(false);

    // Data frame
    frame.setFrameType(QCanBusFrame::FrameType::DataFrame);

    // Single-frame transfer
    if (length <= 8)
    {
        QByteArray bytes((const char*) data, length);

        // Encode the frame ID (ensure multi-frame is set to zero)
        frameId.multiframe = 0;
        frame.setFrameId(DroneCAN_encodeID(&frameId));

        frame.setPayload(bytes);

        return true;
    }
    // Multi-frame transfer
    else
    {
        return false;
    }
}



bool DroneCANTransfer::decodeFrame(QCanBusFrame &frame, QString *errorMsg)
{
    // DroneCAN uses 29-bit identifiers
    if (!frame.hasErrorStateIndicator())
    {
        return false;
    }

    DroneCAN_ID_t frameId;

    DroneCAN_decodeID(frame.frameId(), &frameId);

    if (frameId.multiframe)
    {
        // TODO - Support decode of multiframe messages
        return false;
    }
    else
    {

    }
}


