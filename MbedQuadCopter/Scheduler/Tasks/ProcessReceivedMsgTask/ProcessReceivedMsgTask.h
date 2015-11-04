#ifndef ProcessReceivedMsgTask_H
#define ProcessReceivedMsgTask_H
#include "Task.h"
#include "BaseComms.h"

class ProcessReceivedMsgTask : public Task
{
        public:
        ProcessReceivedMsgTask(BaseComms* comms);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        BaseComms* comms;
};
#endif