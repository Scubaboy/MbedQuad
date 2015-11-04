#include "Range.h"
#include "mbed.h"

float Range::Constrain(const float amt, const float low, const float high)
{
    return (amt)<(low)?(low):((amt)>(high)?(high):(amt));
}

short Range::Constrain (const short amt, const short low, const short high)
{
    return (amt)<(low)?(low):((amt)>(high)?(high):(amt));
}

float Range::ConstrainFloat(float amt, float low, float high) 
{
    // the check for NaN as a float prevents propogation of
    // floating point errors through any function that uses
    // constrain_float(). The normal float semantics already handle -Inf
    // and +Inf
    if (isnan(amt)) {
        return (low+high)*0.5f;
    }
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

float Range::Map (long x, long inMin, long inMax, long outMin, long outMax)
{
    return (x - inMin) * (outMax - outMin) / (inMax - inMin);
}

int Range::Map (int x, int inMin, int inMax, int outMin, int outMax)
{
    return (x - inMin) * (outMax - outMin) / (inMax - inMin);
}

int Range::Min (int a , int b)
{
    return a<b ? a : b;   
}