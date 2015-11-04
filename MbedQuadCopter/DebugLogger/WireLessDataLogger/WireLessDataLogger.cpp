#include "WireLessDataLogger.h"
#include "mbed.h"

WireLessDataLogger::WireLessDataLogger()
{
    this->enable = false; 
    this->dataLoggerDebugMsgPck.type = DataPckTypes::DataLogger;
}

void WireLessDataLogger::Log(char* debugMsg)
{
    if (this->enable)
    {
        int len = strlen(debugMsg); 
        
        if (len > DataLoggerDataPck::DebugMsgSize)
        {
            int start = 0;
            
            while (len > 0)
            {
                memset(this->dataLoggerDebugMsgPck.debugMsg,'\0',DataLoggerDataPck::DebugMsgSize);
                
                if (len > DataLoggerDataPck::DebugMsgSize)
                {
                    memcpy(this->dataLoggerDebugMsgPck.debugMsg,&debugMsg[start],DataLoggerDataPck::DebugMsgSize);
                    this->SendDebugMsg(&dataLoggerDebugMsgPck);
                }
                else
                {
                    memcpy(this->dataLoggerDebugMsgPck.debugMsg,&debugMsg[start],len);
                    this->SendDebugMsg(&dataLoggerDebugMsgPck); 
                }
                
                len -= DataLoggerDataPck::DebugMsgSize;
                start += DataLoggerDataPck::DebugMsgSize;
            }
            
        }
        else
        {
            memset(this->dataLoggerDebugMsgPck.debugMsg,'\0',DataLoggerDataPck::DebugMsgSize);
            memcpy(this->dataLoggerDebugMsgPck.debugMsg,debugMsg,len);
            
            this->SendDebugMsg(&dataLoggerDebugMsgPck);
        }  
    }   
}

void WireLessDataLogger::EnableDisableLogger(bool enable)
{
    this->enable = enable;      
}
        
void WireLessDataLogger::SendDebugMsg(DataLoggerDataPck::DataLogger* dataLogger)
{
    if (this->sendDebugptr != NULL && this->methodCallbackSendDebugMsg != NULL)
    {
        if (!(this->sendDebugptr->*this->methodCallbackSendDebugMsg)((char*)dataLogger, sizeof(DataLoggerDataPck::DataLogger),false))
        {
            
        }
    }
}