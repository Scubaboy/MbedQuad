#include "FlightDataBinaryEncoderDecoder.h"
#include "mbed.h"

FlightDataBinaryEncoderDecoder::FlightDataBinaryEncoderDecoder(const SensorFlightData::SensorFlightDataStruct* systemFlightData)
{
   this->systemFlightData = systemFlightData;

}

void FlightDataBinaryEncoderDecoder::Encode()
{
    if (this->systemFlightData != NULL)
    {
        this->BuildLocalFlightData();
        this->SendRequest(&this->localFlightData);    
    }
}

void FlightDataBinaryEncoderDecoder::BuildLocalFlightData()
{
    if (this->systemFlightData != NULL)
    {
        this->localFlightData.type = DataPckTypes::FlightData;
        this->localFlightData.accelFlightData = *this->systemFlightData->accelFlightData;
        this->localFlightData.altimeterFlightData = *this->systemFlightData->altimeterFlightData;
        this->localFlightData.gyroFlightData = *this->systemFlightData->gyroFlightData;
        this->localFlightData.magFlightData = *this->systemFlightData->magFlightData;
        this->localFlightData.imuFlightData = *this->systemFlightData->imuFlightData;
    }
}

void FlightDataBinaryEncoderDecoder::SendRequest(FlightDataDataPck::FlightData* data)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)data, sizeof(FlightDataDataPck::FlightData),false))
        {
            
        }
    }
}

void FlightDataBinaryEncoderDecoder::Decode()
{
    
}