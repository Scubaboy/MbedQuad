#ifndef HMC5883L_H
#define HMC5883L_H
#include "SensorBase.h"
#include "MagStatus.h"
#include "SensorCalBase.h"
#include "I2CBusCtrl.h"
#include "MagFlightData.h"
#include "MagSensorBase.h"

class HMC5883L : public MagSensorBase
{
    public:
        HMC5883L(SensorCalBase* calObj,I2CBusCtrl* BusCtrl, MagStatus::MagStatusStruct* magStatus, MagFlightData::MagFlightDataStruct* flightData);
        virtual void InitSensor();
        virtual void GetRawSensorData();  
        virtual void  GetScaledSensorData();
        virtual void UpdateHeadingRaw(float roll, float pitch);
        virtual void UpdateHeadingScaled(float roll, float pitch);
    private:
        float GetHeading (float roll, float pitch, float mag[3]);
        bool sensorReady;
        const SensorCalBase* calObj;
        MagStatus::MagStatusStruct* magStatus;
        MagFlightData::MagFlightDataStruct* flightData;
        I2CBusCtrl* busCtrl;
};

#endif