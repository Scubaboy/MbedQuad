#ifndef XYZAxisCalController_H
#define XYZAxisCalController_H
#include "BaseCalibrationController.h"
#include "BaseDataStorageController.h"
#include "UserResponce.h"
#include "SensorBase.h"
#include "BaseThreeAxisFlightData.h"
#include "XYZAxisMinMax.h"
#include "XYZAxisCalCtrlTypes.h"

class XYZAxisCalController : public BaseCalibrationController
{
    public:
        XYZAxisCalController (BaseDataStorageController* dataStorageController, 
                           XYZAxisMinMax::XYZAxisMinMaxCalData *calData, 
                           BaseThreeAxisFlightData::BaseThreeAxisFlightDataStruct* flightData,
                           UserResponce::UserResponceStruct* userResponce, 
                           SensorBase* sensor,
                           const XYZAxisCalCtrlTypes::AxisCalNameLookup* lookUp);
        virtual bool RunCalibration();
        virtual bool CheckAndUseStoredCalData() = 0; 
        virtual bool WriteCalDataToDataStore() = 0; 
    private:
        void CompileRequest(const char* msg, int len, bool reSend);
        bool CalibrateAxis();
        bool RunAxisCalibration(XYZAxisCalCtrlTypes::Axis axis, XYZAxisCalCtrlTypes::AxisMinMax minMax);
        float AverageReading(XYZAxisCalCtrlTypes::Axis axis);
        void SendInstruction(XYZAxisCalCtrlTypes::Axis axis, XYZAxisCalCtrlTypes::AxisMinMax minMax);
        virtual bool SendRequest(char* msg, int len, bool reSend = true);
    public:
        template <class ClassT>
        void SendRequest(ClassT *ptr, bool(ClassT::*pFunc)(char* data, const int DataLength, bool confRequired))
        {
                this->sendRequestrptr = (FunctionPtrInternal*)ptr;  
                this->methodCallbackSendRequest = (bool (FunctionPtrInternal::*)(char* data, const int DataLength, bool confRequired))pFunc; 
        }
    public:
        CalStructure::CalMemStructure calibrationData;
    private:
        FunctionPtrInternal* sendRequestrptr;
        bool (FunctionPtrInternal::*methodCallbackSendRequest)(char* data, const int DataLength, bool confRequired);
        XYZAxisMinMax::XYZAxisMinMaxCalData *calData;
        BaseThreeAxisFlightData::BaseThreeAxisFlightDataStruct* flightData;
        SensorBase* sensor;
        bool calComplete;
        XYZAxisCalCtrlTypes::Stages calStage;
        XYZAxisCalCtrlTypes::Axis axisUnderCal;
        XYZAxisCalCtrlTypes::AxisMinMax axisMag;
        XYZAxisCalCtrlTypes::AxisCalStages axisCalStage;
        BaseDataStorageController* dataStorageController;
        UserResponce::UserResponceStruct* userResponce;
        int samplesTaken;
        DigitalOut test;
        bool waitingLastSend;
        float lastReading;
        const XYZAxisCalCtrlTypes::AxisCalNameLookup* lookUp;
};

#endif