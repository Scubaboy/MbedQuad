#ifndef DataPckParserBase_H
#define DataPckParserBase_H
#include "DataPckTypeBase.h"
#include "ReceiveDataPacket.h"

class DataPckParserBase
{
    public:
        virtual DataPckTypeBase::DataPck* Parse(ReceiveDataPacket::ReceiveDataPacketStruct* dataPck) = 0; 
};

#endif