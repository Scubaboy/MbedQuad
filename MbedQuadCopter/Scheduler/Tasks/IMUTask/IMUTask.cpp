#include "IMUTask.h"
#include "mbed.h"
#include "AngleConverters.h"
#include "MagSensorBase.h"

const float MilliSecondsInSecond = 1000.0f;
const float AllowTimeDelta = 0.20f;

IMUTask::IMUTask(IMUBase* imu, 
                 //SensorBase* gyro, 
                 //SensorBase* accel, 
                 //MagSensorBase* mag, 
                 BusCtrl* busCtrl, 
                 IMUFlightData::IMUFlightDataStruct* imuFlightData)
{
    this->initRun = false;
    this->imu = imu; 
    //this->gyro = gyro;
    //this->accel = accel;
    //this->mag = mag;   
    this->busCtrl = busCtrl;
    this->imuFlightData = imuFlightData;
}

void IMUTask::Init()
{   
    this->initRun = true;
}

void IMUTask::ExecuteTask(int interval)
{
    if (this->imu != NULL)// && this->gyro != NULL && this->accel != NULL && this->mag != NULL)
    {
        if (!this->initRun)
        {
            this->Init();
        }
        
        float timeDelta = (float)interval/(float)MilliSecondsInSecond;
        
        if (timeDelta <= AllowTimeDelta)
        {
            //this->busCtrl->TakeCtrl(BusCtrlUsers::IMU);
                //this->gyro->GetScaledSensorData();
                //this->accel->GetScaledSensorData();
                //this->mag->UpdateHeadingScaled(AngleConverters::DegreesToRadians(this->imuFlightData->rollAngle),
                //                               AngleConverters::DegreesToRadians(this->imuFlightData->pitchAngle));
                this->imu->UpdateEulerAngles(timeDelta);
            //this->busCtrl->ReleaseCtrl(BusCtrlUsers::IMU);
        }
    }
}