#ifndef StorageMemoryMap_H
#define StorageMemoryMap_H
#include "CalStructure.h"
#include "ConfigStructure.h"

namespace StorageMemoryMap
{
    typedef struct
    {
        CalStructure::CalMemStructure calibration;
        ConfigStructure::ConfigMemStructure configuration;
    } MemoryMap;
};
#endif