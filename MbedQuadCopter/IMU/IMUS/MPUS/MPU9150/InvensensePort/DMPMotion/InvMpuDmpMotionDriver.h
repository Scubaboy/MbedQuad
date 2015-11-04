#ifndef InvMpuDmpMotionDriver_H
#define InvMpuDmpMotionDriver_H
#include "MPU9150StructDefs.h"
#include "InvMPU.h"
#include "TimerBase.h"

class InvMpuDmpMotionDriver
{
    public:
        InvMpuDmpMotionDriver(InvMPU* invMPU, TimerBase* systemTimer);
        
        /* Set up functions. */
        void DmpInitStructures();
        int DmpSelectDevice(int device);
        int DmpLoadMotionDriverFirmware(void);
        int DmpSetFifoRate(unsigned short rate);
        int DmpGetFifoRate(unsigned short *rate);
        int DmpEnableFeature(unsigned short mask);
        int DmpGetEnabledFeatures(unsigned short *mask);
        int DmpSetInterruptMode(unsigned char mode);
        int DmpSetOrientation(unsigned short orient);
        int DmpSetGyroBias(long *bias);
        int DmpSetAccelBias(long *bias); 
        
        /* LP quaternion functions. */
        int DmpEnableLpQuat(unsigned char enable);
        int DmpEnable6xLpQuat(unsigned char enable);
        
        /* DMP gyro calibration functions. */
        int DmpEnableGyroCal(unsigned char enable);

        /* Read function. This function should be called whenever the MPU interrupt is
         * detected.
         */
        int DmpReadFifo(short *gyro, short *accel, long *quat,
            unsigned long *timestamp, short *sensors, unsigned char *more); 
            
        private:
            MPU9150StructDefs::DmpS dmpArray[MPU9150Consts::MPU_MAX_DEVICES];
            MPU9150StructDefs::DmpS *dmp;
            int deviceIndex;
            InvMPU* invMPU;
            TimerBase* systemTimer;
};
#endif