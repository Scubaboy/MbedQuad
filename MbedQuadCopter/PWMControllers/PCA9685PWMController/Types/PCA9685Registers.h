#ifndef PCA9685Registers_H
#define PCA9685Registers_H

namespace PCA9685Registers
{
    const int SUBADR1 = 0x2;
    const int SUBADR2 = 0x3;
    const int SUBADR3 = 0x4;

    const int MODE1 = 0x0;
    const int PRESCALE = 0xFE;

    const int LED0_ON_L = 0x6;
    const int LED0_ON_H = 0x7;
    const int LED0_OFF_L = 0x8;
    const int LED0_OFF_H = 0x9;

    const int ALLLED_ON_L = 0xFA;
    const int ALLLED_ON_H = 0xFB;
    const int ALLLED_OFF_L = 0xFC;
    const int ALLLED_OFF_H = 0xFD;   
}; 
#endif