#ifndef MTKV11GPS_H
#define MTKV11GPS_H
#include "mbed.h"
#include "DebugLoggerBase.h"
#include "GPSPositionData.h"
#include "GPSBase.h"
#include "MODSERIAL.h"

class MTKV11GPS : public GPSBase
{
    public:
        MTKV11GPS(PinName tx, PinName rx,GPSPositionData::GPSPositionDataStruct* positionData, DebugLoggerBase* logger);
        virtual bool Initialise();
        virtual void ReadPositionData();
    private:
        bool CheckSumGood (unsigned char* binaryData);
        bool HeaderCorrect (unsigned char* binaryData);
        float ReadFourByteFloat(unsigned char* binaryData, int start, int scalefactor);
        float ReadTwoByteFloat(unsigned char* binaryData, int start, int scaleFactor);
        unsigned short ReadOneByteUnsignedShort(unsigned char* binaryData,int start);
        int ReadFourByteInt(unsigned char* binaryData, int start);
        void WritePositionData();
    private:
        MODSERIAL  uartPort;
        GPSPositionData::GPSPositionDataStruct* positionData;
        DebugLoggerBase* logger;
        unsigned char* gpsMsg;
};
#endif