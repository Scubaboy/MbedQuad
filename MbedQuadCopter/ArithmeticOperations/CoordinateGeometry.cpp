#include "CoordinateGeometry.h"
#include "mbed.h"

//wrap an angle in centi-degrees to 0..35999
int CoordinateGeometry::wrap_360_cd(int error)
{
    if (error > 360000 || error < -360000) 
    {
        // for very large numbers use modulus
        error = error % 36000;
    }
    
    while (error >= 36000) 
    {
        error -= 36000;
    }
    
    while (error < 0)
    {
         error += 36000;
    }
    
    return error;
}

//wrap an angle in centi-degrees to -18000..18000
int CoordinateGeometry::wrap_180_cd(int error)
{
    if (error > 360000 || error < -360000) 
    {
        // for very large numbers use modulus
        error = error % 36000;
    }
    
    while (error > 18000) 
    { 
        error -= 36000; 
    }
    
    while (error < -18000) 
    { 
        error += 36000; 
    }
    
    return error;
}

//wrap an angle in centi-degrees to 0..35999
float CoordinateGeometry::wrap_360_cd_float(float angle)
{
    if (angle >= 72000.0f || angle < 36000.0f) 
    {
        // for larger number use fmodulus
        angle = fmod(angle, 36000.0f);
    }
    
    if (angle >= 36000.0f)
    { 
        angle -= 36000.0f;
    }
    
    if (angle < 0.0f)
    {
         angle += 36000.0f;
    }
    
    return angle;
}

//wrap an angle in centi-degrees to -18000..18000
float CoordinateGeometry::wrap_180_cd_float(float angle)
{
    if (angle > 54000.0f || angle < -54000.0f) 
    {
        // for large numbers use modulus
        angle = fmod(angle,36000.0f);
    }
    
    if (angle > 18000.0f) 
    { 
        angle -= 36000.0f; 
    }
    
    if (angle < -18000.0f) 
    { 
        angle += 36000.0f;
    }
    
    return angle;
}