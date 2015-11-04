#ifndef ManualFlightDataDataPck_H
#define ManualFlightDataDataPck_H

namespace ManualFlightDataDataPck
{
    struct ManualFlightData : public DataPckTypeBase::DataPck
    {
        float requestedRoll;
        float requestedPitch;
        float requestedYaw;
        float requestedThrottle;
    };
};
#endif