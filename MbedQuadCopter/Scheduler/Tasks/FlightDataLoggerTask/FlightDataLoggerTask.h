#ifndef FlightDataLoggerTask_H
#define FlightDataLoggerTask_H
#include "Task.h"
#include "EncodersDecodersBase.h"

class FlightDataLoggerTask : public Task
{
    public:
        FlightDataLoggerTask(EncodersDecodersBase* encoder);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        EncodersDecodersBase* encoder;
};
#endif