#ifndef FlightCtrlActionDataPck_H
#define FlightCtrlActionDataPck_H
#include "DataPckTypeBase.h"
#include "HighLevelCtrlActionTypes.h"

namespace FlightCtrlActionDataPck
{
    struct FlightCtrlAction : public DataPckTypeBase::DataPck
    {
        HighLevelCtrlActionTypes::FlightAction action;
        HighLevelCtrlActionTypes::FlightActionRequest request;
    };
};

#endif