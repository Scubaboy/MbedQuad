#ifndef QuadMotorTypes_H
#define QuadMotorTypes_H

namespace QuadMotorTypes
{
    const int YawFactorCW = -1;
    
    const int YawFactorACW = 1;
    
    const float UpperRightQuad = 45.0;
    
    const float LowerRightQuad = 135.0;
    
    const float UpperLeftQuad = -45.0;
    
    const float LowerLeftQuad = -135.0;
    
    //Cross motor configuration
    const float MotorZero = 0.0;
    const float MotorOne = 90.0;
    const float MotorTwo = 180.0;
    const float MotorThree = 270.0;
    
    const int MaxMotors = 4;
    
    enum MotorID {Motor1=0, Motor2=1, Motor3=2, Motor4=3}; 
    
    enum TestOrder {Test1=0, Test2=1, Test3=2, Test4=3};
    
    struct MotorInternalCtrlDef
    {
        MotorID id;
        float rollFactor;
        float pitchFactor;
        float yawFactor;
        TestOrder testOrder;
    };  
    
    struct MotorFrameCtrlDef
    {
        MotorID id;
        float orientation;
        int yawFactor;
        TestOrder testOrder;
    };
};
#endif