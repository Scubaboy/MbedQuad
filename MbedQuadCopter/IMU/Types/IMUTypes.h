#ifndef IMUTypes_H
#define IMUTypes_H

namespace IMUTypes
{
    typedef struct
    {
        int rawX;
        int rawY;
        int rawZ;
    } RawSensorData;

    typedef struct
    {
        float scaledX;
        float scaledY;
        float scaledZ;
    } ScaledSensorData;
    
    typedef struct
    {
        float xOffset;
        float yOffset;
        float zOffset;
    } SensorChannelOffsets;

    typedef struct
    {
        float xScale;
        float yScale;
        float zScale;
    } SensorChannelScales;

    typedef struct
    {
    } CalBase;
    
    typedef struct
    {
        float pitch;
        float roll;
        float yaw;
    } EulerAngles;
};
#endif