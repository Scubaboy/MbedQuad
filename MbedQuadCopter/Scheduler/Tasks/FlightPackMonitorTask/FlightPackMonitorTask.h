#ifndef FlightPackMonitorTask_H
#define FlightPackMonitorTask_H
#include "Task.h"
#include "BusCtrl.h"
#include "CellVoltageReaderBase.h"
#include "CellSelectorBase.h"

class FlightPackMonitorTask : public Task
{
    public:
        FlightPackMonitorTask(CellVoltageReaderBase* cellVoltageReader,
                              CellSelectorBase* cellSelector,
                              BusCtrl* busCtrl);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        CellVoltageReaderBase* cellVoltageReader;
        CellSelectorBase* cellSelector;
        BusCtrl* busCtrl;
        bool initRun;
};
#endif