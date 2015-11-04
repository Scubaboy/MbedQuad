#include "MPU9150.h"
#include "AngleConverters.h"

MPU9150::MPU9150(
                 MPUBase* mpu,
                 DebugLoggerBase* debugLogger, 
                 SensorFlightData::SensorFlightDataStruct* flightData,
                 IMUStatus::IMUStatusStruct* imuStatus)
{
    this->busCtrl = busCtrl;
    this->mpu = mpu;
    
    if (flightData != NULL)
    {
        this->flightData = flightData->imuFlightData;   
        this->gyroFlightData = flightData->gyroFlightData;
        this->accelFlightData = flightData->accelFlightData;  
        this->magFlightData = flightData->magFlightData; 
    }
    
    this->imuStatus = imuStatus;
    this->debugLogger = debugLogger;
}

void MPU9150::UpdateEulerAngles(float timeDelta)
{
        this->mpu->GetUpdate(&this->gyroRaw, 
                             &this->gyroScaled, 
                             &this->accelRaw, 
                             &this->accelScaled,
                             &this->magRaw,
                             &this->magScaled,
                             &this->eulerAngles,
                             &this->quat);
     
        this->flightData->pitchAngle = AngleConverters::RadiansToDegrees(this->eulerAngles.x);
        this->flightData->rollAngle  = AngleConverters::RadiansToDegrees(this->eulerAngles.y);
        this->flightData->yawAngle  =  AngleConverters::RadiansToDegrees(this->eulerAngles.z);
        this->flightData->pitchCentiAngle = AngleConverters::DegreesToCentiDegrees(this->flightData->pitchAngle);
        this->flightData->rollCentiAngle = AngleConverters::DegreesToCentiDegrees(this->flightData->rollAngle);
        this->flightData->yawCentiAngle = AngleConverters::DegreesToCentiDegrees(this->flightData->yawAngle);
        
        //Need to handle sensor data..... Lookup accel and gyro info.
        //Gyro flight data
        this->gyroFlightData->scaledXAxisRate = AngleConverters::RadiansToDegrees(this->gyroScaled.x);
        this->gyroFlightData->scaledYAxisRate = AngleConverters::RadiansToDegrees(this->gyroScaled.y);
        this->gyroFlightData->scaledZAxisRate = AngleConverters::RadiansToDegrees(this->gyroScaled.z);            
        this->gyroFlightData->scaledXAxisRateCentiDegrees = AngleConverters::DegreesToCentiDegrees(this->gyroScaled.x);
        this->gyroFlightData->scaledYAxisRateCentiDegrees = AngleConverters::DegreesToCentiDegrees(this->gyroScaled.y);
        this->gyroFlightData->scaledZAxisRateCentiDegrees = AngleConverters::DegreesToCentiDegrees(this->gyroScaled.z);
        this->gyroFlightData->rawXAxisRate = this->gyroRaw.x;
        this->gyroFlightData->rawYAxisRate = this->gyroRaw.y;
        this->gyroFlightData->rawZAxisRate = this->gyroRaw.z;
        
        //Accel flight data
        this->accelFlightData->rawXAxis = this->accelRaw.x;
        this->accelFlightData->rawYAxis = this->accelRaw.y;
        this->accelFlightData->rawZAxis = this->accelRaw.z;
        this->accelFlightData->scaledXAxis = this->accelScaled.x;
        this->accelFlightData->scaledYAxis = this->accelScaled.y;
        this->accelFlightData->scaledZAxis = this->accelScaled.z;
        
        //Mag flight data
        this->magFlightData->rawXAxis = this->magRaw.x;
        this->magFlightData->rawYAxis = this->magRaw.y;
        this->magFlightData->rawZAxis = this->magRaw.z;
        this->magFlightData->scaledXAxis = this->magScaled.x;
        this->magFlightData->scaledYAxis = this->magScaled.y;
        this->magFlightData->scaledZAxis = this->magScaled.z;
        
        //Send info to logger
        if (this->debugLogger != NULL)
        {
            char data[100];
            memset(data,'0',sizeof(data));
            sprintf(data,"Roll<%.1f>,Pitch<%.1f>,Yaw<%.1f>\r\n\0",this->flightData->pitchAngle,this->flightData->rollAngle,this->flightData->yawAngle);
            this->debugLogger->Log(data);   
            memset(data,'0',sizeof(data));
            sprintf(data,"GX<%.1f>\r\n\0",this->gyroScaled.x);
            this->debugLogger->Log(data);
        }
}

void MPU9150::Init()
{
    
    this->mpu->Init();
}


void MPU9150::CallCriticalStop()
{
    
}