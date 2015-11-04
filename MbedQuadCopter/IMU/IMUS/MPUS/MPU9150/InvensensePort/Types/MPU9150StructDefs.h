#ifndef MPU9150StructDefs_H
#define MPU9150StructDefs_H
#include "MPU9150Consts.h"

namespace MPU9150StructDefs
{
    typedef struct
    {
        unsigned short key;
        unsigned short addr;
    } tKeyLabel;
    
    struct IntParamS 
    {
        void (*cb)(void);
        unsigned short pin;
        unsigned char lp_exit;
        unsigned char active_low;
    }; 
    
    struct DmpS 
    {
        void (*tap_cb)(unsigned char count, unsigned char direction);
        void (*android_orient_cb)(unsigned char orientation);
        unsigned short orient;
        unsigned short featureMask;
        unsigned short fifoRate;
        unsigned char packetLength;
    };
    
    //typedef DmpS DmpArray[MPU9150Consts::MPU_MAX_DEVICES];   
    
    /* Hardware registers needed by driver. */
    struct DriverRegisters 
    {
        unsigned char whoAmI;
        unsigned char rateDiv;
        unsigned char lpf;
        unsigned char prodId;
        unsigned char userCtrl;
        unsigned char fifoEn;
        unsigned char gyroCfg;
        unsigned char accelCfg;
        unsigned char accelCfg2;
        unsigned char lpAccelOdr;
        unsigned char motionThr;
        unsigned char motionDur;
        unsigned char fifoCountH;
        unsigned char fifoRW;
        unsigned char rawGyro;
        unsigned char rawAccel;
        unsigned char temp;
        unsigned char intEnable;
        unsigned char dmpIntStatus;
        unsigned char intStatus;
        unsigned char accelIntel;
        unsigned char pwrMgmt1;
        unsigned char pwrMgmt2;
        unsigned char intPinCfg;
        unsigned char memRW;
        unsigned char accelOffs;
        unsigned char i2cMst;
        unsigned char bankSel;
        unsigned char memStartAddr;
        unsigned char prgmStartH;
        unsigned char s0Addr;
        unsigned char s0Reg;
        unsigned char s0Ctrl;
        unsigned char s1Addr;
        unsigned char s1Reg;
        unsigned char s1Ctrl;
        unsigned char s4Ctrl;
        unsigned char s0Do;
        unsigned char s1Do;
        unsigned char i2cDelayCtrl;
        unsigned char rawCompass;
        /* The I2C_MST_VDDIO bit is in this register. */
        unsigned char ygOffsTc;
    };
    
    /* Information specific to a particular device. */
    struct DeviceInfo 
    {
        unsigned char addr;
        unsigned short maxFifo;
        unsigned char numReg;
        unsigned short tempSens;
        short tempOffset;
        unsigned short bankSize;
        unsigned short compassFsr;
    };
    
    /* When entering motion interrupt mode, the driver keeps track of the
     * previous state so that it can be restored at a later time.
     * TODO: This is tacky. Fix it.
     */
    struct MotionIntCache 
    {
        unsigned short gyroFsr;
        unsigned char accelFsr;
        unsigned short lpf;
        unsigned short sampleRate;
        unsigned char sensorsOn;
        unsigned char fifoSensors;
        unsigned char dmpOn;
    };
    
    /* Cached chip configuration data.
     * TODO: A lot of these can be handled with a bitmask.
     */
    struct ChipCfg
    {
        /* Matches gyro_cfg >> 3 & 0x03 */
        unsigned char gyroFsr;
        /* Matches accel_cfg >> 3 & 0x03 */
        unsigned char accelFsr;
        /* Enabled sensors. Uses same masks as fifo_en, NOT pwr_mgmt_2. */
        unsigned char sensors;
        /* Matches config register. */
        unsigned char lpf;
        unsigned char clkSrc;
        /* Sample rate, NOT rate divider. */
        unsigned short sampleRate;
        /* Matches fifo_en register. */
        unsigned char fifoEnable;
        /* Matches int enable register. */
        unsigned char intEnable;
        /* 1 if devices on auxiliary I2C bus appear on the primary. */
        unsigned char bypassMode;
        /* 1 if half-sensitivity.
         * NOTE: This doesn't belong here, but everything else in hw_s is const,
         * and this allows us to save some precious RAM.
         */
        unsigned char accelHalf;
        /* 1 if device in low-power accel-only mode. */
        unsigned char lpAccelMode;
        /* 1 if interrupts are only triggered on motion events. */
        unsigned char intMotionOnly;
        MotionIntCache cache;
        /* 1 for active low interrupts. */
        unsigned char activeLowInt;
        /* 1 for latched interrupts. */
        unsigned char latchedInt;
        /* 1 if DMP is enabled. */
        unsigned char dmpOn;
        /* Ensures that DMP will only be loaded once. */
        unsigned char dmpLoaded;
        /* Sampling rate used when DMP is enabled. */
        unsigned short dmpSampleRate;
        unsigned short compassSampleRate;
        unsigned char compassAddr;
        short magSensAdj[3];
    };
    
    /* Information for self-test-> */
    struct SelfTest 
    {
        unsigned long gyroSens;
        unsigned long accelSens;
        unsigned char regRateDiv;
        unsigned char regLpf;
        unsigned char regGyroFsr;
        unsigned char regAccelFsr;
        unsigned short waitMs;
        unsigned char packetThresh;
        float minDps;
        float maxDps;
        float maxGyroVar;
        float minG;
        float maxG;
        float maxAccelVar;
    };
    
    /* Gyro driver state variables. */
    struct State 
    {
        const DriverRegisters *reg;
        const DeviceInfo *hw;
        ChipCfg chipCfg;
        const SelfTest *test;
    };
};
#endif
