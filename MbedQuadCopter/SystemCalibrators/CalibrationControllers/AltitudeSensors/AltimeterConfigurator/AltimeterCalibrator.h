#ifndef AltimeterCalibrator_H
#define AltimeterCalibrator_H
#include "BaseCalibrationController.h"
#include "AltimeterBase.h"
#include "AltimeterCalBase.h"
#include "BaseDataStorageController.h"
#include "AltimeterConfigTypes.h"
#include "AltimeterDataRequestRcvs.h"
#include "AltMeterFlightData.h"


class AltimeterCalibrator : public BaseCalibrationController
{
    public:
        AltimeterCalibrator(AltiMeterBase* altimeter,
                              AltMeterFlightData::AltMeterFlightDataStruct* flightData,
                              MPL115A2CalData::CalData* calData,
                              AltimeterDataRequestRcvs::AltitudeDataRequest* altimeterCalibration);
        virtual bool RunCalibration(); 
    
    private:
        virtual bool SendRequest(char* msg, int len, bool reSend = true);
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
        AltiMeterBase* altimeter; 
        MPL115A2CalData::CalData* calData; 
        AltimeterConfigTypes::Stages configStage;
        AltimeterDataRequestRcvs::AltitudeDataRequest* altimeterCalibration;
        AltMeterFlightData::AltMeterFlightDataStruct* flightData;
        bool waitingLastSend;
};
#endif