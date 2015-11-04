#ifndef AltimeterDataRequestRcvs_H
#define AltimeterDataRequestRcvs_H

namespace AltimeterDataRequestRcvs
{
    //Allowed altitude data request responces.
    struct AltitudeDataRequest
    {
        float pressureAtSeaLevel;
        bool pressureAtSeaLevelRecvd;
    };  
};
#endif