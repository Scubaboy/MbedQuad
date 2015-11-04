#ifndef ADXL345Registers_H
#define ADXL345Registers_H

namespace ADXL345Registers
{
    const int DEV_ID = 0x00;
    const int THRESH_TAP = 0x1D;
    const int OFFSET_X = 0x1E;
    const int OFFSET_Y = 0x1F;
    const int OFFSET_Z = 0x20;
    const int TAP_DURATION = 0x21;
    const int TAP_LATENCY = 0x22;
    const int TAP_WINDOW = 0x23;
    const int THRESH_ACT = 0x24;
    const int THRESH_INACT = 0x25;
    const int TIME_INACT = 0x26;
    const int ACT_INACT_CTL = 0x27;
    const int THRESH_FF = 0x28;
    const int TIME_FF = 0x29;
    const int TAP_AXES = 0x2A;
    const int ACT_TAP_STATUS= 0x2B;
    const int RATE = 0x2C;
    const int POWER_CTL = 0x2D;
    const int INT_ENABLE = 0x2E;
    const int INT_MAP = 0x2F;
    const int INT_SOURCE = 0x30;
    const int DATA_FORMAT = 0x31;
    const int DATAX0 = 0x32;
    const int DATAX1 = 0x33;
    const int DATAY0 = 0x34;
    const int DATAY1 = 0x35;
    const int DATAZ0 = 0x36;
    const int DATAZ1 = 0x37;
    const int FIFO_CTL = 0x38;
    const int FIFO_STATUS = 0x39;
};
#endif