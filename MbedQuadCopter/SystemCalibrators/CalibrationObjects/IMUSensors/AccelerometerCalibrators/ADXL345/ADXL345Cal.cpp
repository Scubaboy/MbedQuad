#include "ADXL345Cal.h"
#include "mbed.h"
#include "const.h"

const XYZAxisMinMax::XYZAxisMinMaxCalData DefaultCal = {-250,250,-250,250,-250,250};

ADXL345Cal::ADXL345Cal(XYZAxisMinMax::XYZAxisMinMaxCalData *calData, AccelStatus::AccelStatusStruct* accelStatus)
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
    
    if (accelStatus != NULL)
    {
        this->accelStatus = accelStatus;
    }
    else
    {
        this->accelStatus = NULL;
    }
}

const IMUTypes::SensorChannelScales ADXL345Cal::GetChannelScaleFactors() const
{
    IMUTypes::SensorChannelScales scaleFactors;
  
    scaleFactors.xScale = 0.0f;
    scaleFactors.yScale = 0.0f;
    scaleFactors.zScale = 0.0f;
  
    if (this->validCalData)
    {
        scaleFactors.xScale = (PhysicalConstants::Gravity / (float)(this->calData->xMax - ((this->calData->xMin+this->calData->xMax) / 2.0f)));
        scaleFactors.yScale = (PhysicalConstants::Gravity / (float)(this->calData->yMax - ((this->calData->yMin+this->calData->yMax) / 2.0f)));
        scaleFactors.zScale = (PhysicalConstants::Gravity / (float)(this->calData->zMax - ((this->calData->zMin+this->calData->zMax) / 2.0f)));
        
        if (this->accelStatus != NULL)
        {
            this->accelStatus->failedToSetScaleFactors = false;
        }
    }
    else
    {
        if (this->accelStatus != NULL)
        {
            this->accelStatus->failedToSetScaleFactors = true;
        }
    }

  return scaleFactors;
}

const IMUTypes::SensorChannelOffsets ADXL345Cal::GetChannelOffsets() const
{
    IMUTypes::SensorChannelOffsets offsets;
  
    offsets.xOffset = 0.0f;
    offsets.yOffset = 0.0f;
    offsets.zOffset = 0.0f;
  
    if (this->validCalData)
    {
        offsets.xOffset = ((this->calData->xMin+this->calData->xMax) / 2.0f);
        offsets.yOffset = ((this->calData->yMin+this->calData->yMax) / 2.0f);
        offsets.zOffset = ((this->calData->zMin+this->calData->zMax) / 2.0f);
        
        if (this->accelStatus != NULL)
        {
            this->accelStatus->failedToSetOffsets = false; 
        }
    }
    else
    {
        if (this->accelStatus != NULL)
        {
            this->accelStatus->failedToSetOffsets = true; 
        }
    }
  
    return offsets;
}