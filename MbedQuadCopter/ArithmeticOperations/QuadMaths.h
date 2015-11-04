#ifndef QuadMaths_H
#define QuadMaths_H

namespace QuadMaths
{
    float pythagorous2(float a, float b);
    float pythagorous3(float a, float b, float c);
    float sq(float v);
    float SafeSqrt(float value); 
    
    // a faster varient of atan.  accurate to 6 decimal places for values between -1 ~ 1 but then diverges quickly
    float FastAtan(float v);
    
    // a varient of asin() that always gives a valid answer.
    float SafeAsin(float v);
    
    float Max(float a,float b);// ((a)>(b)?(a):(b))
    float Min(float a, float b);
};
#endif