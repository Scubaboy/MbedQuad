#ifndef StatusDataTextEncoder_H
#define StatusDataTextEncoder_H
#include "EncodersDecodersBase.h"
#include "FlightStatusEncoderTypes.h"
#include "SystemStatus.h"
#include "BaseComms.h"


class StatusDataTextEncoder : public EncodersDecodersBase
{
    public:
      StatusDataTextEncoder(const SystemStatus::SystemStatusStruct* systemStatusData, BaseComms* comms);
      virtual void Encode();  
    private:
        void BuildAccelStatusData(char* sendBuffer); 
        void BuildGyroStatusData(char* sendBuffer);
        void BuildMagStatusData(char* sendBuffer);
        void BuildIMUStatusData(char* sendBuffer);
        void BuildFlightPackCheckCellSelectStatusData(char* sendBuffer);
        void BuildFlightPackCheckCellVoltageReaderStatusData(char* sendBuffer);
    private:
        virtual void Decode();
        const SystemStatus::SystemStatusStruct* systemStatusData;
        BaseComms* comms;
};
#endif