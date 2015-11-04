#ifndef PWMTestTask_H
#define PWMTestTask_H
#include "Task.h"
#include "PWMController.h"
#include "BusCtrl.h"

class PWMTestTask : public Task
{
        public:
        PWMTestTask(PWMController* pwmCtrl, BusCtrl* busCtrl);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        void GetNextChannel();
    private:
        PWMController* pwmCtrl;
        BusCtrl* busCtrl;
        bool initRun;
        bool changeDirection;
        QuadMotorTypes::MotorID motor;
};
#endif