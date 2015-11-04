#ifndef MPU9150Enums_H
#define MPU9150Enums_H

namespace MPU9150Enums
{
    /* Filter configurations. */
    enum LPF 
    {
        INV_FILTER_256HZ_NOLPF2 = 0,
        INV_FILTER_188HZ,
        INV_FILTER_98HZ,
        INV_FILTER_42HZ,
        INV_FILTER_20HZ,
        INV_FILTER_10HZ,
        INV_FILTER_5HZ,
        INV_FILTER_2100HZ_NOLPF,
        NUM_FILTER
    };

    /* Full scale ranges. */
    enum GyroFsr 
    {
        INV_FSR_250DPS = 0,
        INV_FSR_500DPS,
        INV_FSR_1000DPS,
        INV_FSR_2000DPS,
        NUM_GYRO_FSR
    };
    
    /* Full scale ranges. */
    enum AccelFsr 
    {
        INV_FSR_2G = 0,
        INV_FSR_4G,
        INV_FSR_8G,
        INV_FSR_16G,
        NUM_ACCEL_FSR
    };
    
    /* Clock sources. */
    enum ClockCel 
    {
        INV_CLK_INTERNAL = 0,
        INV_CLK_PLL,
        NUM_CLK
    };
    
    /* Low-power accel wakeup rates. */
    enum LpAccelRate 
    {
        INV_LPA_1_25HZ,
        INV_LPA_5HZ,
        INV_LPA_20HZ,
        INV_LPA_40HZ
    };    
};
#endif