#ifndef CmdReaderTask_H
#define CmdReaderTask_H
#include "Task.h"
#include "CmdMsgControllerBase.h"

class CmdReaderTask : public Task
{
    public:
        CmdReaderTask(CmdMsgControllerBase* cmdReader);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        CmdMsgControllerBase* cmdReader;
};
#endif