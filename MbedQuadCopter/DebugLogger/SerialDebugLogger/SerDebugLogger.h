#ifndef SerDebugLogger_H
#define SerDebugLogger_H
#include "DebugLoggerBase.h"
#include "mbed.h"

class SerDebugLogger : public DebugLoggerBase
{
    public:
        SerDebugLogger();
        virtual void Log(const char* debugMsg);
        virtual void EnableDisableLogger(bool enable);
    private:
        void LogTimeStamp();
    private:
        Serial logger;
        bool enable;   
};
#endif
