#ifndef StabCtrlDataPck_H
#define StabCtrlDataPck_H
#include "DataPckTypeBase.h"

namespace StabCtrlDataPck
{
    struct StabCtrl : public DataPckTypeBase::DataPck
    {
        float proportionalGain; 
    };  
};
#endif