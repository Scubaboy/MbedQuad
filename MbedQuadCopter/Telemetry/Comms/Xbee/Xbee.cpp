#include "Xbee.h"
#include "CRC32.h"
#include "SendConfDataPck.h"
#include "RequiredMsgTypeDataPck.h"
#include "DataPckTypes.h"
#include "DataPckTypeBase.h"
#include "MessageResponceMapper.h"

const int CRCTypeSize = 4;
const int StartEndMarkerSize = 1;
const int ActualDataSize = CommTypes::DataPacketSize - CRCTypeSize;
const int DataSynchPckSize = 3;
const char SynchPck[] = "##\0";
DigitalOut heartBeat = DigitalOut(LED1); 
Xbee::Xbee(PinName tx, PinName rx, PinName rst, DebugLoggerBase* debugLogger, HeartBeatMonitor* heartBeatMonitor) : uartPort(tx,rx), rst(rst), test(LED1)
{
    this->waitingSendConf = false;
    this->uartPort.format(8, Serial::None, 1);
    this->uartPort.baud(9600);
    this->uartPort.rxBufferFlush();
    this->uartPort.txBufferFlush();
    
    this->expectedResponce = -1;
    this->lastMsgReceived = -1;
    //Configure receive buffer
    this->ResetReceiveBuffer();
    this->gotRespnce = false;
    this->isReading = false;
    this->msgReady = false;
    this->isSynch = false;
    this->mode = Synching;
    this->firstPckRecv = false;
    
    if (debugLogger != NULL)
    {
        this->debugLogger = debugLogger;   
    }
    
    this->heartBeatMonitor = heartBeatMonitor;
} 

void Xbee::SendSynch()
{
    if (!this->isSynch)
    {
        if (this->commsEstablished)
        {
            if (this->uartPort.writeable())
            {
                for (int iter = 0; iter < strlen(SynchPck); iter++)
                {
                    this->uartPort.putc(SynchPck[iter]);   
                }       
            }    
        }
    }       
}
        
bool Xbee::isSynched()
{
    return this->mode == Synched ? true : false;    
}
void Xbee::ReSynch()       
{
    this->mode = Synching;
}

bool Xbee::SendDataPacket(char* data, const int dataLength, bool requireConf)
{
    bool result = false;
    
    if (this->mode == Synched)
    {
        if (dataLength <= (CommTypes::DataPacketSize - CRCTypeSize))
        {
            //Set quadID
            if (requireConf && !this->waitingSendConf)
            {
                memset(this->sendBuffer,'1',sizeof(this->sendBuffer));
                memcpy(&this->sendBuffer[CRCTypeSize+(2*StartEndMarkerSize)],data,dataLength);
            
                //Set quadID
                memcpy(&this->sendBuffer[CRCTypeSize+(2*StartEndMarkerSize)+4],&SystemConfig::SystemID,4);
            
                unsigned long crc =  CRC32::CalculateCRC32(CommTypes::DataPacketSize - CRCTypeSize-(4*StartEndMarkerSize), 
                                                    &this->sendBuffer[CRCTypeSize+(2*StartEndMarkerSize)]);
                memcpy(&this->sendBuffer[(2*StartEndMarkerSize)],&crc,CRCTypeSize);
                this->sendBuffer[0] = '<';
                this->sendBuffer[1] = '<';
                this->sendBuffer[CommTypes::DataPacketSize-1] = '>';
                this->sendBuffer[CommTypes::DataPacketSize-2] = '>';
                
                if (this->commsEstablished)
                {
                    int sendIter = 0;

                    while (sendIter < CommTypes::DataPacketSize)
                    {
                        if (this->uartPort.writeable())
                        {
                            this->uartPort.putc(sendBuffer[sendIter]);
                            sendIter++; 
                        
                            if (sendIter % 10 == 0)
                            {
                                wait_ms(20);   
                            }  
                        }
                    }
            
                    result = true;
                    this->waitingSendConf = true;
                    expectedResponce = MessageResponceMapper::ExpectedResponce(*((unsigned int*)data));
                }
            }
            else if (!requireConf)
            { 
                memset(quickSendBuffer,'1',sizeof(quickSendBuffer));
                memcpy(&quickSendBuffer[CRCTypeSize+(2*StartEndMarkerSize)],data,dataLength);
                //Set quadID
                memcpy(&this->quickSendBuffer[CRCTypeSize+(2*StartEndMarkerSize)+4],&SystemConfig::SystemID,4);
            
                unsigned long crc =  CRC32::CalculateCRC32(CommTypes::DataPacketSize - CRCTypeSize-(4*StartEndMarkerSize),
                                                       &quickSendBuffer[CRCTypeSize+(2*StartEndMarkerSize)]);
                memcpy(&quickSendBuffer[2*StartEndMarkerSize],&crc,CRCTypeSize);
                quickSendBuffer[0] = '<';
                quickSendBuffer[1] = '<';
                quickSendBuffer[CommTypes::DataPacketSize-1] = '>';
                quickSendBuffer[CommTypes::DataPacketSize-2] = '>';
            
                if (this->commsEstablished)
                {
                    int sendIter = 0;

                    while ( sendIter < CommTypes::DataPacketSize)
                    {
                        if (this->uartPort.writeable())
                        {
                            this->uartPort.putc(quickSendBuffer[sendIter]);
                            sendIter++;
                            if (sendIter % 10 == 0)
                            {
                                wait_ms(20);   
                            }
                            
                                  
                        }
                    
                        
                    }

                    result = true;
                }    
            }
        } 
    } 
    
    return result; 
}

