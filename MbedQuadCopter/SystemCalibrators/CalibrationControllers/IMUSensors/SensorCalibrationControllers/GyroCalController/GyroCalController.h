#ifndef GyroCalController_H
#define GyroCalController_H
#include "BaseCalibrationController.h"
#include "GyroTypes.h"
#include "SensorBase.h"
#include "GyroCalCtrlTypes.h"
#include "BaseDataStorageController.h"
#include "UserResponce.h"
#include "GyroFlightData.h"
#include "mbed.h"

class GyroCalController : public BaseCalibrationController
{
    public:
        GyroCalController (BaseDataStorageController* dataStorageController, 
                           GyroTypes::GyroCalData *calData, 
                           GyroFlightData::GyroFlightDataStruct* flightData,
                           UserResponce::UserResponceStruct* userResponce, 
                           SensorBase* gyro);
        virtual bool RunCalibration();  
    private:
        bool WriteCalDataToDataStore();
        bool CheckAndUseStoredCalData();
        virtual bool SendRequest(char* msg, int len, bool reSend = true);
        bool CompileRequest(const char* msg, int len);
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
        GyroTypes::GyroCalData *calData;
        GyroFlightData::GyroFlightDataStruct* flightData;
        SensorBase* gyro;
        bool calComplete;
        GyroCalCtrlTypes::Stages calStage;
        GyroCalCtrlTypes::Stages prevCalStage;
        BaseDataStorageController* dataStorageController;
        UserResponce::UserResponceStruct* userResponce;
        int samplesTaken;
        CalStructure::CalMemStructure calibrationData;
        DigitalOut test;
        bool calResult;
};
#endif