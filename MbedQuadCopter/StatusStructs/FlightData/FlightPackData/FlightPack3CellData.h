#ifndef FlightPack3CellData_H
#define FlightPack3CellData_H

namespace FlightPack3CellData
{
    enum PackStatus {Good, Poor, Critical, Unknown};
    
    struct FlightPack3CellDataStruct
    {
        float cell1Voltage;
        float cell1_2Voltage;
        float cell1_3Voltage;
        PackStatus status;
          
    };
};
#endif