bool Xbee::TakeDataPacket(ReceiveDataPacket::ReceiveDataPacketStruct* dataPacket)
{
    bool takeRst = false;
    
   // this->isReading = true;
      
    
    if (dataPacket != NULL && this->msgReady)
    {
        this->lastMsgReceived = -1;
        takeRst = true;
        memcpy(dataPacket->dataPack,&this->receiveBuffer.dataPack,sizeof(dataPacket->dataPack));
        
        if (this->debugLogger != NULL)
        {  
            this->debugLogger->Log("Command received taken.\n\r\0");
        } 
    }
       
    this->isReading = false;
    this->msgReady = false;

    return takeRst;
}

bool Xbee::CommsEstablished()
{
    return this->commsEstablished;
}

void Xbee::EstablishComms()
{
    //Put Xbee into reset;
    this->rst = 0;

    //Take Xbee out of reset mode.
    this->rst = 1;
    wait_ms(1);
    this->receivedDataLen=0;
    this->commsEstablished = true;
    this->uartPort.rxBufferFlush();
    this->uartPort.txBufferFlush();
    this->ResetReceiveBuffer();
    
    //Attach received interrupt handler.
    uartPort.attach(this,&Xbee::ReadUartReceiveBuffer);//,Serial::RxIrq);
}

void Xbee::ReSendLastMsg()
{
    if (this->mode == Synched)
    {
        if (this->waitingSendConf)
        {
            if (this->commsEstablished)
            {
                int iter = 0;
                
                this->uartPort.txBufferFlush();
                while (iter < CommTypes::DataPacketSize)
                {
                    if (this->uartPort.writeable())
                    {
                        this->uartPort.putc(this->sendBuffer[iter]);
                        iter++;
                        if (iter % 10 == 0)
                        {
                            wait_ms(20);   
                        }
                    }
                }
            }
        }
    }

}

bool Xbee::IsWaitingResp()
{
    return this->waitingSendConf;  
}

void Xbee::DataReceived()
{
    switch (this->mode)
    {
        case Synching:
        {
            
            if (this->receivedDataLen == DataSynchPckSize)
            {
                
                if (strcmp(SynchPck,this->intermediateReceiveBuffer) == 0)
                {
                    this->mode = Synched;
                    this->receivedDataLen = 0;
                    this->heartBeatMonitor->StartMonitor();
                    this->heartBeatMonitor->StartSend();
                    
                    if (this->debugLogger != NULL)
                    {  
                        this->debugLogger->Log("Synch Received\n\r\0");
                    }
                } 
                
                this->buildingReceivedDataPacket = false;
            }
            
            break;   
        }   
        case Synched:
        {
            if (this->receivedDataLen == CommTypes::DataPacketSize)
            {
                this->recvTimer.detach();
                this->firstPckRecv = false;
                
                unsigned long crc =  CRC32::CalculateCRC32(ActualDataSize, &this->intermediateReceiveBuffer[CRCTypeSize]);
     
                if (crc == (*(unsigned long*)this->intermediateReceiveBuffer))
                {   
                    if (DataPckTypes::SendConf == *(DataPckTypes::DataPckID*)&this->intermediateReceiveBuffer[CRCTypeSize])
                    {
                        
                        if (this->waitingSendConf)
                        {
                            this->waitingSendConf = false; 
                            this->lastMsgReceived = -1;
                            
                        } 
                
                        this->buildingReceivedDataPacket = false;
                          
                    }
                    else if (this->lastMsgReceived == -1 || 
                        this->lastMsgReceived != *(DataPckTypes::DataPckID*)&this->intermediateReceiveBuffer[CRCTypeSize])
                    {
                        this->ProcessReceivedData(false);
                        this->buildingReceivedDataPacket = false;
                        
                        //Does the sender require a send conformation.
                        DataPckTypeBase::DataPck* data = (DataPckTypeBase::DataPck*)&this->intermediateReceiveBuffer[CRCTypeSize];
        
                        if (data->ackRequired == 1)
                        {
                            SendReceiveResponce(true);
                        }
                        
                        this->lastMsgReceived = *(DataPckTypes::DataPckID*)&this->intermediateReceiveBuffer[CRCTypeSize];
                        if (this->debugLogger != NULL)
                        {  
                            this->debugLogger->Log("Command received\n\r\0");
                        }
                    }
                    else if (this->lastMsgReceived == *(DataPckTypes::DataPckID*)&this->intermediateReceiveBuffer[CRCTypeSize])
                    {
                        //Send Ack
                        this->buildingReceivedDataPacket = false;
                        SendReceiveResponce(true);
                    }
                }
                else
                {
                    //CRC failed request a resent
                    this->buildingReceivedDataPacket = false;
                    
                    if (this->debugLogger != NULL)
                    {
                        this->debugLogger->Log("Synched Received msg failed crc check\n\0");
                    }
                }
                
                this->receivedDataLen = 0;
            }  
                 
            break;   
        }
    }
}

