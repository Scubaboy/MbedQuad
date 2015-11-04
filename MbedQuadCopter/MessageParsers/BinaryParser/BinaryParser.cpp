#include "BinaryParser.h"
#include "mbed.h"
#include "IMUSensorDataPck.h"
#include "FlightPackDataPck.h"
#include "RateDataPck.h"
#include "StabCtrlDataPck.h"
#include "QuadMotorsDataPck.h"
#include "IMUSensorDataPck.h"
#include "ShutdownDataPck.h"
#include "UserResponceDataPck.h"
#include "DataRequestReceivedPck.h"
#include "SystemStatusAckDataPck.h"
#include "DataLoggerDataPck.h"
#include "SelectedSysConfigDataPck.h"
#include "SetModeDataPck.h"
#include "FlightCtrlActionDataPck.h"
#include "ManualFlightDataDataPck.h"
#include "HeartBeatDataPck.h"
#include "SyncTimeDataPck.h"

DataPckTypeBase::DataPck* BinaryParser::Parse(ReceiveDataPacket::ReceiveDataPacketStruct* dataPck)
{
    DataPckTypeBase::DataPck* data = NULL;
     
    switch (*(int*)dataPck->dataPack)
    {
        case DataPckTypes::Sync:
        {
            break;   
        }

        case DataPckTypes::RateCtrlRoll:
        {
            data = this->BuildRateCtrlDataPck(&dataPck->dataPack[8], DataPckTypes::RateCtrlRoll);
            break;
        }
        case DataPckTypes::RateCtrlPitch:
        {
            data = this->BuildRateCtrlDataPck(&dataPck->dataPack[8], DataPckTypes::RateCtrlPitch);
            break;   
        }
        case DataPckTypes::RateCtrlYaw:
        {
            data = this->BuildRateCtrlDataPck(&dataPck->dataPack[8], DataPckTypes::RateCtrlYaw);
            break;   
        }
        case DataPckTypes::StabCtrlRoll: 
        {
            data = this->BuildStabCtrlDataPck(&dataPck->dataPack[8], DataPckTypes::StabCtrlRoll);
            break;
        }
        case DataPckTypes::StabCtrlPitch: 
        {
            data = this->BuildStabCtrlDataPck(&dataPck->dataPack[8], DataPckTypes::StabCtrlPitch);
            break;
        }
        case DataPckTypes::StabCtrlYaw: 
        {
            data = this->BuildStabCtrlDataPck(&dataPck->dataPack[8], DataPckTypes::StabCtrlYaw);
            break;
        }
        case DataPckTypes::QuadMotors: 
        {
            data = this->BuildQuadMotorDataPck(&dataPck->dataPack[8]);
            break;
        }
        case DataPckTypes::IMUSensors: 
        {
            data = this->BuildIMUSensorDataPck(&dataPck->dataPack[8]);
            break;
        }
        case DataPckTypes::FlightPan:
        {
            data = this->BuildFlightPlanDataPck(&dataPck->dataPack[8]);
            break;
        }
        case DataPckTypes::FlightPack:
        {
            data = this->BuildFlightPackDataPck(&dataPck->dataPack[8]);
            break;
        }
 
        case DataPckTypes::UserResponce:
        {
            data = this->BuildUserResponceDataPck(&dataPck->dataPack[8]);;
            break;   
        }
        case DataPckTypes::RequestDataReceived:
        {
            data = this->BuildDataRequestReceivedPck(&dataPck->dataPack[8]);
            break;   
        }
        case DataPckTypes::SystemStatusAck:
        {
            data = this->BuildSystemStatusAckReceivedPck(&dataPck->dataPack[8]);
            break; 
        }
        case DataPckTypes::ShutDown:
        {
            data = this->BuildShutdownDataPck(&dataPck->dataPack[8]);
            break;
        }
        case DataPckTypes::DataLogger:
        {
            data = this->BuildDataLoggerReceivedPck(&dataPck->dataPack[8]);
            break;
        }
        case DataPckTypes::SelectedSysConfMsg:
        {
            data = this->BuildSelectedSysConfigPck(&dataPck->dataPack[8]);
            break;   
        }
        case DataPckTypes::SetModeMsg:
        {
            data = this->BuildSetModePck(&dataPck->dataPack[8]);
            break;   
        }
        case DataPckTypes::FlightCtrlAction:
        {
            data = this->BuildFlightCtrlActionDataPck(&dataPck->dataPack[8]);
            break;   
        }
        case DataPckTypes::ManualFlightData:
        {
            data = this->BuildManualFlightDataPck(&dataPck->dataPack[8]);
            break;
        }
        case DataPckTypes::HeartBeat:
        {
            data = new HeartBeatDataPck::HeartBeatData;
            data->type = DataPckTypes::HeartBeat;
            break;   
        }
        case DataPckTypes::SyncTime:
        {
            data = this->BuildTimeSyncDataPck(&dataPck->dataPack[12]);
            break;   
        }
    }
     
    return data;
}

