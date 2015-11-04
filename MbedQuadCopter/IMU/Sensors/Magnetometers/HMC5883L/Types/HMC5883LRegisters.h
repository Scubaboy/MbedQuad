#ifndef HMC5883LRegisters_H
#define HMC5883LRegisters_H

namespace HMC5883LRegisters
{
    const int CONFIG_A = 0x00;
    const int CONFIG_B = 0x01;
    const int MODE = 0x02;
    const int MAG_XOUT_H = 0x03;
    const int MAG_XOUT_L = 0x04;
    const int MAG_ZOUT_H = 0x05;
    const int MAG_ZOUT_L = 0x06;
    const int MAG_YOUT_H = 0x07;
    const int MAG_YOUT_L = 0x08;
    const int STATUS = 0x09;
    const int ID_A = 0x0A;
    const int ID_B = 0x0B;
    const int ID_C = 0x0C;
};

#endif