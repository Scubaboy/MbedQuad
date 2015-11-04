#ifndef PCA9538_H
#define PCA9538_H
#include "CellSelectorBase.h"
#include "I2CBusCtrl.h"
#include "FlightPackCheckStatus.h"
#include "mbed.h"

class PCA95383CellSelector : public CellSelectorBase
{
    public:
        PCA95383CellSelector(I2CBusCtrl* busCtrl,const CellSelectorPortConfig::PortConfigStruct* config, FlightPackCheckStatus::FlightPackCheckStruct* flightPackCheckStatus);
    public:
        virtual void ConfigPins(); 
        virtual bool SelectCell(CellTypes::CellSelect cell);
        virtual void DeselectCell();
    private:
        void ConfigurePinDirection(char* portConfig, const CellSelectorPortConfig::PortConfigStruct config);
        int SetPortValues(char pinValues);
    private:
        bool checkingCell;
        FlightPackCheckStatus::FlightPackCheckStruct* flightPackCheckStatus;
        I2CBusCtrl* busCtrl;
        const CellSelectorPortConfig::PortConfigStruct* config;
         
};
#endif