DataPckTypeBase::DataPck* BinaryParser::BuildManualFlightDataPck(const char* manualFlightData)
{
    ManualFlightDataDataPck::ManualFlightData* manualFlight = new ManualFlightDataDataPck::ManualFlightData;
    
    manualFlight->type = DataPckTypes::ManualFlightData;
    manualFlight->requestedRoll = *((float*)manualFlightData);
    manualFlightData += sizeof(float);
    manualFlight->requestedPitch = *((float*)manualFlightData);
    manualFlightData += sizeof(float);
    manualFlight->requestedYaw = *((float*)manualFlightData);
    manualFlightData += sizeof(float);
    manualFlight->requestedThrottle = *((float*)manualFlightData);
    
    return manualFlight;
}

DataPckTypeBase::DataPck* BinaryParser::BuildFlightCtrlActionDataPck(const char* flightCtrlActionData)
{
    FlightCtrlActionDataPck::FlightCtrlAction* flightCtrlAction = new FlightCtrlActionDataPck::FlightCtrlAction;
    
    flightCtrlAction->type = DataPckTypes::FlightCtrlAction;
    flightCtrlAction->action = *((HighLevelCtrlActionTypes::FlightAction*)flightCtrlActionData);
    flightCtrlActionData += sizeof(HighLevelCtrlActionTypes::FlightAction);
    flightCtrlAction->request = *((HighLevelCtrlActionTypes::FlightActionRequest*)flightCtrlActionData);
    
    return flightCtrlAction;
}

DataPckTypeBase::DataPck* BinaryParser::BuildSetModePck(const char* setModeData)
{
     SetModeDataPck::SetMode* setMode = new SetModeDataPck::SetMode;
     
     setMode->type = DataPckTypes::SetModeMsg;
     setMode->mode = *((unsigned int*)setModeData);
     
     return setMode;
}

DataPckTypeBase::DataPck* BinaryParser::BuildSelectedSysConfigPck(const char* selectedSysConfig)
{
    SelectedSysConfigDataPck::SelectedSysConfig* selectedSysConf = new SelectedSysConfigDataPck::SelectedSysConfig;
    
    selectedSysConf->type = DataPckTypes::SelectedSysConfMsg;
    selectedSysConf->telemtryFeeds = *((SystemConfigurationTypes::TelemetryFeeds*)selectedSysConfig);
    selectedSysConfig += sizeof(SystemConfigurationTypes::TelemetryFeeds);
    selectedSysConf->gpsMsgFormat = *((SystemConfigurationTypes::GPSMsgFormat*)selectedSysConfig);
    selectedSysConfig += sizeof(SystemConfigurationTypes::GPSMsgFormat);
    selectedSysConf->imu = *((SystemConfigurationTypes::IMU*)selectedSysConfig);
    
    return selectedSysConf;
}

DataPckTypeBase::DataPck* BinaryParser::BuildSystemStatusAckReceivedPck(const char* systemStatusAck)
{
    SystemStatusAckDataPck::SystemStatusAck* dataRequestReceived = new SystemStatusAckDataPck::SystemStatusAck;
    
    dataRequestReceived->type = DataPckTypes::SystemStatusAck;
    dataRequestReceived->ack = *((bool*)systemStatusAck);
    
    return dataRequestReceived;
}

DataPckTypeBase::DataPck* BinaryParser::BuildDataRequestReceivedPck(const char* dataRequest)
{
    DataRequestReceivedPck::DataRequestReceived* dataRequestReceived = new DataRequestReceivedPck::DataRequestReceived;
    
    dataRequestReceived->type = DataPckTypes::RequestDataReceived;
    dataRequestReceived->pressureAtSealevel = *((float*)dataRequest);
    
    return dataRequestReceived;
}

DataPckTypeBase::DataPck* BinaryParser::BuildUserResponceDataPck(const char* userResponceData)
{
    UserResponceDataPck::UserResponceData* userResponce = new UserResponceDataPck::UserResponceData;
    
    userResponce->type = DataPckTypes::UserResponce;
    userResponce->requestAck = *((bool*)userResponceData);
    
    return userResponce;
}



DataPckTypeBase::DataPck* BinaryParser::BuildFlightPackDataPck(const char* flightData)
{
    FlightPackDataPck::FlightPack* flightPack = new FlightPackDataPck::FlightPack;
    
    flightPack->type = DataPckTypes::FlightPack;
    flightPack->cell1VoltageLoss = *((float*)flightData);
    flightData += sizeof(float);
    flightPack->cell1_2VoltageLoss = *((float*)flightData);
    flightData += sizeof(float);
    flightPack->cell1_3VoltageLoss = *((float*)flightData);
    
    return flightPack;
}

