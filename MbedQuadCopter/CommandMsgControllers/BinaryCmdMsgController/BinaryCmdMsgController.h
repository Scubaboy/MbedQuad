#ifndef BinaryCmdMsgController_H
#define BinaryCmdMsgController_H
#include "CmdMsgControllerBase.h"
#include "SystemModeControllerBase.h"
#include "ReceiveDataPacket.h"
#include "FlightMode.h"
#include "MotionControlFlightData.h"
#include "QuadMotorConfigFlightData.h"
#include "DataPckTypeBase.h"
#include "UserResponce.h"
#include "DataRequest.h"
#include "SystemStatusAck.h"
#include "DebugLoggerBase.h"
#include "SysConfigCtrl.h"
#include "BaseHighLevelRateController.h"
#include "FlightCtrlData.h"
#include "HeartBeatStatusStruct.h"
#include "TimeSynchStatus.h"

class BinaryCmdMsgController : public CmdMsgControllerBase
{
    public:
        BinaryCmdMsgController(SystemModeControllerBase* systemModeController,
                               BaseHighLevelRateController* highLevelController,
                               UserResponce::UserResponceStruct* userResponce,
                               DataRequestRecv::DataRequestRecvs* dataRequestRcvs,
                               FlightCtrlData::FlightCtrlDataStruct* flightCtrlData,
                               SystemStatusAck::SystemStatusAckStruct* systemStatusAck,
                               HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus,
							   TimeSynchStatus::TimeSynchStatusStruct* timeSynchStatus,
                               DebugLoggerBase* logger,
                               SysCtrlConfigCtrl* selectedSysConf);
        virtual void CheckForCmdMsg();
    private:
        void SetSelectedSysConfig(DataPckTypeBase::DataPck* selectedSysConf);
        void SetMode(unsigned int mode);
        void SendToLogger(char* msg);
    private:
        virtual void ParseDataCmd();
        virtual void ActionReceivedDataPck();
    public:
        template <class ClassT>
        void CmdMsgReader(ClassT *ptr, bool(ClassT::*pFunc)(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket))
        {
                this->msgReaderptr = (FunctionPtrInternal*)ptr;  
                this->methodCallbackReader = (bool (FunctionPtrInternal::*)(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket))pFunc; 
        } 
        
        template <class ClassT>
        void CmdMsgParser(ClassT *ptr, DataPckTypeBase::DataPck*(ClassT::*pFunc)(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket))
        {
                this->msgParserptr = (FunctionPtrInternal*)ptr;  
                this->methodCallbackParser = (DataPckTypeBase::DataPck* (FunctionPtrInternal::*)(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket))pFunc; 
        }
    private:
        FunctionPtrInternal* msgReaderptr;
        FunctionPtrInternal* msgParserptr;
        bool (FunctionPtrInternal::*methodCallbackReader)(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket); 
        DataPckTypeBase::DataPck* (FunctionPtrInternal::*methodCallbackParser)(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket);
        ReceiveDataPacket::ReceiveDataPacketStruct receivedCmd;
        SystemModeControllerBase* systemModeController;
        UserResponce::UserResponceStruct* userResponce;
        DataRequestRecv::DataRequestRecvs* dataRequestRcvs;
        DataPckTypeBase::DataPck* newDataPck; 
        SystemStatusAck::SystemStatusAckStruct* systemStatusAck;
        DebugLoggerBase* logger;
        SysCtrlConfigCtrl* selectedSysConf;
        BaseHighLevelRateController* highLevelController;
        FlightCtrlData::FlightCtrlDataStruct* flightCtrlData;
        HeartBeatStatus::HeartBeatStatusStruct* heartBeatStatus;
        TimeSynchStatus::TimeSynchStatusStruct* timeSynchStatus;
};
#endif
