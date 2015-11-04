#ifndef SystemStatusLoggerTask_H
#define SystemStatusLoggerTask_H
#include "Task.h"
#include "EncodersDecodersBase.h"

class SystemStatusLoggerTask : public Task
{
    public:
        SystemStatusLoggerTask(EncodersDecodersBase* encoder);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        EncodersDecodersBase* encoder;
};
#endif