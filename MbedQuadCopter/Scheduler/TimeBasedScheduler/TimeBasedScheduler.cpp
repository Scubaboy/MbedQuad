#include "TimeBasedScheduler.h"
#include "mbed.h"

TimeBasedScheduler::TimeBasedScheduler(SystemTaskModeControllerBase* modeController)
{
    this->modeController = modeController;
    this->ClearTaskList();
}

bool TimeBasedScheduler::AddTask(SchedulerTypes::SchedulerItem* taskToAdd)
{
    bool addResult = false;
    
    if (taskToAdd != NULL && this->nextTaskAddLocation < SchedulerTypes::TotalAllowedTasks)
    {
        this->taskList[this->nextTaskAddLocation] = taskToAdd;
        this->nextTaskAddLocation++;
        addResult = true;
    }
    
    return addResult;       
}

bool TimeBasedScheduler::RemoveTask(SchedulerTypes::Tasks taskToRemove)
{
    bool removeResult = false;
    int listIter =0 ;
    
    while (listIter < SchedulerTypes::TotalAllowedTasks && !removeResult)
    {
        if (this->taskList[listIter] != NULL)
        {
            if (this->taskList[listIter]->task == taskToRemove)
            {
                 this->taskList[listIter] = NULL;
                 removeResult = true;  
            }
        }
        
        listIter++;
    }
    
    return removeResult;
}

void TimeBasedScheduler::UpdateTaskExecution(int millis)
{
    if (this->modeController != NULL)
    {
        for (int listIter = 0; listIter < SchedulerTypes::TotalAllowedTasks; listIter++)
        {
            if (this->taskList[listIter] != NULL)
            {
                if ((millis - this->taskList[listIter]->lastUpdatedMS)  >=  this->taskList[listIter]->updateIntervalMS)
                {
                    if (this->modeController->CanTaskExecute(this->taskList[listIter]->task))
                    {
                        this->taskList[listIter]->taskToExecute->ExecuteTask(millis - this->taskList[listIter]->lastUpdatedMS);
                        this->taskList[listIter]->lastUpdatedMS = millis;
                    }
                }
            }
        }
    }
}

void TimeBasedScheduler::ResetStartCounters()
{
    for (int listIter = 0; listIter < SchedulerTypes::TotalAllowedTasks; listIter++)
    {
        if (this->taskList[listIter] != NULL)
        {
            this->taskList[listIter]->lastUpdatedMS = this->taskList[listIter]->updateIntervalMS;
        }
    }
}

void TimeBasedScheduler::ClearTaskList()
{
    for (int listIter = 0; listIter < SchedulerTypes::TotalAllowedTasks; listIter++)
    {
        this->taskList[listIter] = NULL;
    }
    
    this->nextTaskAddLocation = 0;
}