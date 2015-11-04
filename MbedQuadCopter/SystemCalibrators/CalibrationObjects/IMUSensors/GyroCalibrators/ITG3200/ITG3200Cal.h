#ifndef ITG3200Cal_H
#define ITG3200Cal_H
#include "GyroTypes.h"
#include "SensorCalBase.h"
#include "GyroStatus.h"

class ITG3200Cal : public SensorCalBase
{
    public:
        ITG3200Cal(GyroTypes::GyroCalData *calData, GyroStatus::GyroStatusStruct* gyroStatus);
        virtual const IMUTypes::SensorChannelScales GetChannelScaleFactors() const;
        virtual const IMUTypes::SensorChannelOffsets GetChannelOffsets() const ;  
    private:
        const GyroTypes::GyroCalData* calData; 
        GyroStatus::GyroStatusStruct* gyroStatus;
        bool validCalData;
};

#endif