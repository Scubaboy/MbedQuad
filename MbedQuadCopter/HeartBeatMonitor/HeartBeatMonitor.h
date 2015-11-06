#pragma once
#include "HeartBeatStatusStruct.h"
#include "CommsStatus.h"
#include "DebugLoggerBase.h"
#include "mbed.h"
#include "HeartBeatDataPck.h"

class FunctionPtrInternal;

class HeartBeatMonitor
{
    public:
        HeartBeatMonitor(HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus, CommsStatus::CommsStatusStruct* commsStatus, DebugLoggerBase* debugLogger);
        void MonitorHeartBeat();
        void SendHeartBeat();
    private:
        void SendRequest(HeartBeatDataPck::HeartBeatData* heartBeat);
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
    private:

        void SendToLogger(char* data);
        
    private:
        HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus;
        CommsStatus::CommsStatusStruct* commsStatus;
        Timeout heatBeatTimer;
        DebugLoggerBase* debugLogger;
          
};
