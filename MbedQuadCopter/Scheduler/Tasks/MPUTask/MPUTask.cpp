#include "MPUTask.h"
#include "mbed.h"

MPUTask::MPUTask(IMUBase* mpu, BusCtrl* busCtrl)
{
     this->mpu = mpu;
     this->busCtrl = busCtrl;
     this->initRun = false;    
}

void MPUTask::ExecuteTask(int interval)
{
    if (this->mpu != NULL && this->busCtrl != NULL)
    {
       // this->busCtrl->TakeCtrl(BusCtrlUsers::IMU);
            
            if (!this->initRun)
            {
                this->Init();
            }
            
            this->mpu->UpdateEulerAngles(100);
            
        //this->busCtrl->ReleaseCtrl(BusCtrlUsers::IMU);  
    }
}


void MPUTask::Init()
{
    this->mpu->Init();
    this->initRun = true;  
}