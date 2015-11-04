#include "HMC5883L.h"
#include "HMC5883LCal.h"
#include "HMC5883LRegisters.h"
#include "mbed.h"

const int I2CBaseAddr = 0x1E << 1;
const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::IMU;
const HMC5883LCal defaultMagCall = HMC5883LCal(NULL,NULL);

HMC5883L::HMC5883L(SensorCalBase* calObj,I2CBusCtrl* busCtrl, MagStatus::MagStatusStruct* magStatus, MagFlightData::MagFlightDataStruct* flightData)
{
    if (calObj != NULL)
    {
        this->calObj = calObj;
    }
    else
    {
        this->calObj = &defaultMagCall;
    }
    
    if (flightData != NULL)
    {
        this->flightData = flightData;   
    }
    else
    {
        this->flightData = NULL;    
    }
    if (magStatus != NULL)
    {
        this->magStatus = magStatus; 
        this->magStatus->calibrationComplete = false;
        this->magStatus->readyForUse = false;  
    }
    else
    {
        this->magStatus = NULL;
    }
    
    if (busCtrl != NULL)
    {
        this->busCtrl = busCtrl;
    }
    
    this->sensorReady = false;  
}

void HMC5883L::InitSensor()
{
   if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            char data[2];
        
            //Set to continuous mode
            data[0] = HMC5883LRegisters::MODE;
            data[1] = 0x00;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            //50Hz
            data[0] = HMC5883LRegisters::CONFIG_A;
            data[1] = 0x18;
        
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 2);
            wait_ms(5);
        
            this ->sensorReady = true;
        }
        else
        {
            if (this->magStatus != NULL)
            {
                sprintf(this->magStatus->msg,"HMC5883L::InitSensor - I2C Bus Ctrl failed to take bus!\0");   
            }
        }
    }
    else
    {
        if (this->magStatus != NULL)
        {
            sprintf(this->magStatus->msg,"HMC5883L::InitSensor - I2C Bus Ctrl not Set for HMC5883L!\0");   
        }
    }    
}

void HMC5883L::GetRawSensorData()
{
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            char data[6];
            int readRst = 1;
        
            //Set DataX0 register ready for reading.
            data[0] = HMC5883LRegisters::MAG_XOUT_H;
            this->busCtrl->WriteToBus(busUser, I2CBaseAddr, data , 1);
        
            //Read axis data
            readRst = this->busCtrl->ReadFromBus(busUser, I2CBaseAddr, data , 6);
        
            if (readRst == 0)
            {
                if (this->flightData != NULL)
                {
                    // No multiply by -1 for coordinate system transformation here, because of double negation:
                    // We want the gravity vector, which is negated acceleration vector.
                    this->flightData->rawXAxis = -1*int16_t((((int16_t) data[4]) << 8) | data[5]);  // X axis (internal sensor y axis)
                    this->flightData->rawYAxis = -1*int16_t((((int16_t) data[0]) << 8) | data[1]);  // Y axis (internal sensor x axis)
                    this->flightData->rawZAxis = -1*int16_t((((int16_t) data[2]) << 8) | data[3]);  // Z axis (internal sensor z axis)
                }
                else
                {
                    if (this->magStatus != NULL)
                    {
                        sprintf(this->magStatus->msg,"HMC5883L::GetRawSensorData - Flightdata is null!\0"); 
                    }  
                }
            }
            else
            {
                if (this->magStatus != NULL)
                {
                    sprintf(this->magStatus->msg,"HMC5883L::GetRawSensorData - Failed to read axis data!\0"); 
                }
            }
        }
        else
        {
            if (this->magStatus != NULL)
            {
                sprintf(this->magStatus->msg,"HMC5883L::GetRawSensorData - I2C Bus Ctrl failed to take bus!\0");   
            } 
        }   
    }
    else
    {
        if (this->magStatus != NULL)
        {
            sprintf(this->magStatus->msg,"HMC5883L::GetRawSensorData - I2C Bus Ctrl not Set for HMC5883L!\0");   
        }
    }
}

void HMC5883L::GetScaledSensorData()
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
                this->flightData->scaledXAxis = ((float)this->flightData->rawXAxis- offsets.xOffset) * scaleFactors.xScale;
                this->flightData->scaledYAxis = ((float)this->flightData->rawYAxis- offsets.yOffset) * scaleFactors.yScale;
                this->flightData->scaledZAxis = ((float)this->flightData->rawZAxis- offsets.zOffset) * scaleFactors.zScale;
            }
            else
            {
                if (this->magStatus != NULL)
                {
                    sprintf(this->magStatus->msg,"HMC5883L::GetScaledSensorData - Flightdata is null!\0"); 
                }
            }
        } 
    }   
}

void HMC5883L::UpdateHeadingRaw(float roll, float pitch)
{
    float mag[3];
 
    if (this->sensorReady)
    {
        if (this->flightData != NULL)
        {
            this->GetRawSensorData();  
     
            mag[0] = this->flightData->rawXAxis;
            mag[1] = this->flightData->rawYAxis;
            mag[2] = this->flightData->rawZAxis;
     
            this->flightData->rawHeading = this->GetHeading(roll, pitch, mag);
        }
        else
        {
            if (this->magStatus != NULL)
            {
                sprintf(this->magStatus->msg,"HMC5883L::UpdateRawHeading - Flightdata is null!\0"); 
            }
        }
    }    
}

void HMC5883L::UpdateHeadingScaled(float roll, float pitch)
{
    float mag[3];
 
    if (this->sensorReady)
    {
        this->GetScaledSensorData();  
     
        mag[0] = this->flightData->scaledXAxis;
        mag[1] = this->flightData->scaledYAxis;
        mag[2] = this->flightData->scaledZAxis;
        
        if (this->flightData != NULL)
        {
            this->flightData->scaledHeading = this->GetHeading(roll, pitch, mag);
        }
        else
        {
            if (this->magStatus != NULL)
            {
                sprintf(this->magStatus->msg,"HMC5883L::UpdateScaledHeading - Flightdata is null!\0"); 
            }   
        }
    }   
}

float HMC5883L::GetHeading (float roll, float pitch, float mag[3])
{
    float heading = 0.0;
    float mag_x;
    float mag_y;
    float cos_roll;
    float sin_roll;
    float cos_pitch;
    float sin_pitch;
 
    if (this->sensorReady)
    { 
        cos_roll = cos(roll);
        sin_roll = sin(roll);
        cos_pitch = cos(pitch);
        sin_pitch = sin(pitch);
     
        // Tilt compensated magnetic field X
        mag_x = mag[0] * cos_pitch + mag[1] * sin_roll * sin_pitch + mag[2] * cos_roll * sin_pitch;
    
        // Tilt compensated magnetic field Y
        mag_y = mag[1] * cos_roll - mag[2] * sin_roll;
  
        // Magnetic Heading
        heading = atan2(-mag_y, mag_x);
    }
  
  return heading;
}