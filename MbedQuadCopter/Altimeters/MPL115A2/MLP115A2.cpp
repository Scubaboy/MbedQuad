#include "MLP115A2.h"
#include "mbed.h"
#include "MPL115A2Cal.h"
#include "MPL115A2Registers.h"

const int I2CBaseAddr = 0x60 << 1;
const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::Alt;
const MPL115A2Cal defaultAltCall = MPL115A2Cal(NULL,NULL);
const float Multiplier = 44330.0;
const double Power = 0.190295;

MLP115A2::MLP115A2(AltimeterCalBase* calObj,I2CBusCtrl* busCtrl, AltimeterStatus::AltimeterStatusStruct* altStatus, AltMeterFlightData::AltMeterFlightDataStruct* flightData)
{
    if (calObj != NULL)
    {
        this->calObj = calObj;
    }
    else
    {
        this->calObj = &defaultAltCall;
    }
    
    if (altStatus != NULL)
    {
        this->altStatus = altStatus; 
        this->altStatus->calibrationComplete = false;
        this->altStatus->readyForUse = false;  
    }
    else
    {
        this->altStatus = NULL;
    }
    
    this->busCtrl = busCtrl;
    this->flightData = flightData;
    this->sensorReady = false;
}

void MLP115A2::Init()
{
    short sia0MSB, sia0LSB;
    short sib1MSB, sib1LSB;
    short sib2MSB, sib2LSB;
    short sic12MSB, sic12LSB;
    short sic11MSB, sic11LSB;
    short sic22MSB, sic22LSB;
    int i2cResult;
    
    //send request for coefficients
    char cmd[1];
    cmd[0] = MPL115A2Registers::A0_MSB;
    
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::Alt))
        {
            i2cResult = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, cmd, 1);

            // read out coefficients
            char coeff[12];
            i2cResult = this->busCtrl->ReadFromBus(busUser, I2CBaseAddr, coeff, 12);
    
            if (i2cResult == 0)
            {
                sia0MSB = coeff[0];
                sia0LSB = coeff[1];
                sib1MSB = coeff[2];
                sib1LSB = coeff[3];
                sib2MSB = coeff[4];
                sib2LSB = coeff[5];
                sic12MSB = coeff[6];
                sic12LSB = coeff[7];
                sic11MSB = coeff[8];
                sic11LSB = coeff[9];
                sic22MSB = coeff[10];
                sic22LSB = coeff[11];
    
    
                // Placing coefficients into 16-bit Variables
                // a0
                sia0 = (signed int) sia0MSB << 8;
                sia0 += (signed int) sia0LSB & 0x00FF;
        
                // b1
                sib1 = (signed int) sib1MSB << 8;
                sib1 += (signed int) sib1LSB & 0x00FF;
        
                // b2
                sib2 = (signed int) sib2MSB << 8;
                sib2 += (signed int) sib2LSB & 0x00FF;
        
                // c12
                sic12 = (signed int) sic12MSB << 8;
                sic12 += (signed int) sic12LSB & 0x00FF;
        
                // c11
                sic11 = (signed int) sic11MSB << 8;
                sic11 += (signed int) sic11LSB & 0x00FF;
        
                // c22
                sic22 = (signed int) sic22MSB << 8;
                sic22 += (signed int) sic22LSB & 0x00FF;
            }
            else
            {
                if (this->altStatus != NULL)
                {
                    sprintf(this->altStatus->msg,"Failed : To read coefficients %d\0",i2cResult);
                }
            }
            
            this->busCtrl->ReleaseCtrl(BusCtrlUsers::Alt);
        }
        else
        {
            
        }
    }
    else
    {
        if (this->altStatus != NULL)
        {
            sprintf(this->altStatus->msg,"Failed : i2c bus controller NULL\0");
        }
    }
}

void MLP115A2::UpdateAltitude()
{
    if (this->flightData != NULL)
    {
        this->UpdatePresure();
        this->flightData->altitude = (Multiplier *(1-((float)pow((double)(this->flightData->pressure/this->calObj->GetPressureOffset()),Power)))) - 
                                        this->calObj->GetAltitudeOffset();
        
        //((((float)pow(((double)this->calObj->GetPressureOffset() / (double)this->flightData->pressure), 1/5.257) - 1.0) * 
        //                            (this->flightData->temperature + 273.15)) / 0.0065) - this->calObj->GetAltitudeOffset();       
    }
    
}

void MLP115A2::SetOffsets()
{
    //Read resting altitude  
    this->ReadSensor(); 
    this->UpdatePresure();
    this->UpdateAltitude();
   
}

