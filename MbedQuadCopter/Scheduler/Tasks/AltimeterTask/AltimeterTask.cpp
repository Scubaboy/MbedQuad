#include "AltimeterTask.h"
#include "mbed.h"
#include "BusUsers.h"

AltimeterTask::AltimeterTask(AltiMeterBase* altimeter,BusCtrl* busCtrl)
{
    this->altimeter = altimeter;  
    this->busCtrl = busCtrl;
}

void AltimeterTask::Init()
{
    
}
void AltimeterTask::ExecuteTask(int interval)
{
    if (this->altimeter != NULL && this->busCtrl != NULL)
    { 
        this->altimeter->ReadSensor();
        this->altimeter->UpdatePresure();
        this->altimeter->UpdateTemperature();
        this->altimeter->UpdateAltitude(); 
    }
}