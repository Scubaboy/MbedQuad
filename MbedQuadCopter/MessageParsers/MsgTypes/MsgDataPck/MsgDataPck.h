#ifndef MsgDataPck_H
#define MsgDataPck_H
#include "DataPckTypeBase.h"

namespace MsgDataPck
{
    const char MsgSize = 50;
    
    struct MsgData : public DataPckTypeBase::DataPck
    {
        char msg[MsgSize];  
    };  
    
    struct MsgData2
    {
        unsigned int type; 
        int quadID;
        char msg[2];  
    };
};
#endif