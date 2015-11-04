#include "PCA95383CellSelector.h"
#include "PCA9538Registers.h"
#include "mbed.h"

const int MaxCells = 3;
const char OutHigh = 1;
const char OutLow = 0;
const int MinPinID = 0;
const int MaxPinID = 7;
const int I2CBaseAddr = 0x70 << 1;
const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::FlightPackChecker;

PCA95383CellSelector::PCA95383CellSelector(I2CBusCtrl* busCtrl, 
                                           const CellSelectorPortConfig::PortConfigStruct* config,
                                           FlightPackCheckStatus::FlightPackCheckStruct* flightPackCheckStatus)
{
    this->busCtrl = busCtrl;
    this->flightPackCheckStatus = flightPackCheckStatus;
    this->checkingCell = false;
    this->config = config;
}

void PCA95383CellSelector::ConfigPins()
{
    if (config != NULL)
    {
        char portConfig[2];
        
        portConfig[1] = 0;
        for (int iter = 0; iter < MaxCells; iter++)
        {
            this->ConfigurePinDirection(&portConfig[1],config[iter]);
        }
        
        //Write port configuration
        if (this->busCtrl != NULL)
        {
            portConfig[0] = PCA9538Registers::CONFIG;
            int result = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, portConfig , 2);
            
            if (result == 0)
            {
                if (this->flightPackCheckStatus != NULL)
                {
                    this->flightPackCheckStatus->configured = true;
                }
            }
            else
            {
                if (this->flightPackCheckStatus != NULL)
                {
                    this->flightPackCheckStatus->configured = false;
                    sprintf(this->flightPackCheckStatus->cellSelectorMsg,"PCA95383CellSelector::ConfigPins : Failed to configure port dir\0");
                }   
            }
        }
    }
    else
    {
        if (this->flightPackCheckStatus != NULL)
        {
            this->flightPackCheckStatus->configured = false;
            sprintf(this->flightPackCheckStatus->cellSelectorMsg,"PCA95383CellSelector::ConfigPins : pin config undefined\0");
        }
    }
}

void PCA95383CellSelector::ConfigurePinDirection(char *portConfig, const CellSelectorPortConfig::PortConfigStruct config)
{
    if (portConfig != NULL)
    {   
        if (config.pinID >= MinPinID && config.pinID <= MaxPinID)
        {
            *portConfig |= ((char)config.pinMode << config.pinID);  
        }
    }
}

bool PCA95383CellSelector::SelectCell(CellTypes::CellSelect cell)
{
    char portOutValues = 0;
    bool result = false;
    
    portOutValues |= OutHigh << this->config[cell].pinID;
    
    if (this->SetPortValues(portOutValues) == 0)
    {
        this->checkingCell = true;   
        result = true;
    }
    else
    {
        if (this->flightPackCheckStatus != NULL)
        {
            sprintf(this->flightPackCheckStatus->cellSelectorMsg,"PCA95383CellSelector::SelectCell : Failed to select cell\0");
        } 
    } 
    
    return result; 
}

int PCA95383CellSelector::SetPortValues(char pinValues)
{
    int result = 1;
    
    if (this->flightPackCheckStatus != NULL)
    {
        if (this->flightPackCheckStatus->configured)
        {
            if (this->busCtrl != NULL)
            {
                char portOutput[2];
                
                portOutput[0] = PCA9538Registers::OUTPUTPORT;
                portOutput[1] = pinValues;
                
                if (this->busCtrl != NULL)
                {
                    result = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, portOutput , 2);
                }
            }       
        }
    }
    
    return result;
}

void PCA95383CellSelector::DeselectCell()
{
    if (this->checkingCell)
    {
        this->SetPortValues(OutLow);
        this->checkingCell = false;
    }
}