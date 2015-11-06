#include "BinaryCmdMsgController.h"
#include "mbed.h"
#include "RateDataPck.h"
#include "StabCtrlDataPck.h"
#include "UserResponceDataPck.h"
#include "DataRequestReceivedPck.h"
#include "QuadMotorsDataPck.h"
#include "SystemStatusAckDataPck.h"
#include "DataLoggerDataPck.h"
#include "SelectedSysConfigDataPck.h"
#include "SetModeDataPck.h"
#include "HighLevelCtrlActionTypes.h"
#include "FlightCtrlActionDataPck.h"
#include "ManualFlightDataDataPck.h"
#include "SyncTimeDataPck.h"

BinaryCmdMsgController::BinaryCmdMsgController(SystemModeControllerBase* systemModeController,
                                               BaseHighLevelRateController* highLevelController, 
                                               UserResponce::UserResponceStruct* userResponce,
                                               DataRequestRecv::DataRequestRecvs* dataRequestRcvs,
                                               FlightCtrlData::FlightCtrlDataStruct* flightCtrlData,
                                               SystemStatusAck::SystemStatusAckStruct* systemStatusAck,
                                               HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus,
											   TimeSynchStatus::TimeSynchStatusStruct* timeSynchStatus,
                                               DebugLoggerBase* logger,
                                               SysCtrlConfigCtrl* selectedSysConf)
{
      this->systemModeController = systemModeController; 
      this->userResponce = userResponce;
      this->dataRequestRcvs = dataRequestRcvs;
      this->newDataPck = NULL;
      this->systemStatusAck = systemStatusAck;
      this->logger = logger;
      this->selectedSysConf = selectedSysConf;
      this->highLevelController = highLevelController;
      this->flightCtrlData = flightCtrlData;
      this->heartBeatStatus = heartBeatStatus;
      this->timeSynchStatus = timeSynchStatus;
}

void BinaryCmdMsgController::CheckForCmdMsg()
{
    if (this->msgReaderptr != NULL && this->methodCallbackReader != NULL)
    {
        if ((this->msgReaderptr->*this->methodCallbackReader)(&this->receivedCmd))
        {
          //  this->ParseDataCmd();
            
            newDataPck = (DataPckTypeBase::DataPck*)(&this->receivedCmd.dataPack);
            
          //  if (newDataPck != NULL)
          //  {
                this->ActionReceivedDataPck();
          //      delete newDataPck;
          //  }
        }   
    }
    
}

