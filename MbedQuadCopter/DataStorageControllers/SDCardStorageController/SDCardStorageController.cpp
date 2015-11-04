#include "SDCardStorageController.h"

SDCardStorageController::SDCardStorageController(SPIBusCtrl* busCtrl)
{
     this->dataAvailable = false;
     this->busCtrl = busCtrl;
}

bool SDCardStorageController::DataAvailable()
{
    return this->dataAvailable;
}

bool SDCardStorageController::WriteCalData(CalStructure::CalMemStructure calibrationData)
{
    return true;
}

bool SDCardStorageController::ReadCalData(CalStructure::CalMemStructure* calibrationData,bool *ageValid)
{
    *ageValid = false;
    return true;
}
  
bool SDCardStorageController::WriteConfigData(ConfigStructure::ConfigMemStructure configurationData)
{
    return true;
}

bool SDCardStorageController::ReadConfigData(ConfigStructure::ConfigMemStructure* configurationData, bool* ageValid)
{
    *ageValid = false;
    return true;
}