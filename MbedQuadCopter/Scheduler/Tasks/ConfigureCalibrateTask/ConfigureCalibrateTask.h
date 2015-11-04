#ifndef ConfigureCalibrateTask_H
#define ConfigureCalibrateTask_H
#include "Task.h"
#include "BaseSysCalibrationController.h"
#include "BaseSysConfigController.h"
#include "ConfigCalCompleteDataPck.h"
#include "DebugLoggerBase.h"

class FunctionPtrInternal;

class ConfigureCalibrateTask : public Task 
{
    public:
        ConfigureCalibrateTask(BaseSysCalibrationController** calController, BaseSysConfigController** configController, DebugLoggerBase* debugLogger);
        virtual void ExecuteTask(int interval);
        virtual void Init(); 
    private:
        void SendRequest(ConfigCalCompleteDataPck::ConfigCalComplete* configCalComplete);
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
        BaseSysCalibrationController** calController; 
        BaseSysConfigController** configController;
        bool initRun;
        bool configureCalibrateComplete;
        bool calComplete;
        bool configComplete;
        DebugLoggerBase* debugLogger;
};
#endif