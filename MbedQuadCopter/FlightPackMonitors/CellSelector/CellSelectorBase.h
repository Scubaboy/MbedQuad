#ifndef CellSelectorBase_H
#define CellSelectorBase_H
#include "CellSelectorPortConfig.h"
#include "CellTypes.h"

class CellSelectorBase
{
    public:
        virtual void ConfigPins() = 0; 
        virtual bool SelectCell(CellTypes::CellSelect cell) = 0;
        virtual void DeselectCell() = 0; 
};
#endif