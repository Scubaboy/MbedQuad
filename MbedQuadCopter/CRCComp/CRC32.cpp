#include "CRC32.h"

#define CRC32_POLYNOMIAL 0xEDB88320L

void CRC32Value(unsigned long &crc, char byte)
{
    /////////////////////////////////////////////////////////////////////////////////////
    //CRC must be initialized as zero 
    //c is a character from the sequence that is used to form the CRC
    //this code is a modification of the code from the Novatel OEM615 specification
    /////////////////////////////////////////////////////////////////////////////////////
    unsigned long ulTemp1 = ( crc >> 8 ) & 0x00FFFFFFL;
    unsigned long ulCRC = ((int) crc ^ byte ) & 0xff ;
    
    for (int  j = 8 ; j > 0; j-- )
    {
        if ( ulCRC & 1 )
        {
            ulCRC = ( ulCRC >> 1 ) ^ CRC32_POLYNOMIAL;
        }
        else
        {
            ulCRC >>= 1;
        }
    }
    
    crc = ulTemp1 ^ ulCRC; 
}

unsigned long CRC32::CalculateCRC32(const unsigned int size, const char *buffer )
{
    //////////////////////////////////////////////////////////////////////
    //the below code tests the CRC32Value procedure used in a markov form
    //////////////////////////////////////////////////////////////////////
    unsigned long CRC = 0;
    for (int i = 0; i<size; i++)
    {
        CRC32Value( CRC, *buffer++ );
    }
    
    return  CRC;
}