#ifndef CellSelectorPortConfig_H
#define CellSelectorPortConfig_H

namespace CellSelectorPortConfig
{
    enum PinMode {Input = 1, Output = 0};
    enum PortPins {Pin0 = 0, Pin1 = 1, Pin2 = 2, Pin3 = 3, Pin4 = 4, Pin5 = 5, Pin6 = 6, Pin7 = 7};
    
    struct PortConfigStruct
    {
        PinMode pinMode;
        PortPins pinID;
    };  
};
#endif