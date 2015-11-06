#pragma once
#include "Task.h"
#include "HeartBeatMonitor.h"
#include "TimeSynchStatus.h"

class HeartBeatMonitorTask: public Task
{
public:
		HeartBeatMonitorTask( HeartBeatMonitor* heartBeatMonitor, TimeSynchStatus::TimeSynchStatusStruct* timeSynchStatus);
        virtual void ExecuteTask(int interval);
        virtual void Init();

    private:
        HeartBeatMonitor* heartBeatMonitor;
        time_t previousSendTime;
        time_t previousMonitorTime;
        TimeSynchStatus::TimeSynchStatusStruct* timeSynchStatus;
        bool startTimesRecorded;
};


