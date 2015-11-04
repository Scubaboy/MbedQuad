#ifndef IMUTask_H
#define IMUTask_H
#include "Task.h"
#include "IMUBase.h"
#include "SensorBase.h"
#include "BusCtrl.h"
#include "IMUFlightData.h"
#include "MagSensorBase.h"

class IMUTask : public Task
{
    public:
        IMUTask(IMUBase* imu, 
        //SensorBase* gyro, 
        //SensorBase* accel, 
        //MagSensorBase* mag, 
        BusCtrl* busCtrl, IMUFlightData::IMUFlightDataStruct* imuFlightData);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        IMUBase* imu;    
        //SensorBase* gyro;
        //SensorBase* accel;
        //MagSensorBase* mag;
        BusCtrl* busCtrl;
        IMUFlightData::IMUFlightDataStruct* imuFlightData;
        bool initRun;
};
#endif