DataPckTypeBase::DataPck* BinaryParser::BuildRateCtrlDataPck(const char* rateData, DataPckTypes::DataPckID rateCtrlType)
{
    RateDataPck::Rate* rate = new RateDataPck::Rate;
    
    rate->type = rateCtrlType;
    rate->proportionalGain = *((float*)rateData);
    rate += sizeof(float);
    rate->integralGain = *((float*)rateData);
    rate += sizeof(float);
    rate->differentalGain = *((float*)rateData);
    rate += sizeof(float);
    rate->maxIntegral = *((int*)rateData);
    return rate;  
}

DataPckTypeBase::DataPck* BinaryParser::BuildStabCtrlDataPck(const char* stabCtrlData, DataPckTypes::DataPckID stabCtrlType)
{
    StabCtrlDataPck::StabCtrl* stabCtrl = new StabCtrlDataPck::StabCtrl;
    
    stabCtrl->type = stabCtrlType;
    stabCtrl->proportionalGain = *((float*)stabCtrlData);
    
    return stabCtrl;
}

DataPckTypeBase::DataPck* BinaryParser::BuildQuadMotorDataPck(const char* quadMotorData)
{
    QuadMotorsDataPck::Motors* quadMotors = new QuadMotorsDataPck::Motors;
   
    quadMotors->type = DataPckTypes::QuadMotors;
    memcpy(quadMotors->frameMappings,quadMotorData,sizeof (QuadMotorMappingDefinition::MotorFrameMapping));
    quadMotorData += sizeof (QuadMotorMappingDefinition::MotorFrameMapping);
    memcpy(quadMotors->motorProperties,quadMotorData,sizeof (QuadMotorMappingDefinition::MotorProperties));
    quadMotorData += sizeof (QuadMotorMappingDefinition::MotorProperties);
    memcpy(&quadMotors->gloabalLimits,quadMotorData,sizeof (QuadMotorProperties::GlobalLimits));
    quadMotorData += sizeof (QuadMotorProperties::GlobalLimits);
    memcpy(quadMotors->pwmChannelMotorMapping,quadMotorData,sizeof (PWMChannelMotorMapping::ChannelMotorMappingDefinition));

    return quadMotors;
}

DataPckTypeBase::DataPck* BinaryParser::BuildIMUSensorDataPck(const char* imuSensorData)
{
    IMUSensorDataPck::IMUSensor* imuSensor = new IMUSensorDataPck::IMUSensor;
    
    imuSensor->type = DataPckTypes::IMUSensors;
    memcpy(&imuSensor->accelCal,imuSensorData,sizeof (XYZAxisMinMax::XYZAxisMinMaxCalData));
    imuSensorData += sizeof (XYZAxisMinMax::XYZAxisMinMaxCalData);
    memcpy(&imuSensor->gyroCal,imuSensorData,sizeof (GyroTypes::GyroCalData));
    imuSensorData += sizeof (GyroTypes::GyroCalData);
    memcpy(&imuSensor->magCal,imuSensorData,sizeof (XYZAxisMinMax::XYZAxisMinMaxCalData));
    
    return imuSensor;
}

DataPckTypeBase::DataPck* BinaryParser::BuildFlightPlanDataPck(const char* flightPlanData)
{
    return NULL;
}


DataPckTypeBase::DataPck* BinaryParser::BuildDataLoggerReceivedPck(const char* dataLoggerData)
{
    DataLoggerDataPck::DataLogger* dataLogger = new DataLoggerDataPck::DataLogger;
    
    dataLogger->type = DataPckTypes::DataLogger;
    dataLogger->enableDisable = true;//*((bool*)&dataLoggerData[93]);
    
    return dataLogger;  
}

DataPckTypeBase::DataPck* BinaryParser::BuildShutdownDataPck(const char* shutdowndata)
{
    ShutdownDataPck::Shutdown* shutdown = new ShutdownDataPck::Shutdown;
    
    shutdown->type = DataPckTypes::ShutDown;
    shutdown->stop = *((bool*)shutdowndata);
    
    return shutdown; 
}

DataPckTypeBase::DataPck* BinaryParser::BuildTimeSyncDataPck(const char* timeSyncData)
{
    SyncTimeDataPck::SyncTimeData* timeSync = new SyncTimeDataPck::SyncTimeData;
    
    timeSync->type = DataPckTypes::SyncTime;
    timeSync->SyncSeconds = *((int*)timeSyncData);
    
    return timeSync;
}