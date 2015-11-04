#ifndef SystemStartTask_H
#define SystemStartTask_H
#include "Task.h"
#include "SystemIdDataPck.h"
#include "SystemStatusAck.h"

class FunctionPtrInternal;

class SystemStartTask : public Task 
{
    public:
        SystemStartTask(SystemStatusAck::SystemStatusAckStruct* systemStatusAck);
        virtual void ExecuteTask(int interval);
        virtual void Init(); 
    private:
        void SendRequest(SystemIdDataPck::SystemId* systemId);
    public:
        template <class ClassT>
        void SendRequest(ClassT *ptr, bool(ClassT::*pFunc)(char* data, const int DataLength, bool confRequired))
        {
                this->sendRequestrptr = (FunctionPtrInternal*)ptr;  
                this->methodCallbackSendRequest = (bool (FunctionPtrInternal::*)(char* data, const int DataLength, bool confRequired))pFunc; 
        }
    private:
        FunctionPtrInternal* sendRequestrptr;
        bool (FunctionPtrInternal::*methodCallbackSendRequest)(char* data, const int DataLength, bool confRequired);
       bool sentSystemId;
       bool msgPosted;
       SystemStatusAck::SystemStatusAckStruct* systemStatusAck;
};
#endif