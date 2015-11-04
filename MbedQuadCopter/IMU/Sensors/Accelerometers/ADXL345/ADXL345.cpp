#include "ADXL345.h"
#include "mbed.h"
#include "ADXL345Registers.h"
#include "ADXL345Cal.h"

const int I2CBaseAddr = 0x53<< 1;
const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::IMU;
const ADXL345Cal defaultAccelCall = ADXL345Cal(NULL,NULL);

ADXL345::ADXL345(SensorCalBase* calObj, I2CBusCtrl* busCtrl, AccelStatus::AccelStatusStruct* accelStatus, AccelFlightData::AccelFlightDataStruct* flightData)
{
    if (calObj != NULL)
    {
        this->calObj = calObj;
    }
    else
    {
        this->calObj = &defaultAccelCall;
    }
    
    if (flightData != NULL)
    {
        this->flightData = flightData;    
    }
    
    if (accelStatus != NULL)
    {
        this->accelStatus = accelStatus; 
        this->accelStatus->calibrationComplete = false;
        this->accelStatus->readyForUse = false;  
    }
    else
    {
        this->accelStatus = NULL;
    }
    
    if (busCtrl != NULL)
    {
        this->busCtrl = busCtrl;
    }
    
    this->sensorReady = false;
}

void ADXL345::InitSensor()
{
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            char data[2];
        
            //Set to measurement mode.
            data[0] = ADXL345Registers::POWER_CTL;
            data[1] = 0x08;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            //Set to full scale resolution.
            data[0] = ADXL345Registers::DATA_FORMAT;
            data[1] = 0x08;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            //Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwidth)
            data[0] = ADXL345Registers::RATE;
            data[1] = 0x09;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            this ->sensorReady = true;
        }
        else
        {
            if (this->accelStatus != NULL)
            {
                sprintf(this->accelStatus->msg,"ADXL345::InitSensor - I2C Bus Ctrl failed to take ctrl\0");   
            }
        }
    }
    else
    {
        if (this->accelStatus != NULL)
        {
            sprintf(this->accelStatus->msg,"ADXL345::InitSensor - I2C Bus Ctrl not Set for ADXL345!\0");   
        }
    }
}

void ADXL345::GetRawSensorData()
{
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            char data[6];
            int readRst = 1;
        
            //Set DataX0 register ready for reading.
            data[0] = ADXL345Registers::DATAX0;
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 1);
        
            //Read axis data
            readRst = this->busCtrl->ReadFromBus(busUser, I2CBaseAddr, data , 6);
        
            if (readRst == 0)
            {
                // No multiply by -1 for coordinate system transformation here, because of double negation:
                // We want the gravity vector, which is negated acceleration vector.
                if (this->flightData != NULL)
                {
                    this->flightData->rawXAxis = int16_t((((int) data[3]) << 8) | data[2]);  // X axis (internal sensor y axis)
                    this->flightData->rawYAxis = int16_t((((int) data[1]) << 8) | data[0]);  // Y axis (internal sensor x axis)
                    this->flightData->rawZAxis = int16_t((((int) data[5]) << 8) | data[4]);  // Z axis (internal sensor z axis)
                }
                else
                {
                    if (this->accelStatus != NULL)
                    {
                        sprintf(this->accelStatus->msg,"ADXL345::GetRawSensorData - Flightdata struct null!\0"); 
                    }
                }
            }
            else
            {
                if (this->accelStatus != NULL)
                {
                    sprintf(this->accelStatus->msg,"ADXL345::GetRawSensorData - Failed to read axis data!\0"); 
                }
            }
        }
        else
        {
            if (this->accelStatus != NULL)
            {
                sprintf(this->accelStatus->msg,"ADXL345::GetRawSensorData - I2C Bus Ctrl failed to take ctrl\0");   
            } 
        }
        
    }
    else
    {
        if (this->accelStatus != NULL)
        {
            sprintf(this->accelStatus->msg,"ADXL345::GetRawSensorData - I2C Bus Ctrl not Set for ADXL345!\0");   
        }
    }
}  

void ADXL345::GetScaledSensorData()
{
    IMUTypes::SensorChannelOffsets offsets;
    IMUTypes::SensorChannelScales scaleFactors;
    
    if (this->sensorReady)
    {
        this->GetRawSensorData();
     
        if (this->calObj != NULL)
        {
            offsets = this->calObj->GetChannelOffsets();
            scaleFactors = this->calObj->GetChannelScaleFactors();
            
            if (this->flightData != NULL)
            {
                this->flightData->scaledXAxis = ((float)this->flightData->rawXAxis - offsets.xOffset) * scaleFactors.xScale;
                this->flightData->scaledYAxis = ((float)this->flightData->rawYAxis - offsets.yOffset) * scaleFactors.yScale;
                this->flightData->scaledZAxis = ((float)this->flightData->rawZAxis - offsets.zOffset) * scaleFactors.zScale;
            }
            else
            {
                if (this->accelStatus != NULL)
                {
                    sprintf(this->accelStatus->msg,"ADXL345::GetScaledSensorData - Flightdata struct null!\0"); 
                } 
            }
        } 
    }
}
