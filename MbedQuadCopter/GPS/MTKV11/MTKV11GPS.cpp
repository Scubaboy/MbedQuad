#include "MTKV11GPS.h"

const char* CustomBinaryModeCmd = "$PGCMD,16,0,0,0,0,0*6A\r\n";
const int CustomBinaryModeMsgSizeBytes = 37;
const int CheckSumALocation = 35;
const int CheckSumBLocation = 36;
const int SerialRcvBufferSize = 40;
const int HeaderStart = 0;
const int HeaderEnd = 1;
const char HeaderFirstByte = 0xd0;
const char HeaderSecondByte = 0xdd;

MTKV11GPS::MTKV11GPS(PinName tx, PinName rx,GPSPositionData::GPSPositionDataStruct* positionData, DebugLoggerBase* logger) : uartPort(tx,rx)
{
    this->positionData = positionData;
    this->logger = logger;
    this->gpsMsg = new unsigned char[CustomBinaryModeMsgSizeBytes];
}

bool MTKV11GPS::Initialise()
{
    int sendIter = 0;
    bool result = true;
    
    //Setup uart port
    this->uartPort.format(8, Serial::None, 1);
    this->uartPort.baud(38400);
    
    //Set recv buffer size
    if (this->uartPort.rxBufferSetSize(SerialRcvBufferSize) != MODSERIAL::Ok)
    {
        if (this->logger != NULL)
        {
            this->logger->Log("Failed to set Serial Rcv buffer\n\r\0");
        }
        
        result = false;
    }
    else
    {
        //Switch GPS into custom binary datafeed mode.
        this->uartPort.txBufferFlush();
        while (sendIter < strlen(CustomBinaryModeCmd))
        {
            if (this->uartPort.writeable())
            {
                this->uartPort.putc(CustomBinaryModeCmd[sendIter]);
                sendIter++;   
            }
        }
        
        //Fush rcv buffer
        this->uartPort.rxBufferFlush();
    }
    return result;
}

bool MTKV11GPS::CheckSumGood (unsigned char* binaryData)
{
    unsigned char calCKA = 0;
    unsigned char calCKB = 0;
    unsigned char val = 0;
    

    for (int iter = 2; iter <= CustomBinaryModeMsgSizeBytes-3; iter++) // upper limit 34
    {
        val = binaryData[iter];
        calCKA += val;
        calCKB += calCKA;
    }
  
    return (calCKA == binaryData[CheckSumALocation] && calCKB == binaryData[CheckSumBLocation]);
}

bool MTKV11GPS::HeaderCorrect (unsigned char* binaryData)
{
    return (HeaderFirstByte == binaryData[HeaderStart] && HeaderSecondByte == binaryData[HeaderEnd]);
}

float MTKV11GPS::ReadFourByteFloat(unsigned char* binaryData, int start, int scaleFactor)
{
    float val = (float)(*((int*)&binaryData[start]));
    return (val/(float)scaleFactor);
}

int MTKV11GPS::ReadFourByteInt(unsigned char* binaryData, int start)
{
    int result = 0;
    
    if (binaryData != NULL)
    {
        result = *((int*)&binaryData[start]);    
    }
    
    return result;
}

float MTKV11GPS::ReadTwoByteFloat(unsigned char* binaryData, int start, int scaleFactor)
{
    unsigned short rawValue = 0.0;
    float result = 0.0;
    
    if (binaryData != NULL)
    {
        rawValue = *((unsigned short*)&binaryData[start]);
        result = ((float)rawValue)/scaleFactor;
    }
    
    return result;
}

unsigned short MTKV11GPS::ReadOneByteUnsignedShort(unsigned char* binaryData,int start)
{
    char val = 0;
    unsigned short value = 0;
    
    if (binaryData != NULL)
    {
        val = binaryData[start];
        
        value = (unsigned short)val;
    }
    
    return value;
    
}

void MTKV11GPS::WritePositionData()
{
    if (this->logger != NULL && this->positionData != NULL)
    {
        char msg[200];
                        
        memset(msg,'0',sizeof(msg));
    
        sprintf(msg ,"Lat: %f\n\rLong: %f\n\rMsl %f\n\rGroundSpeed %f\n\rHeading %f\n\rSat Count: %d\n\rFix: %d\n\rDate %d\n\rUtc %f\n\rHdop %f\n\r**************\n\r\0",
            this->positionData->latitude,
            this->positionData->longitude,
            this->positionData->mslAltitude,
            this->positionData->groundSpeedms,
            this->positionData->headingDegrees,
            this->positionData->satellities,
            this->positionData->fixType,
            this->positionData->dateDDMMYY,
            this->positionData->utcTime,
            this->positionData->hdop);  
                         
            this->logger->Log(msg);
    }
}

void MTKV11GPS::ReadPositionData()
{
    //Has a complete binary messag been received.
    if (this->uartPort.rxBufferGetCount() >= CustomBinaryModeMsgSizeBytes)
    {
        //read the GPS binary message
        for (int iter = 0; iter < CustomBinaryModeMsgSizeBytes; iter++)
        {
            this->gpsMsg[iter] = this->uartPort.getc();
        }
  
        if (this->CheckSumGood(this->gpsMsg))
        {
            if (this->HeaderCorrect(this->gpsMsg))
            {
                if (this->positionData != NULL)
                {
                    //Extract GPS data from binary message.
                    this->positionData->latitude = this->ReadFourByteFloat(this->gpsMsg,3,1000000); 
                    this->positionData->longitude = this->ReadFourByteFloat(this->gpsMsg,7,1000000);
                    this->positionData->mslAltitude = this->ReadFourByteFloat(this->gpsMsg,11,100);
                    this->positionData->groundSpeedms = this->ReadFourByteFloat(this->gpsMsg,15,100);
                    this->positionData->headingDegrees = this->ReadFourByteFloat(this->gpsMsg,19,100);
                    this->positionData->satellities = this->ReadOneByteUnsignedShort(this->gpsMsg,23);
                    this->positionData->fixType = this->ReadOneByteUnsignedShort(this->gpsMsg,24);
                    this->positionData->dateDDMMYY = this->ReadFourByteInt(this->gpsMsg,25);
                    this->positionData->utcTime = this->ReadFourByteFloat(this->gpsMsg,29,100);
                    this->positionData->hdop = this->ReadTwoByteFloat(this->gpsMsg,33,100);
                    
                    //write position info to debugger
                    if (this->logger != NULL)
                    {
                        this->WritePositionData();
                    }
                }
            }
            else
            {
                if (this->logger != NULL)
                {
                    this->logger->Log("GPS Header Incorrect\n\r\0");  
                } 
            }
        }
        else
        {
            if (this->logger != NULL)
            {
                 this->logger->Log("GPS checksum comparision failed!\n\r\0");  
            }   
        }
        
        //Fush rcv buffer
        this->uartPort.rxBufferFlush();
    }

}