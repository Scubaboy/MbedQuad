#include "MPL115A2Cal.h"
#include "mbed.h"

const MPL115A2CalData::CalData DefaultCal = {101325,0.0};

MPL115A2Cal::MPL115A2Cal(MPL115A2CalData::CalData *calData, AltimeterStatus::AltimeterStatusStruct* altStatus)
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
    
    if (altStatus != NULL)
    {
        this->altStatus = altStatus;
    }
    else
    {
        this->altStatus = NULL;
    }   
}

const float MPL115A2Cal::GetPressureOffset() const
{
     return this->calData != NULL ? this->calData->pressureAtSealevel : 0.0; 
}

const float MPL115A2Cal::GetAltitudeOffset() const
{
    return this->calData != NULL ? this->calData->altitudeOffset : 0.0;
}