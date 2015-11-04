#ifndef DataRequestReceivedPck_H
#define DataRequestReceivedPck_H
#include "DataPckTypeBase.h"

namespace DataRequestReceivedPck
{
    struct DataRequestReceived : public DataPckTypeBase::DataPck
    {
        float pressureAtSealevel;
        bool pressureAtSeaLevelSet;  
    };  
};
#endif