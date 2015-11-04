#include "AngleConverters.h"
#include "mbed.h"

const int    DegToCentiDegScale = 100;
const float  DegToRad  = 0.017453292519943295769236907684886f;
const float  RadToDeg  = 57.295779513082320876798154814105f;

float AngleConverters::RadiansToDegrees(float radians)
{
    return radians * RadToDeg;
}

float AngleConverters::RadiansToCentiDegrees(float radians)
{
    return DegreesToCentiDegrees(RadiansToDegrees(radians));
}

float AngleConverters::DegreesToRadians(float degrees)
{
    return degrees * DegToRad;   
}

float AngleConverters::DegreesToCentiDegrees(float degrees)
{
    return degrees*DegToCentiDegScale;
}