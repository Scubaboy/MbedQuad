#include "HeartBeatIndicator.h"

HeartBeatIndicator::HeartBeatIndicator (PinName heartBeatPin) : heartBeat(heartBeatPin)
{
    heartBeat = 0;
}

void HeartBeatIndicator::UpdateHeartBeat()
{
    heartBeat = !heartBeat;
}