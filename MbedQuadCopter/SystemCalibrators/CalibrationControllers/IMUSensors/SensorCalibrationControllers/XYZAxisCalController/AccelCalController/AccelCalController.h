#ifndef AccelCalController_H
#define AccelCalController_H

#include "XYZAxisCalController.h"

class AccelCalController : public XYZAxisCalController
{
    public:
      AccelCalController(BaseDataStorageController* dataStorageController, 
                         XYZAxisMinMax::XYZAxisMinMaxCalData *calData, 
                         BaseThreeAxisFlightData::BaseThreeAxisFlightDataStruct* flightData,
                         UserResponce::UserResponceStruct* userResponce, 
                         SensorBase* sensor);
      virtual bool CheckAndUseStoredCalData();  
      virtual bool WriteCalDataToDataStore(); 
    
    private:
        BaseDataStorageController* dataStorageController;
        XYZAxisMinMax::XYZAxisMinMaxCalData *calData;    
};

#endif