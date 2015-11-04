#include "ITG3200Cal.h"
#include "mbed.h"

const GyroTypes::GyroCalData DefaultCal = {-34.82,100.41,-16.38};

ITG3200Cal::ITG3200Cal(GyroTypes::GyroCalData *calData, GyroStatus::GyroStatusStruct* gyroStatus)
{
    if (calData != NULL)
    {
        this->calData= calData;
        this->validCalData = true;   
    }
    else
    {
        this->calData = &DefaultCal;
        this->validCalData = true;  
    }  
    
    if (gyroStatus != NULL)
    {
        this->gyroStatus = gyroStatus;
    }
    else
    {
        this->gyroStatus = NULL;
    }
}

const IMUTypes::SensorChannelScales ITG3200Cal::GetChannelScaleFactors() const
{
    IMUTypes::SensorChannelScales scaleFactors;
  
    scaleFactors.xScale = 0.0f;
    scaleFactors.yScale = 0.0f;
    scaleFactors.zScale = 0.0f;

  return scaleFactors;
}

const IMUTypes::SensorChannelOffsets ITG3200Cal::GetChannelOffsets() const
{
    IMUTypes::SensorChannelOffsets offsets;
  
    offsets.xOffset = 0.0f;
    offsets.yOffset = 0.0f;
    offsets.zOffset = 0.0f;
  
    if (this->validCalData)
    {
        offsets.xOffset = this->calData->xOffset;
        offsets.yOffset = this->calData->yOffset;
        offsets.zOffset = this->calData->zOffset;
        
        if (this->gyroStatus != NULL)
        {
            this->gyroStatus->failedToSetOffsets = false; 
        }
    }
    else
    {
        if (this->gyroStatus != NULL)
        {
            this->gyroStatus->failedToSetOffsets = true; 
        }
    }
  
    return offsets;
}