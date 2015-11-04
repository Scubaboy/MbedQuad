#include "MagCalController.h"

const XYZAxisCalCtrlTypes::AxisCalNameLookup Names =
{
    {"Mag rotate X axis to min\0","Mag rotate X axis to max\0"},
    {"Mag rotate Y axis to min\0","Mag rotate Y axis to max\0"},
    {"Mag rotate Z axis to min\0","Mag rotate Z axis to max\0"}
};
 
MagCalController::MagCalController(BaseDataStorageController* dataStorageController, 
                         XYZAxisMinMax::XYZAxisMinMaxCalData *calData, 
                         BaseThreeAxisFlightData::BaseThreeAxisFlightDataStruct* flightData,
                         UserResponce::UserResponceStruct* userResponce, 
                         SensorBase* sensor) : XYZAxisCalController(dataStorageController,
                                                                    calData, 
                                                                    flightData,
                                                                    userResponce, 
                                                                    sensor,
                                                                    &Names)
{
    this->dataStorageController = dataStorageController;
    this->calData = calData;
}

bool MagCalController::CheckAndUseStoredCalData()
{
    bool result = false;
   
    if (this->dataStorageController != NULL)
    {
        if (this->dataStorageController->DataAvailable())
        {
            bool ageValid = false;
            
            if (this->dataStorageController->ReadCalData(&calibrationData, &ageValid))
            {
                if (ageValid)
                {
                    if (this->calData != NULL)
                    {
                        this->calData->xMin = calibrationData.magCal.xMin;
                        this->calData->xMax = calibrationData.magCal.xMax;
                        this->calData->yMin = calibrationData.magCal.yMin;
                        this->calData->yMax = calibrationData.magCal.xMax;
                        this->calData->zMin = calibrationData.magCal.zMin;
                        this->calData->zMax = calibrationData.magCal.zMax;
                        result = true;
                    }               
                }  
            } 
        }
    }
    
    return result;   
}

bool MagCalController::WriteCalDataToDataStore()
{
    bool result = false;
   
    if (this->dataStorageController != NULL)
    {
        result = this->dataStorageController->WriteCalData(calibrationData);
    }
    
    return result;
}