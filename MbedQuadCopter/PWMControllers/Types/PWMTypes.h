#ifndef PWMTypes_H
#define PWMTypes_H

namespace PWMTypes
{
    short const AP_MOTORS_MATRIX_YAW_LOWER_LIMIT_PWM  =  200;
    
    enum ChannelId 
    {
        channel1 = 0, 
        channel2 = 1, 
        channel3 = 2, 
        channel4 = 3, 
        channel5 = 4, 
        channel6 = 5, 
        channel7 = 6, 
        channel8 = 7, 
        channel9 = 8, 
        channel10 = 9, 
        channel11 = 10, 
        channel12 = 11, 
        channel13 = 12, 
        channel14 = 13, 
        channel15 = 14, 
        channel16 = 15
    };
    
    struct PWMChannelDefinition
    {
        short channelOutMax;
        short channelOutMin;  
        short channelOutThrMin;
        short channelOutThrMax;
    }; 
    
    struct PWMGlobalDefs
    {
        int globalOutThrMax;
        int globalOutThrMin;
        int globalOutHover; 
        int globalOutStop; 
    }; 
};
#endif