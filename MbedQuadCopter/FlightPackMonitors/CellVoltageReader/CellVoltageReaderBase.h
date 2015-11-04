#ifndef CellVoltageReaderBase_H
#define CellVoltageReaderBase_H
#include "CellTypes.h"

class CellVoltageReaderBase
{
    public:
        virtual void GetCellVoltage(CellTypes::CellSelect cell) = 0;  
        virtual void Init() = 0;
};
#endif