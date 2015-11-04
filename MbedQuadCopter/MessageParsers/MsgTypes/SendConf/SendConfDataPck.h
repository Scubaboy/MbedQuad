#ifndef SendConf_H
#define SendConf_H
#include "DataPckTypeBase.h"

namespace SendConfDataPck
{
    struct SendConf : public DataPckTypeBase::DataPck
    {
        bool reSend;  
    };  
};
#endif