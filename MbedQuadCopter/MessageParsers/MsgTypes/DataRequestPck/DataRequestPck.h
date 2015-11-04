#ifndef DataRequestPck_H
#define DataRequestPck_H
#include "DataPckTypeBase.h"

namespace DataRequestPck
{
    const char MsgSize = 50;
    
    struct DataRequest : public DataPckTypeBase::DataPck
    {
        bool pressureAtSealevel; 
        bool motorConfigData;
        bool RateCtrlConfigData; 
        char request[MsgSize];
    };  
};
#endif