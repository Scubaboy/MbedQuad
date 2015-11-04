#include "MotorConfig.h"

 MotorConfig::MotorConfig(QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorProperties,
                          BaseDataStorageController* dataStorageController,
                          MotorConfigDataRequestRcvs::MotorConfigRcvs* motorConfigRcvs)
{
    this->dataStorageController = dataStorageController;
    this->motorProperties = motorProperties;  
    this->configStage = MotorConfigTypes::CheckStorage;
    this->motorConfigRcvs = motorConfigRcvs;
}

bool MotorConfig::CheckAndUseStoredCalData()
{
    bool result = false;
   
    if (this->dataStorageController != NULL)
    {
        if (this->dataStorageController->DataAvailable())
        {
            bool ageValid = false;
            
            if (this->dataStorageController->ReadConfigData(&configData, &ageValid))
            {
                if (ageValid)
                {
                    memcpy (this->motorProperties->frameMappings,
                            configData.frameMappings,
                            sizeof(QuadMotorMappingDefinition::MotorFrameMapping));
                    
                    memcpy (this->motorProperties->motorProperties,
                            configData.motorProperties,
                            sizeof(QuadMotorMappingDefinition::MotorProperties));
                    
                    memcpy (this->motorProperties->gloabalLimits,
                            &configData.gloabalLimits,
                            sizeof(QuadMotorProperties::GlobalLimits));
                    
                    memcpy (this->motorProperties->pwmChannelMotorMapping,
                            configData.pwmChannelMotorMapping,
                            sizeof(PWMChannelMotorMapping::ChannelMotorMappingDefinition));     
                    
                    result = true;       
                }  
            } 
        }
    }
    
    return result;
}

bool MotorConfig::Configure()
{
    bool configResult = false;
    
    switch (this->configStage)
    {
        case MotorConfigTypes::CheckStorage:
        {
            
            if (this->CheckAndUseStoredCalData())
            {
                this->configStage = MotorConfigTypes::Complete;
            }
            else
            {
                this->configStage = MotorConfigTypes::SendMotorConfigRequest;
            }
            
            break;   
        }
        case MotorConfigTypes::SendMotorConfigRequest:
        {
            //Send Instruction
            DataRequestPck::DataRequest request;
        
            request.type = DataPckTypes::RequestData;
            request.pressureAtSealevel = false;
            request.motorConfigData = true;
            request.RateCtrlConfigData = false;
            this->SendRequest(&request);
            this->configStage = MotorConfigTypes::WaitingRequestAck;
            break;   
        }
        case MotorConfigTypes::WaitingRequestAck:
        {
            if (this->motorConfigRcvs->motorConfigDataReceived)
            {
                memcpy (configData.frameMappings,
                        this->motorConfigRcvs->frameMappings,
                        sizeof(QuadMotorMappingDefinition::MotorFrameMapping));
                    
                memcpy (configData.motorProperties,
                        this->motorConfigRcvs->motorProperties,
                        sizeof(QuadMotorMappingDefinition::MotorProperties));
                    
                memcpy (&configData.gloabalLimits,
                        &this->motorConfigRcvs->gloabalLimits,
                        sizeof(QuadMotorProperties::GlobalLimits));
                    
                memcpy (configData.pwmChannelMotorMapping,
                        this->motorConfigRcvs->pwmChannelMotorMapping,
                        sizeof(PWMChannelMotorMapping::ChannelMotorMappingDefinition));
                        
                this->configStage = MotorConfigTypes::Complete;
                this->motorConfigRcvs->motorConfigDataReceived = false; 
            }
            break;
        }
        case MotorConfigTypes::Complete:
        {
            if (this->dataStorageController->WriteConfigData(configData))
            {
                configResult = true;   
            }
            break;   
        }
    }
    
    return configResult;
}

void MotorConfig::SendRequest(DataRequestPck::DataRequest* dataRequest)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)dataRequest, sizeof(DataRequestPck::DataRequest),true))
        {
            
        }
    }
}