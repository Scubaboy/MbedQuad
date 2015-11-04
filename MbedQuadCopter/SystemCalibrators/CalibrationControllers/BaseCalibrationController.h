#ifndef BaseCalibrationController_H
#define BaseCalibrationController_H
#include "MsgDataPck.h"

class FunctionPtrInternal;

class BaseCalibrationController
{
    public:
        virtual bool RunCalibration() = 0;  
     
    private:
        virtual bool SendRequest(char* msg, int len, bool reSend = true) = 0;
};
#endif