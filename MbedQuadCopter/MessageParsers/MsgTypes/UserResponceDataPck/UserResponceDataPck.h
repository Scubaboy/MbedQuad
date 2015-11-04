#ifndef UserResponceDatPck_H
#define UserResponceDataPck_H
#include "DataPckTypeBase.h"

namespace UserResponceDataPck
{
    struct UserResponceData : public DataPckTypeBase::DataPck
    {
        bool requestAck;  
    };  
};
#endif