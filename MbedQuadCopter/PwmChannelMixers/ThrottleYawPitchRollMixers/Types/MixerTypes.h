#ifndef MixerTypes_H
#define MixerTypes_H

namespace MixerTypes
{
    struct ConfigDef
    {
        int minThrottle;
        int maxThrottle;
        int testThrottle;
    }; 
    
    struct InputAtts
    {
        int yaw;
        int roll;
        int pitch;
        int throttle;  
    };
    
    struct rawPWMOutputs
    {
        short rollPWM;
        short pitchPWM;
        short yawPWM;
        short throttlePWM;   
    };
};
#endif