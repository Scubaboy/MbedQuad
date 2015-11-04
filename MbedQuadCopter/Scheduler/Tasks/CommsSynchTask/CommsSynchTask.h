#ifndef CommsSynchTask_H
#define CommsSynchTask_H
#include "Task.h"
#include "SystemIdDataPck.h"
#include "SystemStatusAck.h"
#include "BaseComms.h"
#include "SystemModeControllerBase.h"

class FunctionPtrInternal;

class CommsSynchTask : public Task 
{
    public:
        CommsSynchTask(BaseComms* sysComms, SystemModeControllerBase* systemModeController);
        virtual void ExecuteTask(int interval);
        virtual void Init(); 
    private:
        BaseComms* sysComms;
        SystemModeControllerBase* systemModeController;
        bool waitingSynch;
        FlightMode::Modes mode;
};
#endif