#ifndef BasicTimer_H
#define BasicTimer_H
#include "TimerBase.h"
#include "mbed.h"

class BasicTimer : public TimerBase
{
    public:
        BasicTimer();
        virtual float Read_Ms(); 
    private: 
        Timer timer; 
};
#endif