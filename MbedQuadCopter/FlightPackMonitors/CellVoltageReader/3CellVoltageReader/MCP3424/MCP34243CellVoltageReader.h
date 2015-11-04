#ifndef MCP34243CellVoltageReader_H
#define MCP34243CellVoltageReader_H
#include "CellVoltageReaderBase.h"
#include "CellSelectorBase.h"
#include "FlightPack3CellData.h"
#include "FlightPackCheckStatus.h"
#include "I2CBusCtrl.h"
#include "MCP3424Types.h"

class MCP34243CellVoltageReader : public CellVoltageReaderBase
{
    public:
        MCP34243CellVoltageReader(I2CBusCtrl* busCtrl,const MCP3424Types::CellChannelConfigStruct* channelMapper, CellSelectorBase* cellSelector,FlightPackCheckStatus::FlightPackCheckStruct* flightPackCheckStatus, FlightPack3CellData::FlightPack3CellDataStruct* flightPack3CellData);
        virtual void GetCellVoltage(CellTypes::CellSelect cell);
        virtual void Init();  
    private:
        float RecordCellVoltage(MCP3424Types::AdcChannels channel,float channelScale, CellTypes::CellSelect cell);
        char GetChannel(MCP3424Types::AdcChannels channel);
    private:
        I2CBusCtrl* busCtrl;
        CellSelectorBase* cellSelector;
        FlightPack3CellData::FlightPack3CellDataStruct* flightPack3CellData;
        const MCP3424Types::CellChannelConfigStruct* channelMapper;
        FlightPackCheckStatus::FlightPackCheckStruct* flightPackCheckStatus;
};
#endif