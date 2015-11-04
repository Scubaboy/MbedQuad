#include "FlightDataSetup.h"
#include "PWMTypes.h"

void FlightDataSetup::Initialise(SensorFlightData::SensorFlightDataStruct* flightData)
{
    //Initialise gyro flightData
    flightData->gyroFlightData->scaledXAxisRate = 0.0;
    flightData->gyroFlightData->scaledYAxisRate = 0.0;
    flightData->gyroFlightData->scaledZAxisRate = 0.0; 
    flightData->gyroFlightData->scaledXAxisRateCentiDegrees = 0.0;
    flightData->gyroFlightData->scaledYAxisRateCentiDegrees = 0.0;
    flightData->gyroFlightData->scaledZAxisRateCentiDegrees = 0.0; 
    flightData->gyroFlightData->rawXAxisRate = 0.0;
    flightData->gyroFlightData->rawYAxisRate = 0.0;
    flightData->gyroFlightData->rawZAxisRate = 0.0;  
    
    //Initialise accelerometer flightdata
    flightData->accelFlightData->rawXAxis = 0.0;
    flightData->accelFlightData->rawYAxis = 0.0;
    flightData->accelFlightData->rawZAxis = 0.0;  
    flightData->accelFlightData->scaledXAxis = 0.0;
    flightData->accelFlightData->scaledYAxis = 0.0;
    flightData->accelFlightData->scaledZAxis = 0.0;
    
    //Initialise magtrometer flightdata
    flightData->magFlightData->scaledXAxis = 0.0;
    flightData->magFlightData->scaledYAxis = 0.0;
    flightData->magFlightData->scaledZAxis = 0.0;
    flightData->magFlightData->rawXAxis = 0.0;
    flightData->magFlightData->rawYAxis = 0.0;
    flightData->magFlightData->rawZAxis = 0.0;  
    flightData->magFlightData->rawHeading = 0.0;
    flightData->magFlightData->scaledHeading = 0.0;
    
    //Initialise IMU flightdata
    flightData->imuFlightData->pitchAngle = 0.0;
    flightData->imuFlightData->rollAngle = 0.0;
    flightData->imuFlightData->yawAngle = 0.0; 
    flightData->imuFlightData->sinRoll = 0.0;
    flightData->imuFlightData->sinPitch = 0.0;
    flightData->imuFlightData->sinYaw = 0.0;
    flightData->imuFlightData->cosYaw = 0.0;
    flightData->imuFlightData->cosPitch = 0.0;
    flightData->imuFlightData->cosRoll = 0.0;
    flightData->imuFlightData->pitchCentiAngle = 0.0;
    flightData->imuFlightData->rollCentiAngle = 0.0;
    flightData->imuFlightData->yawCentiAngle = 0.0;
    flightData->imuFlightData->valid = false;
    
    //Initialise Altimeter flightdata
    flightData->altimeterFlightData->pressure = 0.0;
    flightData->altimeterFlightData->temperature = 0.0;
    flightData->altimeterFlightData->altitude = 0.0;
    
    //Initialise flightpack flightdata
    flightData->flightPackData->cell1Voltage = 0.0;
    flightData->flightPackData->cell1_2Voltage = 0.0;
    flightData->flightPackData->cell1_3Voltage = 0.0;
    flightData->flightPackData->status = FlightPack3CellData::Unknown;
    
    //Initialise GPS position flightdata
    flightData->gpsPositionData->latitude = 0.0;
    flightData->gpsPositionData->longitude = 0.0;
    flightData->gpsPositionData->mslAltitude = 0.0; 
    flightData->gpsPositionData->groundSpeedms = 0.0;
    flightData->gpsPositionData->headingDegrees = 0.0;
    flightData->gpsPositionData->satellities = 0; 
    flightData->gpsPositionData->fixType = 0;
    flightData->gpsPositionData->dateDDMMYY = 0;
    flightData->gpsPositionData->utcTime = 0.0; 
    flightData->gpsPositionData->hdop = 0.0;
    flightData->gpsPositionData->ck_A = 0;
    flightData->gpsPositionData->ck_B = 0;
}

