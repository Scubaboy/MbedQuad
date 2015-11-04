#ifndef HighLevelFlightControllerTask_H
#define HighLevelFlightControllerTask_H
#include "Task.h"
#include "BaseHighLevelRateController.h"

class HighLevelFlightControllerTask : public Task
{
    public:
        HighLevelFlightControllerTask(BaseHighLevelRateController* HighLevelController);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        BaseHighLevelRateController* HighLevelController;
        bool initRun;

};
#endif