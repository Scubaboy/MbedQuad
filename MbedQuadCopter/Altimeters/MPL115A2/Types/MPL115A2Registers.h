#ifndef MPL115A2Registers_H
#define MPL115A2Registers_H

namespace MPL115A2Registers
{
    const int PADC_MSB = 0x00;
    const int PADC_LSB = 0x01;
    const int TADC_MSB = 0x02;
    const int TADC_LSB = 0x03;
    const int A0_MSB   = 0x04;
    const int A0_LSB   = 0x05;
    const int B1_MSB   = 0x06;
    const int B1_LSB   = 0x07;
    const int B2_MSB   = 0x08;
    const int B2_LSB   = 0x09;
    const int C12_MSB  = 0x0A;
    const int C12_LSB  = 0x0B;
    const int CONVERT  = 0x12;   
};
#endif