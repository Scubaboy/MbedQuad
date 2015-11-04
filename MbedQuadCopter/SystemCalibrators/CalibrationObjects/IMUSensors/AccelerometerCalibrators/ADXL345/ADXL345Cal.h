#ifndef ADXL345Cal_H
#define ADXL345Cal_H
#include "XYZAxisMinMax.h"
#include "SensorCalBase.h"
#include "AccelStatus.h"

class ADXL345Cal : public SensorCalBase
{
    public:
        ADXL345Cal(XYZAxisMinMax::XYZAxisMinMaxCalData *calData, AccelStatus::AccelStatusStruct* accelStatus);
        virtual const IMUTypes::SensorChannelScales GetChannelScaleFactors() const;
        virtual const IMUTypes::SensorChannelOffsets GetChannelOffsets() const ;  
    private:
        const XYZAxisMinMax::XYZAxisMinMaxCalData* calData; 
        AccelStatus::AccelStatusStruct* accelStatus;
        bool validCalData;
};

#endif