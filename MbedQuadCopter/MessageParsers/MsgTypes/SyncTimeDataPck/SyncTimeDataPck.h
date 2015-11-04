#pragma once
#include "DataPckTypeBase.h"

namespace SyncTimeDataPck
{
    struct SyncTimeData : public DataPckTypeBase::DataPck
    {
        int SyncSeconds;
    };  
    
};