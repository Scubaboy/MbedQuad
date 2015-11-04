#ifndef XYZAxisCalCtrlTypes_H
#define XYZAxisCalCtrlTypes_H

namespace XYZAxisCalCtrlTypes
{
    enum Stages {CheckStorage,Calibrate, Complete};  
    enum AxisCalStages {SendAxisStartRequest, WaitingAxisStartAck, CalibratingAxis, AxisCalComplete};
    
    enum Axis 
    {
        XAxisCal = 0, 
        YAxisCal = 1, 
        ZAxisCal = 2
    };
        
    enum AxisMinMax 
    {
        Min = 0, 
        Max = 1
    };
    
    typedef char* AxisCalNameLookup[3][2];
};
#endif