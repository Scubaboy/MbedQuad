#ifndef ShutdownDataPck_H
#define ShutdownDataPck_H
#include "DataPckTypeBase.h"

namespace ShutdownDataPck
{
    struct Shutdown : public DataPckTypeBase::DataPck
    {
        bool stop;  
    };  
};
#endif