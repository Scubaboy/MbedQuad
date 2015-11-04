#pragma once
#include "HeartBeatStatusStruct.h"
#include "SystemModeControllerBase.h"
#include "DebugLoggerBase.h"
#include "mbed.h"
#include "HeartBeatDataPck.h"

class FunctionPtrInternal;

class HeartBeatMonitor
{
    public:
        HeartBeatMonitor(HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus, SystemModeControllerBase* modeController, DebugLoggerBase* debugLogger);
        void StartMonitor();
        void StartSend();
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
        void MonitorHeartBeat();
        void SendHeartBeat();
        void SendToLogger(char* data);
        
    private:
        HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus;
        SystemModeControllerBase* modeController;
        Timeout heatBeatTimer;
        DebugLoggerBase* debugLogger;
          
};