#ifndef SchedulerTypes_H
#define SchedulerTypes_H
#include "Task.h"

namespace SchedulerTypes
{
    const int TotalAllowedTasks = 100;
    
    enum Tasks 
    {
        IMUUpdate = 0, 
        AltimeterUpdate = 1, 
        ToggleHeartBeat = 2,
        FlightPackMonitor = 3, 
        SendSystemStatus = 4, 
        SendFlightData = 5, 
        UpdateLowLevelRateCtrl = 6, 
        UpdateHighLevelRateCtrl = 7,
        CheckCmdMsgReceived = 8,
        StartUp = 9, 
        ConfigCal = 10,
        PWMTest = 11,
        MPUUpdate = 12,
        ReSendLastDataPck = 13,
        CheckReceiveQueue = 14,
        CommsSynch = 15,
        ProcessReceivedMsg = 16,
        GPSUpdate = 17,
		HeartBeatSend = 18,
		HeartBeatMonitor = 19
    };
    
    typedef struct SchedulerItem
    {
        int updateIntervalMS;
        int lastUpdatedMS;
        Tasks task;
        Task* taskToExecute;
    } SchedulerItem;   
};
#endif
