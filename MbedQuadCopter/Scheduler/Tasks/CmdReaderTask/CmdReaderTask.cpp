#include "CmdReaderTask.h"
#include "mbed.h"

CmdReaderTask::CmdReaderTask(CmdMsgControllerBase* cmdReader)
{
    this->cmdReader = cmdReader;  
}
     
void CmdReaderTask::Init()
{
    
}
   
void CmdReaderTask::ExecuteTask(int interval)
{
    if (this->cmdReader != NULL)
    {
        this->cmdReader->CheckForCmdMsg();
    }
}