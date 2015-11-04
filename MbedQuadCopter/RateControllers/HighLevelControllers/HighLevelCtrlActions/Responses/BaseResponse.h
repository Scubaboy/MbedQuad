#ifndef BaseResponse_H
#define BaseResponse_H

namespace BaseResponse
{
    enum ResponseTypes {Hover};
    
    struct BaseResp
    {
        ResponseTypes type;   
    };
};
#endif