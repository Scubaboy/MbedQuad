#ifndef FlightPackDataPck_H
#define FlightPackDataPck_H
#include "DataPckTypeBase.h"

namespace FlightPackDataPck
{
    struct FlightPack : public DataPckTypeBase::DataPck
    {
        float cell1VoltageLoss;
        float cell1_2VoltageLoss;
        float cell1_3VoltageLoss;
    };  
};
#endif