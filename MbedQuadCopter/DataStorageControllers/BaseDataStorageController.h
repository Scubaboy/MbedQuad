#ifndef BaseDataStorageController_H
#define BaseDataStorageController_H
#include "CalStructure.h"
#include "ConfigStructure.h"

class BaseDataStorageController
{
    public:
        virtual bool DataAvailable() = 0;
        virtual bool WriteCalData(CalStructure::CalMemStructure calibrationData) = 0;
        virtual bool ReadCalData(CalStructure::CalMemStructure* calibrationData,bool *ageValid) = 0;  
        virtual bool WriteConfigData(ConfigStructure::ConfigMemStructure configurationData) = 0;
        virtual bool ReadConfigData(ConfigStructure::ConfigMemStructure* configurationData, bool* ageValid) = 0;  
};
#endif