void BinaryCmdMsgController::ActionReceivedDataPck()
{
    switch (newDataPck->type)
    {
        case DataPckTypes::RateCtrlRoll:
        {
            RateDataPck::Rate* localRate = (RateDataPck::Rate*)this->newDataPck;
            
            this->dataRequestRcvs->rollRatePIDGainsRcvs.gains.maxIntegral = localRate->maxIntegral;
            this->dataRequestRcvs->rollRatePIDGainsRcvs.gains.differentalGain = localRate->differentalGain;
            this->dataRequestRcvs->rollRatePIDGainsRcvs.gains.integralGain = localRate->integralGain;
            this->dataRequestRcvs->rollRatePIDGainsRcvs.gains.proportionalGain = localRate->proportionalGain;
            this->dataRequestRcvs->rollRatePIDGainsRcvs.ratePIDGainsRecvd = true;
            break;
        }
        case DataPckTypes::RateCtrlPitch:
        {
            RateDataPck::Rate* localRate = (RateDataPck::Rate*)this->newDataPck;
            
            this->dataRequestRcvs->pitchRatePIDGainsRcvs.gains.maxIntegral = localRate->maxIntegral;
            this->dataRequestRcvs->pitchRatePIDGainsRcvs.gains.differentalGain = localRate->differentalGain;
            this->dataRequestRcvs->pitchRatePIDGainsRcvs.gains.integralGain = localRate->integralGain;
            this->dataRequestRcvs->pitchRatePIDGainsRcvs.gains.proportionalGain = localRate->proportionalGain;
            this->dataRequestRcvs->pitchRatePIDGainsRcvs.ratePIDGainsRecvd = true;
            break;
        }
        case DataPckTypes::RateCtrlYaw:
        {
            RateDataPck::Rate* localRate = (RateDataPck::Rate*)this->newDataPck;

           this->dataRequestRcvs->yawRatePIDGainsRcvs.gains.maxIntegral = localRate->maxIntegral;
            this->dataRequestRcvs->yawRatePIDGainsRcvs.gains.differentalGain = localRate->differentalGain;
            this->dataRequestRcvs->yawRatePIDGainsRcvs.gains.integralGain = localRate->integralGain;
            this->dataRequestRcvs->yawRatePIDGainsRcvs.gains.proportionalGain = localRate->proportionalGain;
            this->dataRequestRcvs->yawRatePIDGainsRcvs.ratePIDGainsRecvd = true;
            break;
        }
        case DataPckTypes::StabCtrlRoll:
        {
            StabCtrlDataPck::StabCtrl* localStab = (StabCtrlDataPck::StabCtrl*)this->newDataPck;
            
            this->dataRequestRcvs->stabalisedRoll.proportionalGain = localStab->proportionalGain;
            this->dataRequestRcvs->stabalisedRoll.stabGainRcv = true;
            break;   
        }
        case DataPckTypes::StabCtrlPitch:
        {
            StabCtrlDataPck::StabCtrl* localStab = (StabCtrlDataPck::StabCtrl*)this->newDataPck;
            
            this->dataRequestRcvs->stabalisedPitch.proportionalGain = localStab->proportionalGain;
            this->dataRequestRcvs->stabalisedPitch.stabGainRcv = true;
            break;   
        }
        case DataPckTypes::StabCtrlYaw:
        {
            StabCtrlDataPck::StabCtrl* localStab = (StabCtrlDataPck::StabCtrl*)this->newDataPck;
            
            this->dataRequestRcvs->stabalisedYaw.proportionalGain = localStab->proportionalGain;
            this->dataRequestRcvs->stabalisedYaw.stabGainRcv = true;
            break;   
        }
        case DataPckTypes::UserResponce:
        {
            UserResponceDataPck::UserResponceData* localUserResponce = (UserResponceDataPck::UserResponceData*)this->newDataPck;                
            this->userResponce->userAck = localUserResponce->requestAck;
            break;   
        }
        case DataPckTypes::RequestDataReceived:
        {
            DataRequestReceivedPck::DataRequestReceived* dataRequestReceived = (DataRequestReceivedPck::DataRequestReceived*)this->newDataPck;
            
            this->dataRequestRcvs->altDataReqRcvs.pressureAtSeaLevel = dataRequestReceived->pressureAtSealevel;
            this->dataRequestRcvs->altDataReqRcvs.pressureAtSeaLevelRecvd = true; 
        
            break;   
        }
        case DataPckTypes::SystemStatusAck:
        {
            SystemStatusAckDataPck::SystemStatusAck* systemStatusAckReceived = (SystemStatusAckDataPck::SystemStatusAck*)this->newDataPck;
            
            this->systemStatusAck->acked = systemStatusAckReceived->ack;
            break;   
        }
        case DataPckTypes::QuadMotors: 
        {
            QuadMotorsDataPck::Motors* motorConfigReceived = (QuadMotorsDataPck::Motors*)this->newDataPck;
            
            memcpy (this->dataRequestRcvs->motorConfigRcvs.frameMappings,
                    motorConfigReceived->frameMappings,
                    sizeof(QuadMotorMappingDefinition::MotorFrameMapping));
                    
            memcpy (this->dataRequestRcvs->motorConfigRcvs.motorProperties,
                    motorConfigReceived->motorProperties,
                    sizeof(QuadMotorMappingDefinition::MotorProperties));
                    
            memcpy (&this->dataRequestRcvs->motorConfigRcvs.gloabalLimits,
                    &motorConfigReceived->gloabalLimits,
                    sizeof(QuadMotorProperties::GlobalLimits));
                    
            memcpy (this->dataRequestRcvs->motorConfigRcvs.pwmChannelMotorMapping,
                    motorConfigReceived->pwmChannelMotorMapping,
                    sizeof(PWMChannelMotorMapping::ChannelMotorMappingDefinition));
                    
            this->dataRequestRcvs->motorConfigRcvs.motorConfigDataReceived = true;
            break;   
        }
        case DataPckTypes::DataLogger: 
        {
            if (this->logger != NULL)
            {
                DataLoggerDataPck::DataLogger* dataLoggerConfigReceived = (DataLoggerDataPck::DataLogger*)this->newDataPck;
                this->logger->EnableDisableLogger(dataLoggerConfigReceived->enableDisable);
            }
            
            break;
        }
        case DataPckTypes::SelectedSysConfMsg:
        {
            this->SetSelectedSysConfig(this->newDataPck);
            break;
        }
        case DataPckTypes::SetModeMsg:
        {
            SetModeDataPck::SetMode* setModeReceived = (SetModeDataPck::SetMode*)this->newDataPck;
            this->SetMode(setModeReceived->mode);
            break;   
        }
        case DataPckTypes::FlightCtrlAction:
        {
            FlightCtrlActionDataPck::FlightCtrlAction* FlightCtrlActionRcv = (FlightCtrlActionDataPck::FlightCtrlAction*)this->newDataPck;
            
            HighLevelCtrlActionTypes::ActionRequest actRquest;
            actRquest.action = static_cast<HighLevelCtrlActionTypes::Actions>(FlightCtrlActionRcv->action);
            actRquest.request = static_cast<HighLevelCtrlActionTypes::Request>(FlightCtrlActionRcv->request);
                                                                 
            this->highLevelController->AddAction(actRquest);
            break;   
        }
        case DataPckTypes::ManualFlightData:
        {
            ManualFlightDataDataPck::ManualFlightData* manualFlightDataRcv = (ManualFlightDataDataPck::ManualFlightData*)this->newDataPck;
            
            this->flightCtrlData->manualCtrlFlightData->requestedRoll = manualFlightDataRcv->requestedRoll;
            this->flightCtrlData->manualCtrlFlightData->requestedPitch = manualFlightDataRcv->requestedPitch;
            this->flightCtrlData->manualCtrlFlightData->requestedYaw = manualFlightDataRcv->requestedYaw;
            this->flightCtrlData->manualCtrlFlightData->requestedThrottle = manualFlightDataRcv->requestedThrottle;
            break;   
        }
        case DataPckTypes::HeartBeat:
        {
            this->SendToLogger("Received Comms controller heatbeat.\n\r");
            
            if (this->heartBeatStatus != NULL)
            {
                this->heartBeatStatus->HeartBeatReceived = true;
                this->SendToLogger("Received Comms controller heatbeat updated.\n\r");  
            }
            break;   
        }
        case DataPckTypes::SyncTime:
        {
            SyncTimeDataPck::SyncTimeData* synchTimeDataRcv = (SyncTimeDataPck::SyncTimeData*)this->newDataPck;
            
            if (this->timeSynchStatus != NULL)
            {
            	this->timeSynchStatus->timeSynched = true;
            }

            this->SendToLogger("Time synch\n\r\0");
            
            set_time(synchTimeDataRcv->SyncSeconds);
            break;   
        }
    }
}

