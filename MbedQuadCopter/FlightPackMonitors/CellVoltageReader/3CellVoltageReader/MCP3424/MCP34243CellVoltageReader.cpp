#include "MCP34243CellVoltageReader.h"
#include "mbed.h"

const int I2CBaseAddr = 0x68 << 1;
const float MilliVoltToVolts = 1000.0;

const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::FlightPackChecker;

MCP34243CellVoltageReader::MCP34243CellVoltageReader(I2CBusCtrl* busCtrl,
                                                     const MCP3424Types::CellChannelConfigStruct* channelMapper,
                                                     CellSelectorBase* cellSelector,
                                                     FlightPackCheckStatus::FlightPackCheckStruct* flightPackCheckStatus, 
                                                     FlightPack3CellData::FlightPack3CellDataStruct* flightPack3CellData)
{
    this->busCtrl = busCtrl;
    this->cellSelector = cellSelector;
    this->flightPack3CellData = flightPack3CellData; 
    this->channelMapper = channelMapper;
    this->flightPackCheckStatus = flightPackCheckStatus;
}

void MCP34243CellVoltageReader::GetCellVoltage(CellTypes::CellSelect cell)
{
    float cellVoltage = 0.0;
    
    if (this->flightPack3CellData != NULL)
    { 
        if (cell >= CellTypes::Cell1 && cell <= CellTypes::Cell1_3)
        {
            switch (this->channelMapper[cell].channel)
            {
                case MCP3424Types::Channel1:
                case MCP3424Types::Channel3:
                case MCP3424Types::Channel4:
                {
                    cellVoltage = this->RecordCellVoltage(this->channelMapper[cell].channel,this->channelMapper[cell].scale, cell);
                    
                    if (cell == CellTypes::Cell1)
                    {
                        this->flightPack3CellData->cell1Voltage =  cellVoltage; 
                    }
                    else if (cell == CellTypes::Cell1_2)
                    {
                        this->flightPack3CellData->cell1_2Voltage =  cellVoltage;
                    }
                    else if (cell == CellTypes::Cell1_3)
                    {
                        this->flightPack3CellData->cell1_3Voltage =  cellVoltage;
                    }
                    break;
                }
            }   
        }
    }
}

float MCP34243CellVoltageReader::RecordCellVoltage(MCP3424Types::AdcChannels channel,float channelScale, CellTypes::CellSelect cell)
{
    char config = 0;
    int mvDivisor = 0;
    float cellVoltage = 80.0;
    
    if (this->busCtrl != NULL && this->cellSelector != NULL)
    {
        if (this->cellSelector->SelectCell(cell))
        {
            config = MCP3424Types::StartConv | MCP3424Types::SingleConv;
            config |= this->GetChannel(channel) << 5 | MCP3424Types::Conv18Bit << 2 | MCP3424Types::GainX1;
            mvDivisor = 1 << (MCP3424Types::GainX1 + 2* MCP3424Types::Conv18Bit);
            
            int writeResult = this->busCtrl->WriteToBus(busUser, I2CBaseAddr, &config , 1);

            if (writeResult == 0)
            {
                int data;
                
                do
                {
                    int readResult = this->busCtrl->ReadFromBus(busUser, I2CBaseAddr, (char*)&data , 4);
                
                    if (readResult == 0)
                    {
                        int adjustedVal = 0;
                        char* adjustedValPtr = (char*)&adjustedVal;
                    
                        adjustedValPtr[2] = ((char*)&data)[0]; 
                        adjustedValPtr[1] = ((char*)&data)[1];
                        adjustedValPtr[0] = ((char*)&data)[2];
                        adjustedValPtr[3] = adjustedValPtr[2] & 0x80 ? 0xff : 0;
                    
                        cellVoltage = (((float)adjustedVal/(float)mvDivisor)/MilliVoltToVolts) * channelScale;
                    }
                    else
                    {
                        if (this->flightPackCheckStatus != NULL)
                        {
                            sprintf (this->flightPackCheckStatus->cellAdcMsg,"MCP34243CellVoltageReader::RecordCellVoltage -> Failed to read adc value\0");   
                        }
                    }
                    
                }while(((char*)&data)[3] & MCP3424Types::Busy);
                
                this->cellSelector->DeselectCell();
            }
            else
            {
                if (this->flightPackCheckStatus != NULL)
                {
                    sprintf (this->flightPackCheckStatus->cellAdcMsg,"MCP34243CellVoltageReader::RecordCellVoltage -> Failed to set adc config\0");   
                } 
            }
        }
        else
        {
            if (this->flightPackCheckStatus != NULL)
            {
                sprintf (this->flightPackCheckStatus->cellAdcMsg,"MCP34243CellVoltageReader::RecordCellVoltage -> Failed to set cell selector\0");   
            }
        }
    }
    
    return cellVoltage;
}

char MCP34243CellVoltageReader::GetChannel(MCP3424Types::AdcChannels channel)
{
    char channelVal = 0x00;
    
    switch (channel)
    {
        case MCP3424Types::Channel1:
        {
            channelVal = MCP3424Types::Chan1;
            break;
        }
        case MCP3424Types::Channel3:
        {
            channelVal = MCP3424Types::Chan3;
            break;   
        }
        case MCP3424Types::Channel4:
        {
            channelVal = MCP3424Types::Chan4;
            break;
        }
    }
    
    return channelVal;
}

void MCP34243CellVoltageReader::Init()
{
    
}