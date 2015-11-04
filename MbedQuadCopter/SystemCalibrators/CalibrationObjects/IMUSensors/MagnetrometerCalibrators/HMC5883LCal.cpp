#include "HMC5883LCal.h"
#include "mbed.h"

const XYZAxisMinMax::XYZAxisMinMaxCalData DefaultCal = {-600,600,-600,600,-600,600};
const float Scaler = 100.0f;

HMC5883LCal::HMC5883LCal(XYZAxisMinMax::XYZAxisMinMaxCalData *calData, MagStatus::MagStatusStruct* magStatus)
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
    
    if (magStatus != NULL)
    {
        this->magStatus = magStatus;
    }
    else
    {
        this->magStatus = NULL;
    }
}

const IMUTypes::SensorChannelScales HMC5883LCal::GetChannelScaleFactors() const
{
    IMUTypes::SensorChannelScales scaleFactors;
  
    scaleFactors.xScale = 0.0f;
    scaleFactors.yScale = 0.0f;
    scaleFactors.zScale = 0.0f;
  
    if (this->validCalData)
    {
        scaleFactors.xScale = (Scaler / (float)(this->calData->xMax - ((this->calData->xMin+this->calData->xMax) / 2.0f)));
        scaleFactors.yScale = (Scaler / (float)(this->calData->yMax - ((this->calData->yMin+this->calData->yMax) / 2.0f)));
        scaleFactors.zScale = (Scaler / (float)(this->calData->zMax - ((this->calData->zMin+this->calData->zMax) / 2.0f)));
        
        if (this->magStatus != NULL)
        {
            this->magStatus->failedToSetScaleFactors = false;
        }
    }
    else
    {
        if (this->magStatus != NULL)
        {
            this->magStatus->failedToSetScaleFactors = true;
        }
    }

  return scaleFactors;
}

const IMUTypes::SensorChannelOffsets HMC5883LCal::GetChannelOffsets() const
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
        
        if (this->magStatus != NULL)
        {
            this->magStatus->failedToSetOffsets = false; 
        }
    }
    else
    {
        if (this->magStatus != NULL)
        {
            this->magStatus->failedToSetOffsets = true; 
        }
    }
  
    return offsets;
}