#pragma once
#include "Task.h"
#include "HeartBeatMonitor.h"

class HeartBeatSendTask : public Task
{
    public:
		HeartBeatSendTask( HeartBeatMonitor* heartBeatMonitor);
        virtual void ExecuteTask(int interval);
        virtual void Init();

    private:
        HeartBeatMonitor* heartBeatMonitor;
};
