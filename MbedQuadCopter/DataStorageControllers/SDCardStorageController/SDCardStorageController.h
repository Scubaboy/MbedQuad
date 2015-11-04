#ifndef SDCardStorageController_H
#define SDCardStorageController_H
#include "BaseDataStorageController.h"
#include "SPIBusCtrl.h"

class SDCardStorageController : public BaseDataStorageController
{
    public:
        SDCardStorageController(SPIBusCtrl* busCtrl);
        virtual bool DataAvailable();
        virtual bool WriteCalData(CalStructure::CalMemStructure calibrationData);
        virtual bool ReadCalData(CalStructure::CalMemStructure* calibrationData,bool *ageValid);  
        virtual bool WriteConfigData(ConfigStructure::ConfigMemStructure configurationData);
        virtual bool ReadConfigData(ConfigStructure::ConfigMemStructure* configurationData, bool* ageValid);  
    private:
        bool dataAvailable;
        SPIBusCtrl* busCtrl;
};
#endif