#ifndef ITG3200Registers_H
#define ITG3200Registers_H

namespace ITG3200Registers
{
    const int DEV_ID = 0x00;
    const int SMPLRT_DIV = 0x15;
    const int DLPF_FS= 0x16;
    const int INT_CFG = 0x17;
    const int INT_STATUS = 0x1A;
    const int TEMP_OUT_H = 0x1B;
    const int TEMP_OUT_L = 0x1C;
    const int GYRO_XOUT_H = 0x1D;
    const int GYRO_XOUT_L = 0x1E;
    const int GYRO_YOUT_H = 0x1F;
    const int GYRO_YOUT_L = 0x20;
    const int GYRO_ZOUT_H = 0x21;
    const int GYRO_ZOUT_L = 0x22;
    const int PWR_MGM = 0x3E;
};
#endif