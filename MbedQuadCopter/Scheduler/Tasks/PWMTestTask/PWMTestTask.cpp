#include "PWMTestTask.h"
#include "mbed.h"
#include "QuadMotorTypes.h"

PWMTestTask::PWMTestTask(PWMController* pwmCtrl, BusCtrl* busCtrl)
{
    this->pwmCtrl = pwmCtrl;
    this->busCtrl = busCtrl;  
    this->initRun = false;
    this->changeDirection = false;
}


void PWMTestTask::ExecuteTask(int interval)
{
    if (this->pwmCtrl != NULL && this->busCtrl != NULL)
    {
        if (!this->initRun)
        {
            this->Init();   
        }
        
        this->busCtrl->TakeCtrl(BusCtrlUsers::PWMCtrl);
            if (this->changeDirection)
            {
               this->pwmCtrl->SetPWM(this->motor,500); 
            }
            else
            {
                this->pwmCtrl->SetPWM(this->motor,200);
                this->GetNextChannel();
            }
            
            this->changeDirection = !this->changeDirection;
            
        this->busCtrl->ReleaseCtrl(BusCtrlUsers::PWMCtrl);
    }
}

void PWMTestTask::GetNextChannel()
{
    switch (this->motor)
    {
        case QuadMotorTypes::Motor1:
        {
            this->motor = QuadMotorTypes::Motor2;
            break;   
        } 
        case QuadMotorTypes::Motor2:
        {
            this->motor = QuadMotorTypes::Motor3;
            break;   
        }
        case QuadMotorTypes::Motor3:
        {
            this->motor = QuadMotorTypes::Motor4;
            break;   
        }
        case QuadMotorTypes::Motor4:
        {
            this->motor = QuadMotorTypes::Motor1;
            break;   
        }
          
    }
}

void PWMTestTask::Init()
{
    if (this->pwmCtrl != NULL && this->busCtrl != NULL)
    { 
        this->busCtrl->TakeCtrl(BusCtrlUsers::PWMCtrl);
            this->pwmCtrl->Reset();
            this->pwmCtrl->SetPWMFreq(60.0);
        this->busCtrl->ReleaseCtrl(BusCtrlUsers::PWMCtrl);
    }
    
    this->motor = QuadMotorTypes::Motor1;
    this->initRun = true;
}