#include "GyroCalController.h"
#include "CalStructure.h"

const int RequiredSamples = 10;
const char requestMsg[] = "Place quad on level and vibration free surface.\0";
const char CompleteMsg[] = "Qyro cal complete.\0";
static MsgDataPck::MsgData request;// =   new MsgDataPck::MsgData();

GyroCalController::GyroCalController (BaseDataStorageController* dataStorageController, 
                                      GyroTypes::GyroCalData *calData, 
                                      GyroFlightData::GyroFlightDataStruct* flightData,
                                      UserResponce::UserResponceStruct* userResponce,
                                      SensorBase* gyro) : test(LED1)
{
     this->gyro = gyro;
     this->calData = calData;
     this->calComplete = false;
     this->flightData = flightData;
     this->dataStorageController = dataStorageController;
     this->calStage = GyroCalCtrlTypes::CheckStorage;
     this->prevCalStage = GyroCalCtrlTypes::CheckStorage;
     this->userResponce = userResponce;
     this->calResult = false;
}

bool GyroCalController::CheckAndUseStoredCalData()
{
    bool result = false;
   
    if (this->dataStorageController != NULL)
    {
        if (this->dataStorageController->DataAvailable())
        {
            bool ageValid = false;
            
            if (this->dataStorageController->ReadCalData(&calibrationData, &ageValid))
            {
                if (ageValid)
                {
                    if (this->calData != NULL)
                    {
                        this->calData->xOffset = calibrationData.gyroCal.xOffset;
                        this->calData->yOffset = calibrationData.gyroCal.yOffset;
                        this->calData->zOffset = calibrationData.gyroCal.zOffset;             
                        result = true;
                    }               
                }  
            } 
        }
    }
    
    return result;
}
       
bool GyroCalController::RunCalibration()
{    
    if (!this->calResult)
    {
        switch (this->calStage)
        {
            case GyroCalCtrlTypes::CheckStorage:
            {
                if (this->CheckAndUseStoredCalData())
                {
                    this->calStage = GyroCalCtrlTypes::Complete;
                }
                else
                {
                    this->calStage = GyroCalCtrlTypes::SendStartRequest;
                }
            
                break;   
            } 
            case GyroCalCtrlTypes::SendStartRequest:
            {
                this->CompileRequest(requestMsg, strlen(requestMsg));
                this->calStage =  GyroCalCtrlTypes::WaitingStartAck;
                break;
            }
            case GyroCalCtrlTypes::WaitingStartAck:
            {
                if (this->userResponce->userAck)
                {
                    test = !test;
                
                    if (this->prevCalStage == GyroCalCtrlTypes::Calibrating)
                    {
                        this->calStage = GyroCalCtrlTypes::Complete;
                    }
                    else
                    {
                        this->calStage =  GyroCalCtrlTypes::Calibrating;
                    }
                
                    this->userResponce->userAck = false;     
                }
                
                break;
            }
            case GyroCalCtrlTypes::Calibrating:
            {
                //Take a number of readings and average.
                this->samplesTaken = 0;
            
                while (samplesTaken < RequiredSamples)
                {
                    this->gyro->GetRawSensorData();
                    this->calData->xOffset = (int)this->flightData->rawXAxisRate;
                    this->calData->xOffset = (int)this->flightData->rawYAxisRate;
                    this->calData->xOffset = (int)this->flightData->rawZAxisRate;
                    wait(0.5);
                    samplesTaken++;
                }
            
                this->calData->xOffset /= RequiredSamples;
                this->calData->yOffset /= RequiredSamples;
                this->calData->zOffset /= RequiredSamples;
            
                this->calibrationData.gyroCal.xOffset = this->calData->xOffset;  
                this->calibrationData.gyroCal.yOffset = this->calData->yOffset;
                this->calibrationData.gyroCal.zOffset = this->calData->zOffset;
            
                this->prevCalStage = GyroCalCtrlTypes::Calibrating;
                this->calStage = GyroCalCtrlTypes::WaitingStartAck;
                
                if (!this->CompileRequest(CompleteMsg, strlen(CompleteMsg)))
                {
                    this->calStage = GyroCalCtrlTypes::Calibrating;
                }
                
                break;   
            }
            case GyroCalCtrlTypes::Complete:
            {
                this->WriteCalDataToDataStore();
                this->calResult = true;
                break;   
            }
        }      
    }
    
    return this->calResult;
} 

bool GyroCalController::WriteCalDataToDataStore()
{
    bool writeResult = false;
    
    if (this->dataStorageController != NULL)
    {
        writeResult = this->dataStorageController->WriteCalData(calibrationData);
    }
    
    return writeResult;
}

bool GyroCalController::SendRequest(char* msg, int len, bool reSend)
{
    bool result = true;
    
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)msg, sizeof(MsgDataPck::MsgData),true))
        {
           result = false; 
        }
    }
    
    return result;
}

bool GyroCalController::CompileRequest(const char* msg, int len)
{
    //Send Instruction
       
    request.type = DataPckTypes::Message;
    request.ackRequired = true;
    memset (request.msg,'1',sizeof(request.msg));
    memcpy(request.msg,msg,len);
    return this->SendRequest((char*)&request,sizeof(request)); 

}
