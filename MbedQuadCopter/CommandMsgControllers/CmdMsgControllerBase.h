#ifndef CmdMsgControllerBase_H
#define CmdMsgControllerBase_H

class FunctionPtrInternal;

class CmdMsgControllerBase
{
    public:
        virtual void CheckForCmdMsg() = 0;  
    private:
        virtual void ParseDataCmd() = 0;
        virtual void ActionReceivedDataPck() = 0;
};
#endif