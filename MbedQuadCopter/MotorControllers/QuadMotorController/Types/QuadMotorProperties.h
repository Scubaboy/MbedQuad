#ifndef QuadMotorProperties_H
#define QuadMotorProperties_H

namespace QuadMotorProperties
{
    struct GlobalLimits
    {
        float yaw;
        float rollPitch;
    };
    
    struct MotorProperties
    {
        float maxRateCtrl;
        float minRateCtrl;
        bool armed;
    };  
};
#endif