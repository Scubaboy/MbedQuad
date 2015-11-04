#include "SysConfigCtrl.h"
#include "MCP3424Types.h"
#include "CellSelectorPortConfig.h"
#include "MCP34243CellVoltageReader.h"

//Configure FlightPackCell monitor
const MCP3424Types::CellChannelConfigStruct flightPackCellAdcChannelMap[3] =
{
    {MCP3424Types::Channel3,2.63},
    {MCP3424Types::Channel4,4.0},
    {MCP3424Types::Channel1,6.0}       
}; 
    
const CellSelectorPortConfig::PortConfigStruct flightPackCellSelectorConfig[3] = 
{
    {CellSelectorPortConfig::Output,CellSelectorPortConfig::Pin0},
    {CellSelectorPortConfig::Output,CellSelectorPortConfig::Pin1},
    {CellSelectorPortConfig::Output,CellSelectorPortConfig::Pin2}
};
    
void SysCtrlConfigCtrl::CreateFlightPckMonitor()
{
    
    
    selectedSysConf->imuBusCtrl = this->busCtrl1;
    //Setup Flight pack monitor
    selectedSysConf->flightPackCellSelector = new PCA95383CellSelector(this->busCtrl1, 
                                                      flightPackCellSelectorConfig, 
                                                      &this->sysStatus->flightPackCheckStatus);
                                                      
    selectedSysConf->flightPackMonitor = new MCP34243CellVoltageReader(this->busCtrl1,
                                                            flightPackCellAdcChannelMap, 
                                                            this->selectedSysConf->flightPackCellSelector,
                                                            &this->sysStatus->flightPackCheckStatus, 
                                                            this->flightData->flightPackData);
     
}