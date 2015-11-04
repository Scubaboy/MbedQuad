#ifndef SchedulerBase_H
#define SchedulerBase_H
#include "SchedulerTypes.h"

class SchedulerBase
{
    public:
        virtual bool AddTask(SchedulerTypes::SchedulerItem* taskToAdd) = 0;
        virtual bool RemoveTask(SchedulerTypes::Tasks taskToRemove) = 0;
        virtual void UpdateTaskExecution(int millis) = 0;  
        virtual void ResetStartCounters() = 0;
};
#endif