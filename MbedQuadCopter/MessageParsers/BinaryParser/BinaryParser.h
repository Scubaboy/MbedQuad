#ifndef BinaryParser_H
#define BinaryParser_H
#include "DataPckParserBase.h"

class BinaryParser : public DataPckParserBase
{
    public:
        virtual DataPckTypeBase::DataPck* Parse(ReceiveDataPacket::ReceiveDataPacketStruct* dataPck); 
    private:
        DataPckTypeBase::DataPck* BuildFlightPackDataPck(const char* flightData);
        DataPckTypeBase::DataPck* BuildRateCtrlDataPck(const char* rateData, DataPckTypes::DataPckID rateCtrlType);
        DataPckTypeBase::DataPck* BuildStabCtrlDataPck(const char* stabCtrlData, DataPckTypes::DataPckID stabCtrlType);
        DataPckTypeBase::DataPck* BuildQuadMotorDataPck(const char* quadMotorData);
        DataPckTypeBase::DataPck* BuildIMUSensorDataPck(const char* imuSensorData);
        DataPckTypeBase::DataPck* BuildFlightPlanDataPck(const char* flightPlanData);
        DataPckTypeBase::DataPck* BuildShutdownDataPck(const char* shutdowndata);
        DataPckTypeBase::DataPck* BuildUserResponceDataPck(const char* userResponceData);
        DataPckTypeBase::DataPck* BuildDataRequestReceivedPck(const char* dataRequestReceived);
        DataPckTypeBase::DataPck* BuildSystemStatusAckReceivedPck(const char* systemStatusAck);
        DataPckTypeBase::DataPck* BuildDataLoggerReceivedPck(const char* dataLogger);
        DataPckTypeBase::DataPck* BuildSelectedSysConfigPck(const char* selectedSysConfig);
        DataPckTypeBase::DataPck* BuildSetModePck(const char* setMode);
        DataPckTypeBase::DataPck* BuildFlightCtrlActionDataPck(const char* flightCtrlActionData);
        DataPckTypeBase::DataPck* BuildManualFlightDataPck(const char* manualFlightData);
        DataPckTypeBase::DataPck* BuildTimeSyncDataPck(const char* timeSyncData);

};
#endif