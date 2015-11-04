#ifndef RequiredMsgTypeDataPck_H
#define RequiredMsgTypeDataPck_H
#include "DataPckTypeBase.h"

namespace RequiredMsgTypeDataPck
{
    const char MsgSize = 50;
    
    struct RequiredMsgType : public DataPckTypeBase::DataPck
    {
        unsigned int msgType;
    };  
};
#endif