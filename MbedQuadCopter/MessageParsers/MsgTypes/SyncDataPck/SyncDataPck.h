#ifndef SyncDataPck_H
#define SyncDataPck_H
#include "DataPckTypeBase.h"

namespace SyncDataPck
{
    struct Sync : public DataPckTypeBase::DataPck
    {
        char* data;  
    };  
};
#endif