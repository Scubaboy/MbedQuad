#ifndef WireLessDataLogger_H
#define WireLessDataLogger_H
#include "DebugLoggerBase.h"
#include "DataLoggerDataPck.h"
#include "mbed.h"

class FunctionPtrInternal;

class WireLessDataLogger : public DebugLoggerBase
{
    public:
        WireLessDataLogger();
        virtual void Log(char* debugMsg);
        virtual void EnableDisableLogger(bool enable);
    public:
        template <class ClassT>
        void SendDebugMsg(ClassT *ptr, bool(ClassT::*pFunc)(char* data, const int DataLength, bool confRequired))
        {
                this->sendDebugptr = (FunctionPtrInternal*)ptr;  
                this->methodCallbackSendDebugMsg = (bool (FunctionPtrInternal::*)(char* data, const int DataLength, bool confRequired))pFunc; 
        }
    private:
        void SendDebugMsg(DataLoggerDataPck::DataLogger* dataLogger);
    private:
        FunctionPtrInternal* sendDebugptr;
        bool (FunctionPtrInternal::*methodCallbackSendDebugMsg)(char* data, const int DataLength, bool confRequired);
        bool enable;  
        DataLoggerDataPck::DataLogger dataLoggerDebugMsgPck; 
};
#endif