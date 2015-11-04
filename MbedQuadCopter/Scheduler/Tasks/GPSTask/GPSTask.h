#ifndef GPSTask_H
#define GPSTask_H
#include "Task.h"
#include "GPSBase.h"

class GPSTask : public Task
{
    public:
        GPSTask(GPSBase* gps);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        GPSBase* gps;
        bool initRun;
};
#endif