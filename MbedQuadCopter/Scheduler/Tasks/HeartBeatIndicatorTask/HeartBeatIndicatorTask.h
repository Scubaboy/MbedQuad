#ifndef HeartBeatIndicatorTask_H
#define HeartBeatIndicatorTask_H
#include "Task.h"
#include "HeartBeatIndicator.h"


class HeartBeatIndicatorTask : public Task
{
    public:
        HeartBeatIndicatorTask(HeartBeatIndicator* heartBeat);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    
    private:
        HeartBeatIndicator* heartBeat;
};
#endif