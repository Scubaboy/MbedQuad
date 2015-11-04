#ifndef MotorConfigTypes_H
#define MotorConfigTypes_H

namespace MotorConfigTypes
{
    enum Stages {CheckStorage, SendMotorConfigRequest, WaitingRequestAck, Complete};    
};
#endif