#ifndef CheckCommsReceiveQueueTask_H
#define CheckCommsReceiveQueueTask_H
#include "Task.h"
#include "BaseComms.h"

class CheckCommsReceiveQueueTask : public Task
{
        public:
        CheckCommsReceiveQueueTask(BaseComms* comms);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        BaseComms* comms;
};
#endif