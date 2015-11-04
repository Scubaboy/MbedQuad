#ifndef ReceiveDataPacket_H
#define ReceiveDataPacket_H
#include "CommTypes.h"

namespace ReceiveDataPacket
{

    typedef struct ReceiveDataPacketStruct
    {
        char dataPack[CommTypes::DataPacketSize];
    } ReceiveDataPacketStruct;
};
#endif