#include "BasicTimer.h"

BasicTimer::BasicTimer()
{
    this->timer.start();    
}

float BasicTimer::Read_Ms()
{
    if (this->timer.read_us() < 0)
    {
        this->timer.reset();
    }
        
    return this->timer.read_ms();     
}
        