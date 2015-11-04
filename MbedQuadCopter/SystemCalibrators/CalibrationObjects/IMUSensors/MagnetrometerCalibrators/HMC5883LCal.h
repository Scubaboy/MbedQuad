#ifndef HMC5883LCal_H
#define HMC5883LCal_H
#include "XYZAxisMinMax.h"
#include "SensorCalBase.h"
#include "MagStatus.h"

class HMC5883LCal : public SensorCalBase
{
    public:
        HMC5883LCal(XYZAxisMinMax::XYZAxisMinMaxCalData *calData, MagStatus::MagStatusStruct* magStatus);
        virtual const IMUTypes::SensorChannelScales GetChannelScaleFactors() const;
        virtual const IMUTypes::SensorChannelOffsets GetChannelOffsets() const ;  
    private:
        const XYZAxisMinMax::XYZAxisMinMaxCalData* calData; 
        MagStatus::MagStatusStruct* magStatus;
        bool validCalData;
};

#endif