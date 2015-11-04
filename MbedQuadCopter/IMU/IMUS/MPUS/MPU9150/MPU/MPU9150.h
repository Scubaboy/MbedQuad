#ifndef MPU9150_H
#define MPU9150_H
#include "InvMpuDmpMotionDriver.h"
#include "InvMPU.h"
#include "SensorFlightData.h"
#include "IMUStatus.h"
#include "IMUBase.h"
#include "MPUBase.h"
#include "Quaternion.h"

class MPU9150 : public IMUBase
{
    public:
        MPU9150(
                MPUBase* mpu,
                DebugLoggerBase* debugLogger,
                SensorFlightData::SensorFlightDataStruct* flightData,
                IMUStatus::IMUStatusStruct* imuStatus);
                
        virtual void Init();  
        virtual void UpdateEulerAngles(float timeDelta);
    
    private:
        virtual void CallCriticalStop();
        InvMpuDmpMotionDriver* dmpDriver;
        InvMPU* mpuDriver;
        int ret;
        unsigned int updateRate;
        short gyro[3];
        short sensors;
        unsigned char fifoCount;

        IMUFlightData::IMUFlightDataStruct* flightData;
        GyroFlightData::GyroFlightDataStruct* gyroFlightData;
        AccelFlightData::AccelFlightDataStruct* accelFlightData;
        MagFlightData::MagFlightDataStruct* magFlightData;
        IMUStatus::IMUStatusStruct* imuStatus;
        DebugLoggerBase* debugLogger;
        I2CBusCtrl* busCtrl;
        MPUBase* mpu;
        Vector3f gyroRaw;
        Vector3f gyroScaled; 
        Vector3f accelRaw;
        Vector3f accelScaled;
        Vector3f magRaw;
        Vector3f magScaled;
        Vector3f eulerAngles;
        Quaternion::Quat quat;

};
#endif