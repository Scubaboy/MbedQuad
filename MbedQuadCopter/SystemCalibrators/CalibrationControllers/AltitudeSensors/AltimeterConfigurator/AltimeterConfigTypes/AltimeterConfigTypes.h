#ifndef AltimeterConfigTypes_H
#define AltimeterConfigTypes_H

namespace AltimeterConfigTypes
{
    enum Stages {CheckStorage, SendSealevelRequest, WaitingRequestAck, ReadBaseAltitude, Complete};   
};
#endif