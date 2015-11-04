#ifndef BasicLowLevelRateController_H
#define BasicLowLevelRateController_H

class BasicLowLevelRateController
{
    public:
        
        //init_targets - resets target angles to current angles
        virtual void InitTargets() = 0;
        
        virtual void Init() =0;
        
        //
        // rate_controller_run - run lowest level body-frame rate controller and send outputs to the motors
        //      should be called at 100hz or more
        //
        virtual void RateControllerRun(float timeDeltaSeconds) = 0;
};
#endif