#ifndef PositionResponse_H
#define PositionResponse_H
#include "BaseResponse.h"
#include "Vector3.h"

namespace PositionResponse
{
    struct PositionResp : public BaseResponse::BaseResp
    {
        short throttle;
        int pitchDegrees;
        int rollDegrees;
        int yawDegrees;  
    };  
};
#endif