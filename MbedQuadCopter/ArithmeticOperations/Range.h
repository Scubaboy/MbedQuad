#ifndef Range_H
#define Range_H

namespace Range
{
    float Constrain (const float amt, const float low, const float high);
    float ConstrainFloat(float amt, float low, float high);
    short Constrain (const short amt, const short low, const short high);
    float Map (long x, long inMin, long inMax, long outMin, long outMax);
    int Map (int x, int inMin, int inMax, int outMin, int ouyMax);
    int Min (int a , int b);
};
#endif