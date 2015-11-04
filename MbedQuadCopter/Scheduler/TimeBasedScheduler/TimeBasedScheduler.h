#ifndef TimeBasedScheduler_H
#define TimeBasedScheduler_H
#include "SchedulerBase.h"
#include "SystemTaskModeControllerBase.h"

class TimeBasedScheduler : public SchedulerBase
{
    public:
        TimeBasedScheduler(SystemTaskModeControllerBase* modeController);
        virtual bool AddTask(SchedulerTypes::SchedulerItem* taskToAdd);
        virtual bool RemoveTask(SchedulerTypes::Tasks taskToRemove);
        virtual void UpdateTaskExecution(int millis);
        virtual void ResetStartCounters();
    private:
        void ClearTaskList();
    private:
        SystemTaskModeControllerBase* modeController;
        SchedulerTypes::SchedulerItem* taskList[SchedulerTypes::TotalAllowedTasks];
        int nextTaskAddLocation;
};
#endif