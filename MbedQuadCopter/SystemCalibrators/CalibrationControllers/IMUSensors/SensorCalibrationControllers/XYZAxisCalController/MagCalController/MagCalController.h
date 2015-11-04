#ifndef MagCalController_H
#define MagCalController_H

#include "XYZAxisCalController.h"

class MagCalController : public XYZAxisCalController
{
    public:
      MagCalController(BaseDataStorageController* dataStorageController, 
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