#ifndef SystemStatusAckDataPck_H
#define SystemStatusAckDataPck_H
#include "DataPckTypeBase.h"
#include "SystemConfigurationTypes.h"

namespace SystemStatusAckDataPck
{
    struct SystemStatusAck : public DataPckTypeBase::DataPck
    {
        bool ack;
    };  
};
#endif