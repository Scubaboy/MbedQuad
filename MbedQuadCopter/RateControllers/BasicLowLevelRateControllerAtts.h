#ifndef BasicLowLevelRateControllerAtts_H
#define BasicLowLevelRateControllerAtts_H

class BasicLowLevelRateControllerAtts
{
    public:
        virtual void AngleRollPitchRateYaw(float rollAngle, float pitchAngle, float yawAngle) = 0;  
        virtual void SetThrottleOut(short throttle, bool boost) = 0;
};
#endif