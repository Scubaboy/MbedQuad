#ifndef HighLevelCtrlActionTypes_H
#define HighLevelCtrlActionTypes_H

namespace HighLevelCtrlActionTypes
{
    typedef unsigned int FlightAction;
    typedef unsigned int FlightActionRequest;
     
    enum Actions 
    {
        PIDTuning = 0, 
        Hover = 1
    };
    
    enum Request 
    {
        Add = 0, 
        Remove = 1
    };
    
    struct ActionRequest
    {
        Actions action;
        Request request;  
    };
};
#endif