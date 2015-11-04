#include "QuadMaths.h"
#include "mbed.h"

const float PI = 3.141592653589793f;

float QuadMaths::sq(float v) 
{
    return v*v;
}

// 2D vector length
float QuadMaths::pythagorous2(float a, float b) 
{
    return sqrtf(sq(a)+sq(b));
}

// 3D vector length
float QuadMaths::pythagorous3(float a, float b, float c) 
{
    return sqrtf(sq(a)+sq(b)+sq(c));
}

float QuadMaths::SafeSqrt(float value)
{
    float ret = sqrtf(value);
    
    if (isnan(ret)) 
    {
        ret = 0;
    }
    
    return ret;
}

float QuadMaths::SafeAsin(float v)
{
    float result;
    
    if (isnan(v)) 
    {
        result = 0.0;
    }
    else if (v >= 1.0f) 
    {
        result = PI/2;
    }
    else if (v <= -1.0f) 
    {
        result = -PI/2;
    }
    else
    {
        result = asinf(v);
    }
    
    return result;
}

// a faster varient of atan.  accurate to 6 decimal places for values between -1 ~ 1 but then diverges quickly
float QuadMaths::FastAtan(float v)
{
    float v2 = v*v;
    return (v*(1.6867629106f + v2*0.4378497304f)/(1.6867633134f + v2));
}

float QuadMaths::Max(float a,float b)// ((a)>(b)?(a):(b))
{
    return ((a)>(b)?(a):(b));
}

float QuadMaths::Min(float a, float b)
{
    return ((a)<(b)?(a):(b));  
}