void MLP115A2::UpdatePresure()
{
    short siPcomp;
    int lt1, lt2, lt3, sic11x1, sia11, sic12x2;
    int sia1, sic22x2, sia2, sia1x1, siy1, sia2x2;

        // Step 1 c11x1 = c11 * Padc
        lt1 = (int) sic11;
        lt2 = (int) uiPadc;
        lt3 = lt1*lt2;
        sic11x1 = (int) lt3;
    
        // Step 2 a11 = b1 + c11x1
        lt1 = ((int)sib1)<<14;
        lt2 = (int) sic11x1;
        lt3 = lt1 + lt2;
        sia11 = (int)(lt3>>14);
    
        // Step 3 c12x2 = c12 * Tadc
        lt1 = (int) sic12;
        lt2 = (int) uiTadc;
        lt3 = lt1*lt2;
        sic12x2 = (int)lt3;
    
        // Step 4 a1 = a11 + c12x2
        lt1 = ((int)sia11<<11);
        lt2 = (int)sic12x2;
        lt3 = lt1 + lt2;
        sia1 = (int) lt3>>11;
    
        // Step 5 c22x2 = c22*Tadc
        lt1 = (int)sic22;
        lt2 = (int)uiTadc;
        lt3 = lt1 * lt2;
        sic22x2 = (int)(lt3);
    
        // Step 6 a2 = b2 + c22x2
        lt1 = ((int)sib2<<15);
        lt2 = ((int)sic22x2>1);
        lt3 = lt1+lt2;
        sia2 = ((int)lt3>>16);
    
        // Step 7 a1x1 = a1 * Padc
        lt1 = (int)sia1;
        lt2 = (int)uiPadc;
        lt3 = lt1*lt2;
        sia1x1 = (int)(lt3);
    
        // Step 8 y1 = a0 + a1x1
        lt1 = ((int)sia0<<10);
        lt2 = (int)sia1x1;
        lt3 = lt1+lt2;
        siy1 = ((int)lt3>>10);
    
        // Step 9 a2x2 = a2 * Tadc
        lt1 = (int)sia2;
        lt2 = (int)uiTadc;
        lt3 = lt1*lt2;
        sia2x2 = (int)(lt3);
    
        // Step 10 pComp = y1 + a2x2
        lt1 = ((int)siy1<<10);
        lt2 = (int)sia2x2;
        lt3 = lt1+lt2;
    
        // Fixed point result with rounding
        siPcomp = ((int)lt3>>13);
    
        // decPcomp is defined as a floating point number
        // Conversion to decimal value from 1023 ADC count value
        // ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively
        if (this->flightData != NULL)
        {
            this->flightData->pressure = (((650.0/1023.0)*siPcomp)+500);
        }
    
}

void MLP115A2::UpdateTemperature()
{
    
        if (this->flightData != NULL)
        {
            this->flightData->temperature = (25 + ((uiTadc - 498.0) / -5.35));   
        }
    
}

bool MLP115A2::ReadSensor()
{
    // start AD conversions
    int i2cResult;
    char cmd[2];
    bool result = true;
    
    cmd[0]=MPL115A2Registers::CONVERT;
    cmd[1]=0x01;
    
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::Alt))
        {
            i2cResult = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, cmd , 2);
    
            if(i2cResult == 0)
            {
                wait(0.01);
                cmd[0]=MPL115A2Registers::PADC_MSB;
        
                i2cResult = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, cmd, 1);
    
                if(i2cResult == 0)
                {
                    // compensation
                    char data[4];   
                    i2cResult = this->busCtrl->ReadFromBus(busUser, I2CBaseAddr, data, 4);
            
                    if(i2cResult == 0)
                    {   
                        uiPadc = (unsigned short) data[0] << 8;
                        uiPadc += (unsigned short) data[1] & 0x00FF;
                        uiTadc = (unsigned short) data[2] << 8;
                        uiTadc += (unsigned short) data[3] & 0x00FF;
        
                        // Coefficient 9 equation compensation
                        uiPadc = uiPadc >> 6;
                        uiTadc = uiTadc >> 6;
                    }
                    else
                    {
                        result = false;
                    
                        if (this->altStatus != NULL)
                        {
                            //sprintf(this->altStatus->msg,"Failed : Read temp and pressure register %d\0",i2cResult);
                        }
                    }   
                }
                else
                {
                    result = false;
                
                    if (this->altStatus != NULL)
                    {
                        //sprintf(this->altStatus->msg,"Failed : Select temp and pressure register %d\0",i2cResult);
                    }
                }
                
                this->busCtrl->ReleaseCtrl(BusCtrlUsers::Alt); 
            }
            else
            {
                
            }
        }
        else
        {
            result = false;
            
            if (this->altStatus != NULL)
            {
                //sprintf(this->altStatus->msg,"Failed : Requested temp and pressure %d\0",i2cResult);
            }
        }
    } 
    
    return result;    
}