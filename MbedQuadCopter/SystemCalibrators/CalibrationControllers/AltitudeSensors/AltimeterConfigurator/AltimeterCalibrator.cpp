#include "AltimeterCalibrator.h"
#include "DataRequestPck.h"

const char SeaLevelPressureRequest[] = "Enter pressure at sea level";

AltimeterCalibrator::AltimeterCalibrator(AltiMeterBase* altimeter,
                                         AltMeterFlightData::AltMeterFlightDataStruct* flightData,
                                         MPL115A2CalData::CalData* calData,
                                         AltimeterDataRequestRcvs::AltitudeDataRequest* altimeterCalibration)
{
    this->altimeter = altimeter; 
    this->calData = calData; 
    this->altimeterCalibration = altimeterCalibration;
    this->configStage = AltimeterConfigTypes::SendSealevelRequest;
    this->flightData = flightData;
}
      
bool AltimeterCalibrator::RunCalibration()
{
    bool configResult = false;
    
    switch (this->configStage)
    {
        case AltimeterConfigTypes::SendSealevelRequest:
        {
            //Send Instruction
            DataRequestPck::DataRequest request;
        
            request.type = DataPckTypes::RequestData;
            request.pressureAtSealevel = true;
            request.motorConfigData = false;
            request.RateCtrlConfigData = false;
            
            //Set message
            memset (request.request,'\0',sizeof(request.request));
            memcpy(request.request,SeaLevelPressureRequest,sizeof(SeaLevelPressureRequest));
            
            this->SendRequest((char*)&request,sizeof(request));
            
            if (!this->waitingLastSend)
            {
                this->configStage = AltimeterConfigTypes::WaitingRequestAck;   
            }
            
            break;   
        }
        case AltimeterConfigTypes::WaitingRequestAck:
        {
            if (this->altimeterCalibration->pressureAtSeaLevelRecvd)
            {
                this->calData->pressureAtSealevel = this->altimeterCalibration->pressureAtSeaLevel;
                this->altimeterCalibration->pressureAtSeaLevelRecvd = false;
                this->configStage = AltimeterConfigTypes::ReadBaseAltitude;   
            }
            break;
        }
        case AltimeterConfigTypes::ReadBaseAltitude:
        {
            if (this->flightData != NULL && this->altimeter != NULL)
            {
                this->altimeter->UpdateAltitude();
                this->calData->altitudeOffset = this->flightData->altitude;
                this->configStage = AltimeterConfigTypes::Complete; 
            }
            break;   
        }
        case AltimeterConfigTypes::Complete:
        {
            configResult = true;
            break;   
        }
    }
    
    return configResult;
}

bool AltimeterCalibrator::SendRequest(char* msg, int len, bool reSend)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)msg, len,true))
        {
            this->waitingLastSend = true;
        }
        else
        {
            this->waitingLastSend = false;
        }
    }
}