#include "PID.h"
#include "mbed.h"
#include "Range.h"

// Examples for _filter:
// f_cut = 10 Hz -> _filter = 15.9155e-3
// f_cut = 15 Hz -> _filter = 10.6103e-3
// f_cut = 20 Hz -> _filter =  7.9577e-3
// f_cut = 25 Hz -> _filter =  6.3662e-3
// f_cut = 30 Hz -> _filter =  5.3052e-3
const float  AC_PID_D_TERM_FILTER = 0.00795770f;

PID::PID(PIDControlData::PIDControlDataStruct* pidStateData, Vector2f constraints)
{
    if (pidStateData != NULL)
    {
        this->pidStateData = pidStateData;
    }
    else
    {
        this->pidStateData = NULL;   
    }
    
    this->constraints = constraints;
}

void PID::CalculatePID(float error, float timeDelta)
{
    if (this->pidStateData != NULL)
    {
        this->CalculateP(error);
        this->CalculateI(error, timeDelta);
        this->CalculateD(error, timeDelta);
        
        this->pidStateData->pidValue = this->ConstrainOuput(this->pidStateData->pValue + this->pidStateData->iValue + this->pidStateData->dValue);
    } 
}

float PID::ConstrainOuput(float rawInput)
{
    return Range::Constrain (rawInput, this->constraints.x, this->constraints.y);
}

void PID::CalculatePI(float error, float timeDelta)
{
    if (this->pidStateData != NULL)
    {
        this->CalculateP(error);
        this->CalculateI(error, timeDelta);
        this->pidStateData->piValue = this->ConstrainOuput(this->pidStateData->pValue + this->pidStateData->iValue);
    } 
}

void PID::CalculateP(float error)
{
    if (this->pidStateData != NULL)
    {
        this->pidStateData->pValue = (float)error * this->pidStateData->proportionalGain;
    }
}

void PID::CalculateI(float error, float timeDelta)
{
     if (this->pidStateData != NULL)
     {
        if((this->pidStateData->integralGain != 0) && (timeDelta != 0)) 
        {
            this->pidStateData->iValue += ((float)error * this->pidStateData->integralGain) * timeDelta;
            
            if (this->pidStateData->iValue < -this->pidStateData->maxIntegral) 
            {
                this->pidStateData->iValue = -this->pidStateData->maxIntegral;
            } 
            else if (this->pidStateData->iValue > this->pidStateData->maxIntegral) 
            {
                this->pidStateData->iValue = this->pidStateData->maxIntegral;
            }
        }
        else
        {
            this->pidStateData->iValue = 0.0;
        }
    }
}

void PID::CalculateD(float error, float timeDelta)
{
    if (this->pidStateData != NULL)
    {
        if ((this->pidStateData->differentalGain != 0) && (timeDelta != 0)) 
        {
            float derivative;
            if (isnan(this->pidStateData->lastDerivativeInputForFilter)) 
            {
                // we've just done a reset, suppress the first derivative
                // term as we don't want a sudden change in input to cause
                // a large D output change          
                derivative = 0;
                this->pidStateData->lastDerivativeInputForFilter = 0;
            } 
            else 
            {
                // calculate instantaneous derivative
                derivative = (error - this->pidStateData->lastDerivativeInput) / timeDelta;
            }

            // discrete low pass filter, cuts out the
            // high frequency noise that can drive the controller crazy
            derivative = this->pidStateData->lastDerivativeInputForFilter +
                      (timeDelta / ( AC_PID_D_TERM_FILTER + timeDelta)) * (derivative - this->pidStateData->lastDerivativeInputForFilter);

            // update state
            this->pidStateData->lastDerivativeInput = error;
            this->pidStateData->lastDerivativeInputForFilter = derivative;

            // add in derivative component
            this->pidStateData->dValue = this->pidStateData->differentalGain * derivative;
        }
        else
        {
            this->pidStateData->dValue = 0.0; 
        }
    }
}

void PID::CalculateLeakyI(float error, float timeDelta, float leak_rate)
{
    if (this->pidStateData != NULL)
    {
        if((this->pidStateData->integralGain != 0) && (timeDelta != 0))
        {
            this->pidStateData->iValue -= (float)this->pidStateData->iValue * leak_rate;
            this->pidStateData->iValue += ((float)error * this->pidStateData->integralGain) * timeDelta;
            
            if (this->pidStateData->iValue < -this->pidStateData->maxIntegral) 
            {
                this->pidStateData->iValue = -this->pidStateData->maxIntegral;
            } 
            else if (this->pidStateData->iValue> this->pidStateData->maxIntegral) 
            {
                this->pidStateData->iValue = this->pidStateData->maxIntegral;
            }
        }
        else
        {
            this->pidStateData->iValue = 0.0;
        }
    }
}

void PID::ResetIntegrator()
{
    if (this->pidStateData != NULL)
    {
        this->pidStateData->iValue = 0.0;
        this->pidStateData->lastDerivativeInputForFilter = NAN;
    }
    
}