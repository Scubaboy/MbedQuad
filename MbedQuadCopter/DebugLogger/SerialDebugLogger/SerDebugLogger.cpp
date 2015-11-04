#include "SerDebugLogger.h"

SerDebugLogger::SerDebugLogger() : logger(USBTX, USBRX)
{
    this->enable = false;
}

void SerDebugLogger::Log(const char* debugMsg)
{
    const char* runner = debugMsg;
    
    this->LogTimeStamp();
    
    if (this->enable)
    {
        while (*runner != '\0')
        {
            if (this->logger.writeable())
            {
                this->logger.putc(*runner);
                runner++;
            }
        }
    }
}

void SerDebugLogger::EnableDisableLogger(bool enable)
{
    this->enable = enable;  
}

void SerDebugLogger::LogTimeStamp()
{
    time_t seconds = time(NULL);
    char* runner = ctime(&seconds);
    
    if (this->enable)
    {
        while (*runner != '\0')
        {
            if (this->logger.writeable())
            {
                if (*runner != '\n')
                {
                    this->logger.putc(*runner);
                }
                
                runner++; 
            }
        }
        
        //Add space
        if (this->logger.writeable())
        {
            this->logger.putc(' ');       
        }
    }
}
