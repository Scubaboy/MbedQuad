#ifndef DataPckTypes_H
#define DataPckTypes_H

namespace DataPckTypes
{
    typedef unsigned int DataPckID;
    
        const unsigned int Sync = 0; 
        const unsigned int RateCtrlRoll = 1; 
        const unsigned int RateCtrlPitch = 2;
        const unsigned int RateCtrlYaw = 3;
        const unsigned int StabCtrlRoll = 4; 
        const unsigned int StabCtrlPitch = 5;
        const unsigned int StabCtrlYaw = 6;
        const unsigned int QuadMotors = 7; 
        const unsigned int IMUSensors = 8; 
        const unsigned int FlightPan = 9; 
        const unsigned int FlightPack =10;
        const unsigned int Config = 11;
        const unsigned int Activate =12; 
        const unsigned int ShutDown = 13;
        const unsigned int Message = 14;
        const unsigned int UserResponce = 15;
        const unsigned int RequestData = 16;
        const unsigned int RequestDataReceived = 17;
        const unsigned int ConfigCalComplete = 18;
        const unsigned int SystemId = 19;
        const unsigned int SystemStatusAck = 20;
        const unsigned int FlightData = 21;
        const unsigned int SendConf = 22;
        const unsigned int DataLogger = 23;
        const unsigned int RequiredMsgType = 24;
        const unsigned int FreeTxtMsg = 25;
        const unsigned int SelectedSysConfMsg = 26;
        const unsigned int SetModeMsg = 27;
        const unsigned int FlightCtrlAction = 28;
        const unsigned int ManualFlightData = 29;
        const unsigned int HeartBeat = 30;
        const unsigned int SyncTime = 31;
};
#endif