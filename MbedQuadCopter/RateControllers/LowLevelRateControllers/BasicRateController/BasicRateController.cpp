#include "BasicRateController.h"

BasicRateController::BasicRateController(PIDAxisRateConfiguration::PIDAxisRateConfigStruct* pidAxisRateCtrl,
                            MotionControlFlightData::MotionControlFlightDataStruct* motionCtrlData,
                            SensorFlightData::SensorFlightDataStruct* flightSensorsData,
                            BaseMotorController* motorController)
{
    this->pidAxisRateCtrl = pidAxisRateCtrl;
    this->motionCtrlData = motionCtrlData;
    this->flightSensorsData = flightSensorsData;   
    this->motorController = motorController;
}

//Angles in degrees
void BasicRateController::AngleRollPitchRateYaw(float rollAngle, float pitchAngle, float yawAngle)
{
      this->targetAngles.x = rollAngle;
      this->targetAngles.z = yawAngle;
      this->targetAngles.y = pitchAngle; 
}

        
//init_targets - resets target angles to current angles
void BasicRateController::InitTargets()
{
    // set earth frame angle targets to current lean angles
    this->actualAngle.x = this->flightSensorsData->imuFlightData->rollAngle;// _ahrs.roll_sensor;
    this->actualAngle.y = this->flightSensorsData->imuFlightData->pitchAngle;// _ahrs.pitch_sensor;
    this->actualAngle.z = this->flightSensorsData->imuFlightData->yawAngle;// _ahrs.yaw_sensor;
}
        
void BasicRateController::Init()
{
    this->motorController->Init();      
}
        
//
// rate_controller_run - run lowest level body-frame rate controller and send outputs to the motors
//      should be called at 100hz or more
//
void BasicRateController::RateControllerRun(float timeDeltaSeconds)
{
    this->actualAngle.x = this->flightSensorsData->imuFlightData->rollAngle;// _ahrs.roll_sensor;
    this->actualAngle.y = this->flightSensorsData->imuFlightData->pitchAngle;// _ahrs.pitch_sensor;
    this->actualAngle.z = this->flightSensorsData->imuFlightData->yawAngle;
    
    Vector3f angleDif = this->targetAngles - this->actualAngle;
    
    //Compute PID values
    //Calculate the pitch PID output;
    pidAxisRateCtrl->pitch->CalculatePID(angleDif.y, timeDeltaSeconds);
    this->motorController->SetPitch(this->motionCtrlData->ratePitch->pidValue);
    
    pidAxisRateCtrl->roll->CalculatePID(angleDif.x, timeDeltaSeconds);
    this->motorController->SetRoll(this->motionCtrlData->rateRoll->pidValue);
    
    pidAxisRateCtrl->yaw->CalculatePID(angleDif.z, timeDeltaSeconds);
    this->motorController->SetYaw(this->motionCtrlData->rateYaw->pidValue);
    
    //Send to motorcontroller.
    this->motorController->Output();
    
}

void BasicRateController::SetThrottleOut(short throttle, bool boost)
{
    this->motorController->SetThrottle(throttle);
}