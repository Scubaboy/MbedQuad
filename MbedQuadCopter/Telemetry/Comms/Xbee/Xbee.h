#ifndef Xbee_H
#define Xbee_H
#include "BaseComms.h"
#include "mbed.h"
#include "ReceiveDataPacket.h"
#include "MODSERIAL.h"
#include "DebugLoggerBase.h"
#include "HeartBeatMonitor.h"
#include "CommsStatus.h"

const int QueueSize = 5;

class Xbee : public BaseComms
{
    public:
        Xbee(PinName tx, PinName rx, PinName rst,DebugLoggerBase* debugLogger, CommsStatus::CommsStatusStruct* commsStatus);
        virtual bool SendDataPacket(char* data, const int dataLength, bool requireConf = true);
        virtual bool TakeDataPacket(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket);
        virtual bool CommsEstablished();
        virtual void EstablishComms(); 
        virtual void ReSendLastMsg();
        virtual void DataReceived();
        virtual bool IsWaitingResp();
        virtual void SendSynch();
        virtual bool isSynched();
        virtual void ReSynch();
    private:
        //void DataReceived(void);
        void ResetReceiveBuffer(void);
        void ReadUartReceiveBuffer(MODSERIAL_IRQ_INFO* data);
        void ProcessReceivedData(bool sendConf = true);
        void NotifyMsgTypeRequired(unsigned int msgType);
        void RecvTimerAction();
        virtual void SendReceiveResponce(bool dataPckValid);
    private:
        CommsStatus::CommsStatusStruct* commsStatus;
        MODSERIAL  uartPort;
        DigitalOut rst;   
        bool receiveBufferFull;
        bool buildingReceivedDataPacket;
        int receivedDataLen;
        int receiveBufferTakeIter;
        int receivedDataPacketBytesRemaining;
        int receiveBufferInsertIter;
        int receiveBufferDataPcksRcved;
        bool commsEstablished;
        ReceiveDataPacket::ReceiveDataPacketStruct receiveBuffer;//[QueueSize];   
        char intermediateReceiveBuffer[CommTypes::DataPacketSize];
        char sendBuffer[CommTypes::DataPacketSize];
        char quickSendBuffer[CommTypes::DataPacketSize];
        bool waitingSendConf;
         int expectedResponce;
         int lastMsgReceived;
        bool gotRespnce;
        bool isReading;
        bool msgReady;
        DigitalOut test; 
        bool isSynch;
        bool firstPckRecv;
        Timeout recvTimer;
        DebugLoggerBase* debugLogger;

};
#endif
