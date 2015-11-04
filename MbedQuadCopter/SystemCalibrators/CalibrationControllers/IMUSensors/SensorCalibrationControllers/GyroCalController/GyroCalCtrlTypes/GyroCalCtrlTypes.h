#ifndef GyroCalCtrlTypes_H
#define GyroCalCtrlTypes_H

namespace GyroCalCtrlTypes
{
    enum Stages {CheckStorage, SendStartRequest, WaitingStartAck, Calibrating, Complete};
};
#endif