#include "ITG3200.h"
#include "ITG3200Cal.h"
#include "ITG3200Registers.h"
#include "AngleConverters.h"
#include "mbed.h"

const int I2CBaseAddr = 0x69 << 1;
const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::IMU;
const ITG3200Cal defaultGyroCall = ITG3200Cal(NULL,NULL);
const float GyroGain = 0.06957;

ITG3200::ITG3200(SensorCalBase* calObj,I2CBusCtrl* busCtrl, GyroStatus::GyroStatusStruct* gyroStatus, GyroFlightData::GyroFlightDataStruct* flightData)
{
    if (calObj != NULL)
    {
        this->calObj = calObj;
    }
    else
    {
        this->calObj = &defaultGyroCall;
    }
    
    if (gyroStatus != NULL)
    {
        this->gyroStatus = gyroStatus; 
        this->gyroStatus->calibrationComplete = false;
        this->gyroStatus->readyForUse = false;  
    }
    else
    {
        this->gyroStatus = NULL;
    }
    
    if (flightData != NULL)
    {
        this->flightData = flightData;   
    }
    else
    {
        this->flightData = NULL;    
    }
    
   // if (busCtrl != NULL)
    //{
        this->busCtrl = busCtrl;
    //}
    
    this->sensorReady = false;
}

void ITG3200::InitSensor()
{
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            char data[2];
        
            //Power up reset defaults.
            data[0] = ITG3200Registers::PWR_MGM;
            data[1] = 0x80;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            //Select full-scale range of the gyro sensors
            //Set LP filter bandwidth to 42Hz
            data[0] = ITG3200Registers::DLPF_FS;
            data[1] = 0x1B;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            //Set sample rato to 50Hz
            data[0] = ITG3200Registers::SMPLRT_DIV;
            data[1] = 0x0A;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            //Set clock to PLL with z gyro reference
            data[0] = ITG3200Registers::PWR_MGM;
            data[1] = 0x00;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            this->sensorReady = true;
        
            this->busCtrl->ReleaseCtrl(BusCtrlUsers::IMU);
        }
        else
        {
            if (this->gyroStatus != NULL)
            {
                sprintf(this->gyroStatus->msg,"ITG3200::InitSensor - I2C Bus Ctrl failed to take bus!\0");   
            }  
        }
    }
    else
    {
        if (this->gyroStatus != NULL)
        {
            sprintf(this->gyroStatus->msg,"ITG3200::InitSensor - I2C Bus Ctrl not Set for ITG3200!\0");   
        }
    }
}

void ITG3200::GetRawSensorData()
{  
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            char data[6];
            int readRst = 1;
        
            //Set DataX0 register ready for reading.
            data[0] = ITG3200Registers::GYRO_XOUT_H;
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 1);
        
            //Read axis data
            readRst = this->busCtrl->ReadFromBus(busUser, I2CBaseAddr, data , 6);
        
            if (readRst == 0)
            {
                if (this->flightData != NULL)
                {
                    // No multiply by -1 for coordinate system transformation here, because of double negation:
                    // We want the gravity vector, which is negated acceleration vector.
                    this->flightData->rawXAxisRate = -1*int16_t((((int16_t) data[2]) << 8) | data[3]);  // X axis (internal sensor y axis)
                    this->flightData->rawYAxisRate = -1*int16_t((((int16_t) data[0]) << 8) | data[1]);  // Y axis (internal sensor x axis)
                    this->flightData->rawZAxisRate = -1*int16_t((((int16_t) data[4]) << 8) | data[5]);  // Z axis (internal sensor z axis)
                }
                else
                {
                    if (this->gyroStatus != NULL)
                    {
                        sprintf(this->gyroStatus->msg,"ITG3200::GetRawSensorData - flightdata struct null!\0");   
                    }
                }
            }
            else
            {
                sprintf(this->gyroStatus->msg,"ITG3200::GetRawSensorData - Failed to read axis data!\0"); 
            }
        
            this->busCtrl->ReleaseCtrl(BusCtrlUsers::IMU);
        }
        else
        {
            if (this->gyroStatus != NULL)
            {
                sprintf(this->gyroStatus->msg,"ITG3200::GetRawSensorData - I2C Bus Ctrl failed to take bus!\0");   
            } 
        }
        
    }
    else
    {
        if (this->gyroStatus != NULL)
        {
            sprintf(this->gyroStatus->msg,"ITG3200::GetRawSensorData - I2C Bus Ctrl not Set for ITG3200!\0");   
        }
    }
}

void ITG3200::GetScaledSensorData()
{
    IMUTypes::SensorChannelOffsets offsets;
    
    if (this->sensorReady)
    {
        this->GetRawSensorData();
     
        if (this->calObj != NULL)
        {
            offsets = this->calObj->GetChannelOffsets();
            
            if (this->flightData != NULL)
            {
                this->flightData->scaledXAxisRate = this->RadsPerSecond(this->flightData->rawXAxisRate - offsets.xOffset);
                this->flightData->scaledYAxisRate = this->RadsPerSecond(this->flightData->rawYAxisRate - offsets.yOffset);
                this->flightData->scaledZAxisRate = this->RadsPerSecond(this->flightData->rawZAxisRate - offsets.zOffset);
                
                this->flightData->scaledXAxisRateCentiDegrees = AngleConverters::RadiansToCentiDegrees(this->flightData->scaledXAxisRate);
                this->flightData->scaledYAxisRateCentiDegrees = AngleConverters::RadiansToCentiDegrees(this->flightData->scaledYAxisRate);
                this->flightData->scaledZAxisRateCentiDegrees = AngleConverters::RadiansToCentiDegrees(this->flightData->scaledZAxisRate);
            }
            else
            {
                if (this->gyroStatus != NULL)
                {
                    sprintf(this->gyroStatus->msg,"ITG3200::GetScaledSensorData - flightdata struct null!\0");   
                }
            }
        } 
    }
}

float ITG3200::RadsPerSecond(float scaledValue)
{
    return scaledValue * (0.01745329252 * GyroGain);    
}