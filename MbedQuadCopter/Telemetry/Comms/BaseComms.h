#ifndef BaseComms_H
#define BaseComms_H
#include "ReceiveDataPacket.h"
class BaseComms
{
    public:
        virtual bool SendDataPacket(char* data, const int dataLength, bool requireConf = true) =0;
        virtual bool TakeDataPacket(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket) = 0;
        virtual bool CommsEstablished() = 0;
        virtual void EstablishComms() = 0;
        virtual void ReSendLastMsg() = 0;
        virtual void DataReceived() = 0;
        virtual bool IsWaitingResp() = 0;
        virtual void SendSynch() = 0;
        virtual bool isSynched() = 0;
        virtual void ReSynch() = 0;
    private:
        virtual void SendReceiveResponce(bool dataPckValid) = 0;
      
};
#endif