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

#ifndef DRONECAN_TRANSFER_HPP
#define DRONECAN_TRANSFER_HPP

#include <qcanbusframe.h>

#include "dronecan_id.h"

// TODO - Move this elsewhere
#define DRONECAN_MAX_TRANSFER_SIZE 384

class DroneCANTransfer
{
    DroneCANTransfer(uint8_t txferId);

protected:

    bool encodeFrame(QCanBusFrame &frame, QString *errorMsg = nullptr);
    bool decodeFrame(QCanBusFrame &frame, QString *errorMsg = nullptr);

protected:
    //! Frame ID
    DroneCAN_ID_t frameId;

    //! Transfer ID
    uint8_t transferId = 0;

    //! Raw data buffer
    uint8_t data[DRONECAN_MAX_TRANSFER_SIZE];

    //! Transfer length
    uint16_t length = 0;

    //! Timestamp of transfer creation
    uint64_t timestamp = 0;
};

#endif // DRONECAN_TRANSFER_HPP
