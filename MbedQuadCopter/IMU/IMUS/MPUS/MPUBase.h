#ifndef MPUBase_H
#define MPUBase_H
#include "Quaternion.h"

class MPUBase
{
    public:
        virtual void Init() = 0;
        virtual void GetUpdate(Vector3f* gyroRaw, 
                               Vector3f* gyroScaled, 
                               Vector3f* accelRaw, 
                               Vector3f* accelScaled,
                               Vector3f* magRaw,
                               Vector3f* magScaled,
                               Vector3f* eulerAngles,
                               Quaternion::Quat* quad) = 0;   
};
#endif