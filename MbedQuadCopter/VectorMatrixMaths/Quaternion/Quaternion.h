#ifndef Quaternion_H
#define Quaternion_H
#include "Vector3.h"

namespace Quaternion
{
    struct Quat
    {
        Vector3f vect;
        float    w;    
    };
    
    //void encode(char *buffer);
    
    //void decode(const char *buffer);
    
    //void set( float _w, float _x, float _y, float _z);
    
    const Vector3f getEulerAngles(Quat quat);

};
#endif