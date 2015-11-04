#ifndef ConfigCalCompleteDataPck_H
#define ConfigCalCompleteDataPck_H
#include "DataPckTypeBase.h"

namespace ConfigCalCompleteDataPck
{
    struct ConfigCalComplete : public DataPckTypeBase::DataPck
    {
        bool complete;  
    };  
};
#endif