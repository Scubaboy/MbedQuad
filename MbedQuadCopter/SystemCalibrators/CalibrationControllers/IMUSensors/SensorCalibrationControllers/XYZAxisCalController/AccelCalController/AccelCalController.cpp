#include "AccelCalController.h"

const XYZAxisCalCtrlTypes::AxisCalNameLookup Names =
{
    {"Accel rotate X axis to min\0","Accel rotate X axis to max\0"},
    {"Accel rotate Y axis to min\0","Accel rotate Y axis to max\0"},
    {"Accel rotate Z axis to min\0","Accel rotate Z axis to max\0"}
};

AccelCalController::AccelCalController(BaseDataStorageController* dataStorageController, 
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

bool AccelCalController::CheckAndUseStoredCalData()
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
                        this->calData->xMin = calibrationData.accelCal.xMin;
                        this->calData->xMax = calibrationData.accelCal.xMax;
                        this->calData->yMin = calibrationData.accelCal.yMin;
                        this->calData->yMax = calibrationData.accelCal.xMax;
                        this->calData->zMin = calibrationData.accelCal.zMin;
                        this->calData->zMax = calibrationData.accelCal.zMax;
                        result = true;
                    }               
                }  
            } 
        }
    }
    
    return result;   
}

bool AccelCalController::WriteCalDataToDataStore()
{
    bool result = false;
   
    if (this->dataStorageController != NULL)
    {
        result = this->dataStorageController->WriteCalData(calibrationData);
    }
    
    return result;
}