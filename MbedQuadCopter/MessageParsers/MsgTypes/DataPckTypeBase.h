#ifndef DataPckTypeBase_H
#define DataPckTypeBase_H
#include "DataPckTypes.h"
#include "SystemConfig.h"

namespace DataPckTypeBase
{    
    struct DataPck
    {
        unsigned int type; 
        int quadID;// = SystemConfig::SystemID;  
        unsigned int ackRequired;
    };  
};
#endif