#ifndef LowLevelFlightControllerTask_H
#define LowLevelFlightControllerTask_H
#include "Task.h"
#include "BasicLowLevelRateController.h"

class LowLevelFlightControllerTask : public Task
{
    public:
        LowLevelFlightControllerTask(BasicLowLevelRateController* lowLevelController);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        BasicLowLevelRateController* lowLevelController;
        bool initRun;

};
#endif