void BinaryCmdMsgController::SetMode(unsigned int mode)
{
    if (FlightMode::IsValidMode(mode))
    {
        if (mode != FlightMode::ConfigCal)
        {
            this->selectedSysConf->ReleaseCalConfigObjects();
        }
        
        this->systemModeController->SetMode(mode);
    }
}

void BinaryCmdMsgController::ParseDataCmd()
{
    if (this->msgParserptr != NULL && this->methodCallbackParser != NULL)
    {
        newDataPck = (this->msgParserptr->*this->methodCallbackParser)(&this->receivedCmd);     
    }
}

void BinaryCmdMsgController::SetSelectedSysConfig(DataPckTypeBase::DataPck* selectedSysConf)
{
    SelectedSysConfigDataPck::SelectedSysConfig* selectedSysConfReceived = (SelectedSysConfigDataPck::SelectedSysConfig*)selectedSysConf;
    
    this->selectedSysConf->CreateIMU(selectedSysConfReceived->imu);
    this->selectedSysConf->CreateAltimeter(selectedSysConfReceived->altimeterOption);
    this->selectedSysConf->CreateGPS(selectedSysConfReceived->gpsOption);
    this->selectedSysConf->CreateFlightPckMonitor();
    this->selectedSysConf->CreateMotor();
    //this->selectedSysConf->CreateMotorPwmTestCtrl();
}

void BinaryCmdMsgController::SendToLogger(char* msg)
{
    if (this->logger != NULL)
    {
        this->logger->Log(msg);
    }
}
