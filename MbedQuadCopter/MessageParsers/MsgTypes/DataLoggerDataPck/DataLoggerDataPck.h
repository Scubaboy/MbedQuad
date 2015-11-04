#ifndef DataLoggerDataPck_H
#define DataLoggerDataPck_H
#include "DataPckTypeBase.h"

namespace DataLoggerDataPck
{
    const char DebugMsgSize = 92;
    
    struct DataLogger : public DataPckTypeBase::DataPck
    {
        char debugMsg[DebugMsgSize];
        bool enableDisable;
    };  
};
#endif