#ifndef BasicRateController_H
#define BasicRateController_H
#include "BasicLowLevelRateControllerAtts.h"
#include "BasicLowLevelRateController.h"
#include "Vector3.h"
#include "PIDAxisRateConfiguration.h"
#include "MotionControlFlightData.h"
#include "SensorFlightData.h"
#include "BaseMotorController.h"

class BasicRateController : public BasicLowLevelRateControllerAtts, public BasicLowLevelRateController
{
    public:
        BasicRateController(PIDAxisRateConfiguration::PIDAxisRateConfigStruct* pidAxisRateCtrl,
                            MotionControlFlightData::MotionControlFlightDataStruct* motionCtrlData,
                            SensorFlightData::SensorFlightDataStruct* flightSensorsData,
                            BaseMotorController* motorController);
                            
        virtual void AngleRollPitchRateYaw(float rollAngle, float pitchAngle, float yawAngle);
        
        virtual void SetThrottleOut(short throttle, bool boost);
        
        //init_targets - resets target angles to current angles
        virtual void InitTargets();
        
        virtual void Init();
        
        //
        // rate_controller_run - run lowest level body-frame rate controller and send outputs to the motors
        //      should be called at 100hz or more
        //
        virtual void RateControllerRun(float timeDeltaSeconds);
    private:
        Vector3f targetAngles;
        Vector3f actualAngle;
        PIDAxisRateConfiguration::PIDAxisRateConfigStruct* pidAxisRateCtrl;
        MotionControlFlightData::MotionControlFlightDataStruct* motionCtrlData;
        SensorFlightData::SensorFlightDataStruct* flightSensorsData;
        BaseMotorController* motorController;
};
#endif