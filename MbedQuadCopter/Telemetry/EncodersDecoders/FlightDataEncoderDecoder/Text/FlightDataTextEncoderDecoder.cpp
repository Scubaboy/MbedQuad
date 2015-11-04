#include "FlightDataTextEncoderDecoder.h"
#include "mbed.h"

const int tempBufferSize = 100;

FlightDataTextEncoderDecoder::FlightDataTextEncoderDecoder(const SensorFlightData::SensorFlightDataStruct* systemFlightData, BaseComms* comms)
{
   this->systemFlightData = systemFlightData;
   this->comms = comms;
}

void FlightDataTextEncoderDecoder::Encode()
{
    char localFlightData[FlightDataEncoderDecoder::stringSize];
    
    if (this->comms != NULL && this->systemFlightData != NULL)
    {
        if (this->comms->CommsEstablished())
        {
            this->comms->SendDataPacket("System flightdata \n\n\0", 20);
            this->BuildAccelFlightData(localFlightData);
            this->comms->SendDataPacket(localFlightData, strlen(localFlightData)); 
            this->BuildGyroFlightData(localFlightData);
            this->comms->SendDataPacket(localFlightData, strlen(localFlightData));
            this->BuildMagFlightData(localFlightData);
            this->comms->SendDataPacket(localFlightData, strlen(localFlightData));
            this->BuildIMUFlightData(localFlightData);
            this->comms->SendDataPacket(localFlightData, strlen(localFlightData));
            this->BuildFlightPackData(localFlightData);
            this->comms->SendDataPacket(localFlightData, strlen(localFlightData));
        }   
    }
}

void FlightDataTextEncoderDecoder::BuildAccelFlightData(char* sendBuffer)
{
    //Start with AccelFlightData
    memset(sendBuffer,'\0',tempBufferSize);
    sprintf(sendBuffer,"Accel Data <%.2f,%.2f,%.2f>\n",this->systemFlightData->accelFlightData->scaledXAxis,
                                                      this->systemFlightData->accelFlightData->scaledYAxis,
                                                      this->systemFlightData->accelFlightData->scaledZAxis);
}
 
void FlightDataTextEncoderDecoder::BuildGyroFlightData(char* sendBuffer)
{
    //GyroFlightData
    memset(sendBuffer,'\0',tempBufferSize);
    sprintf(sendBuffer,"Gyro Scaled Data <%.2f,%.2f,%.2f>\n",this->systemFlightData->gyroFlightData->scaledXAxisRateCentiDegrees,
                                                      this->systemFlightData->gyroFlightData->scaledYAxisRateCentiDegrees,
                                                      this->systemFlightData->gyroFlightData->scaledZAxisRateCentiDegrees);
}

void FlightDataTextEncoderDecoder::BuildMagFlightData(char* sendBuffer)
{
    //MagFlightData
    memset(sendBuffer,'\0',tempBufferSize);
    sprintf(sendBuffer,"Mag Data <%.2f,%.2f,%.2f>\n",this->systemFlightData->magFlightData->scaledXAxis,
                                                      this->systemFlightData->magFlightData->scaledYAxis,
                                                      this->systemFlightData->magFlightData->scaledZAxis);  
}

void FlightDataTextEncoderDecoder::BuildIMUFlightData(char* sendBuffer)
{
    memset(sendBuffer,'\0',tempBufferSize);
    sprintf(sendBuffer,"IMU Data <%.2f,%.2f,%.2f>\n",this->systemFlightData->imuFlightData->pitchAngle,
                                                      this->systemFlightData->imuFlightData->rollAngle,
                                                      this->systemFlightData->imuFlightData->yawAngle);   
}

void FlightDataTextEncoderDecoder::BuildFlightPackData(char* sendBuffer)
{
    memset(sendBuffer,'\0',tempBufferSize);
    sprintf(sendBuffer,"FlightPack Data <%.2f,%.2f,%.2f>\n\n",this->systemFlightData->flightPackData->cell1Voltage,
                                                              this->systemFlightData->flightPackData->cell1_2Voltage,
                                                              this->systemFlightData->flightPackData->cell1_3Voltage);
}

void FlightDataTextEncoderDecoder::Decode()
{
    
}