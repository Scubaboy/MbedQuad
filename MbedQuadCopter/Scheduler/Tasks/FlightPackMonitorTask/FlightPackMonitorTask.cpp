#include "FlightPackMonitorTask.h"
#include "CellTypes.h"

FlightPackMonitorTask::FlightPackMonitorTask(CellVoltageReaderBase* cellVoltageReader, 
                                             CellSelectorBase* cellSelector,
                                             BusCtrl* busCtrl)
{  
    this->initRun = false;
    this->busCtrl = busCtrl;
    this->cellVoltageReader = cellVoltageReader;
    this->cellSelector = cellSelector;
         
}

void FlightPackMonitorTask::Init()
{
    this->busCtrl->TakeCtrl(BusCtrlUsers::FlightPackChecker);   
    this->cellSelector->ConfigPins();  
    this->busCtrl->ReleaseCtrl(BusCtrlUsers::FlightPackChecker);
    this->initRun = true;
}

void FlightPackMonitorTask::ExecuteTask(int interval)
{
    if (!this->initRun)
    {
        this->Init();
    }
    
    this->busCtrl->TakeCtrl(BusCtrlUsers::FlightPackChecker);   
      this->cellVoltageReader->GetCellVoltage(CellTypes::Cell1);  
      this->cellVoltageReader->GetCellVoltage(CellTypes::Cell1_2); 
      this->cellVoltageReader->GetCellVoltage(CellTypes::Cell1_3); 
    this->busCtrl->ReleaseCtrl(BusCtrlUsers::FlightPackChecker);
}