void FlightDataSetup::Initialise(QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct* motorFlightData)
{
    //Initialise default motor frame configuration flat X cnofig
    motorFlightData->frameMappings[QuadMotorTypes::Motor1].id = QuadMotorTypes::Motor1;
    motorFlightData->frameMappings[QuadMotorTypes::Motor1].orientation = QuadMotorTypes::MotorZero;//QuadMotorTypes::UpperRightQuad;
    motorFlightData->frameMappings[QuadMotorTypes::Motor1].yawFactor = QuadMotorTypes::YawFactorACW;
    motorFlightData->frameMappings[QuadMotorTypes::Motor1].testOrder = QuadMotorTypes::Test1;
    
    motorFlightData->frameMappings[QuadMotorTypes::Motor2].id = QuadMotorTypes::Motor2;
    motorFlightData->frameMappings[QuadMotorTypes::Motor2].orientation = QuadMotorTypes::MotorOne;//QuadMotorTypes::LowerLeftQuad;
    motorFlightData->frameMappings[QuadMotorTypes::Motor2].yawFactor = QuadMotorTypes::YawFactorACW;
    motorFlightData->frameMappings[QuadMotorTypes::Motor2].testOrder = QuadMotorTypes::Test2;
    
    motorFlightData->frameMappings[QuadMotorTypes::Motor3].id = QuadMotorTypes::Motor3;
    motorFlightData->frameMappings[QuadMotorTypes::Motor3].orientation = QuadMotorTypes::MotorTwo;//QuadMotorTypes::UpperLeftQuad;
    motorFlightData->frameMappings[QuadMotorTypes::Motor3].yawFactor = QuadMotorTypes::YawFactorCW;
    motorFlightData->frameMappings[QuadMotorTypes::Motor3].testOrder = QuadMotorTypes::Test3;
    
    motorFlightData->frameMappings[QuadMotorTypes::Motor4].id = QuadMotorTypes::Motor4;
    motorFlightData->frameMappings[QuadMotorTypes::Motor4].orientation = QuadMotorTypes::MotorThree;//QuadMotorTypes::LowerRightQuad;
    motorFlightData->frameMappings[QuadMotorTypes::Motor4].yawFactor = QuadMotorTypes::YawFactorCW;
    motorFlightData->frameMappings[QuadMotorTypes::Motor4].testOrder = QuadMotorTypes::Test4;
    
    //initialise Motor properties
    motorFlightData->motorProperties[QuadMotorTypes::Motor1].maxRateCtrl = 4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor1].minRateCtrl = -4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor1].armed = false;
    
    motorFlightData->motorProperties[QuadMotorTypes::Motor2].maxRateCtrl = 4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor2].minRateCtrl = -4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor2].armed = false;
    
    motorFlightData->motorProperties[QuadMotorTypes::Motor3].maxRateCtrl = 4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor3].minRateCtrl = -4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor3].armed = false;
    
    motorFlightData->motorProperties[QuadMotorTypes::Motor4].maxRateCtrl = 4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor4].minRateCtrl = -4500;
    motorFlightData->motorProperties[QuadMotorTypes::Motor4].armed = false;
      
    //Initialise PWM Channel motor mappings;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor1].channelID = PWMTypes::channel1;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor1].channelDef.channelOutMax =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor1].channelDef.channelOutMin =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor1].channelDef.channelOutThrMin = 0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor1].channelDef.channelOutThrMax = 0;
    
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor2].channelID = PWMTypes::channel2;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor2].channelDef.channelOutMax =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor2].channelDef.channelOutMin =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor2].channelDef.channelOutThrMin = 0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor2].channelDef.channelOutThrMax = 0;
    
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor3].channelID = PWMTypes::channel3;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor3].channelDef.channelOutMax =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor3].channelDef.channelOutMin =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor3].channelDef.channelOutThrMin = 0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor3].channelDef.channelOutThrMax = 0;
    
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor4].channelID = PWMTypes::channel4;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor4].channelDef.channelOutMax =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor4].channelDef.channelOutMin =0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor4].channelDef.channelOutThrMin = 0;
    motorFlightData->pwmChannelMotorMapping[QuadMotorTypes::Motor4].channelDef.channelOutThrMax = 0;
}

void FlightDataSetup::Initialise(FlightMode::FlightModeStruct* flightMode)
{
    flightMode->mode = FlightMode::StartUp;
    flightMode->previousMode = FlightMode::NotSet;
}

void FlightDataSetup::Initialise(UserResponce::UserResponceStruct* userResponce, 
                                 DataRequestRecv::DataRequestRecvs* dataRequestRcvs, 
                                 SystemStatusAck::SystemStatusAckStruct* systemStatusAck)
{
    userResponce->userAck = false;
    dataRequestRcvs->altDataReqRcvs.pressureAtSeaLevelRecvd = false; 
    //dataRequest->motorConfigDataReceived = false;
    //dataRequest->RatePitchCtrlConfigDataReceived = false; 
    //dataRequest->RateRollCtrlConfigDataReceived = false;
    //dataRequest->RateYawCtrlConfigDataReceived = false;
    //dataRequest->StabPitchCtrlConfigDataReceived = false; 
    //dataRequest->StabRollCtrlConfigDataReceived = false;
    //dataRequest->StabYawCtrlConfigDataReceived = false;
    systemStatusAck->acked = false;
}

void FlightDataSetup::Configure(SensorFlightData::SensorFlightDataStruct* flightData,
                   AccelFlightData::AccelFlightDataStruct* accelFlightData,
                   AltMeterFlightData::AltMeterFlightDataStruct* altimeterFlightData,
                   GyroFlightData::GyroFlightDataStruct* gyroFlightData,
                   MagFlightData::MagFlightDataStruct* magFlightData,
                   IMUFlightData::IMUFlightDataStruct* imuFlightData,
                   FlightPack3CellData::FlightPack3CellDataStruct* flightPackData,
                   GPSPositionData::GPSPositionDataStruct* gpsPositionData)
{
    flightData->accelFlightData = accelFlightData;
    flightData->altimeterFlightData = altimeterFlightData;
    flightData->gyroFlightData = gyroFlightData;
    flightData->magFlightData = magFlightData;
    flightData->imuFlightData = imuFlightData;
    flightData->flightPackData = flightPackData;
    flightData->gpsPositionData = gpsPositionData;
}