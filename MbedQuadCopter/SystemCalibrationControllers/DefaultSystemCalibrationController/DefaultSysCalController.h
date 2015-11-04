#ifndef DefaultSysCalController_H
#define DefaultSysCalController_H
#include "BaseSysCalibrationController.h"
#include "BaseDataStorageController.h"
#include "SensorCalibration.h"
#include "BusCtrl.h"
#include "CalibrationControllerTypes.h"

class DefaultSysCalController : public BaseSysCalibrationController
{
    public:
        DefaultSysCalController(BusCtrl* busCtrl, SensorCalibration::SensorCalibratorStruct* sensorCalibrators);
        virtual bool CalibrateSystem();
    private:
        SensorCalibration::SensorCalibratorStruct* sensorCalibrators;
        BusCtrl* busCtrl;
        CalibrationControllerTypes::Stage stage;
};
#endif