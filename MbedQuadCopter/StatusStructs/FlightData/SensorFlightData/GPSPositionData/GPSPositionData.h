#ifndef GPSPositionData_H
#define GPSPositionData_H

namespace GPSPositionData
{
  struct GPSPositionDataStruct 
    {
        float latitude;
        float longitude;
        float mslAltitude; 
        float groundSpeedms;
        float headingDegrees;
        unsigned short   satellities; 
        unsigned short   fixType;
        int   dateDDMMYY;
        float utcTime; 
        float hdop;
        int   ck_A;
        int   ck_B;
    };
};
#endif