void Xbee::ProcessReceivedData(bool sendConf)
{
        memcpy(&this->receiveBuffer.dataPack,
            &this->intermediateReceiveBuffer[CRCTypeSize],ActualDataSize); 
        this->msgReady = true;
}

void Xbee::SendReceiveResponce(bool dataPckValid)
{
    SendConfDataPck::SendConf sendConf;

    sendConf.type = DataPckTypes::SendConf;
    sendConf.reSend = !dataPckValid;
    this->SendDataPacket((char*)&sendConf,sizeof(SendConfDataPck::SendConf),false);
}

void Xbee::NotifyMsgTypeRequired(unsigned int msgType)
{
    RequiredMsgTypeDataPck::RequiredMsgType reqMsgType;
    
    reqMsgType.type = DataPckTypes::RequiredMsgType;
    reqMsgType.msgType = msgType;
    this->SendDataPacket((char*)&reqMsgType,sizeof(RequiredMsgTypeDataPck::RequiredMsgType),false);   
}

void Xbee::RecvTimerAction()
{
    this->recvTimer.detach();
    this->firstPckRecv = false;
    
    if (this->receivedDataLen < CommTypes::DataPacketSize)
    {
        this->buildingReceivedDataPacket = false;
        this->uartPort.rxBufferFlush();
        
        if (this->debugLogger != NULL)
        {
            this->debugLogger->Log("Msg receive cycle failed.\n\r\0");
        }
    }
}

void Xbee::ReadUartReceiveBuffer(MODSERIAL_IRQ_INFO* data)
{     
    if (this->commsEstablished)
    {
        if (!this->buildingReceivedDataPacket)
        {
            this->buildingReceivedDataPacket = true; 
            this->receivedDataLen = 0;
            memset (this->intermediateReceiveBuffer,'\0',sizeof(this->intermediateReceiveBuffer));
        }

        while (this->uartPort.readable())
        {
            switch (this->mode)
            {
                case Synched:
                {
                    if (!this->firstPckRecv)
                    {
                        this->firstPckRecv = true;     
                        this->recvTimer.attach(this, &Xbee::RecvTimerAction, 0.500);
                    }
                    
                    if (this->receivedDataLen < sizeof(this->intermediateReceiveBuffer) && !this->isReading) 
                    {
                        this->intermediateReceiveBuffer[this->receivedDataLen++] = this->uartPort.getc();
                    }
                    else
                    {
                        this->uartPort.getc();
                    }       
                    break;   
                }
                case Synching:
                {
                    if (this->receivedDataLen > DataSynchPckSize)
                    {
                        this->receivedDataLen = 0;
                    }
                    this->intermediateReceiveBuffer[this->receivedDataLen++] = this->uartPort.getc();
                    break;   
                }   
            }  
        }   
    }
}

void Xbee::ResetReceiveBuffer(void)
{
    memset(&this->receiveBuffer.dataPack[0],0,CommTypes::DataPacketSize);   
    this->receiveBufferFull = false;
    this->buildingReceivedDataPacket = false;
    this->receiveBufferTakeIter = 0;
    this->receiveBufferDataPcksRcved = 0;
    this->receiveBufferInsertIter = 0;
}