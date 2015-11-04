#ifndef ReSendLastDataPckTask_H
#define ReSendLastDataPckTask_H
#include "Task.h"
#include "BaseComms.h"

class ReSendLastDataPckTask : public Task
{
        public:
        ReSendLastDataPckTask(BaseComms* comms);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        BaseComms* comms;
};
#endif