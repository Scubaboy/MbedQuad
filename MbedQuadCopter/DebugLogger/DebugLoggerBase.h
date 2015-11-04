#ifndef DebugLoggerBase_H
#define DebugLoggerBase_H

class DebugLoggerBase
{
    public:
        virtual void Log(const char* debugMsg) = 0;
        virtual void EnableDisableLogger(bool enable) = 0;  
};
#endif
