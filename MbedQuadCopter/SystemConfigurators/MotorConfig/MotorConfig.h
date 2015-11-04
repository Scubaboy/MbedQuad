#ifndef MotorConfig_H
#define MotorConfig_H
#include "BaseConfigurator.h"
#include "QuadMotorConfigFlightData.h"
#include "BaseDataStorageController.h"
#include "MotorConfigDataRequestRcvs.h"
#include "MotorConfigTypes.h"

class MotorConfig : public BaseConfigurator
{
    public:
        MotorConfig(QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorProperties,
                    BaseDataStorageController* dataStorageController,
                    MotorConfigDataRequestRcvs::MotorConfigRcvs* motorConfigRcvs);
        virtual bool Configure(); 
    
    private:
        virtual void SendRequest(DataRequestPck::DataRequest* dataRequest);
        bool WriteCalDataToDataStore();
        bool CheckAndUseStoredCalData();
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
        ConfigStructure::ConfigMemStructure configData;
        MotorConfigTypes::Stages configStage;
        BaseDataStorageController* dataStorageController;
        QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorProperties;
        MotorConfigDataRequestRcvs::MotorConfigRcvs* motorConfigRcvs;

};
#endif