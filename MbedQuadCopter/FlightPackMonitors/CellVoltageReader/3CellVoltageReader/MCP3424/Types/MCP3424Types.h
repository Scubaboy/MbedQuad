#ifndef MCP3424Types_H
#define MCP3424Types_H

namespace MCP3424Types
{
    enum AdcChannels {Channel1, Channel2, Channel3, Channel4, Unspecified};
    
    struct CellChannelConfigStruct
    {
        AdcChannels channel;
        float scale;    
    };
    
    const char GainX1 = 0x00;
    const char GainX2 = 0x01;
    const char GainX4 = 0x02;
    const char GainX8 = 0x03;
    const char Conv12Bit = 0x00;
    const char Conv14Bit = 0x01;
    const char Conv16Bit = 0x02;
    const char Conv18Bit = 0x03;
    const char Chan1 = 0x00;
    const char Chan2 = 0x01;
    const char Chan3 = 0x02;
    const char Chan4 = 0x03;
    const char StartConv = 0x80; 
    const char Busy = 0x80; 
    const char SingleConv = 0x00;
    const char ContinuousConv = 0x10;
    
}
#endif