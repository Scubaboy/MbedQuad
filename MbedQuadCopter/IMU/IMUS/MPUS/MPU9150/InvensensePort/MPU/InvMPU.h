#ifndef InvMPU_H
#define InvMPU_H
#include "MPU9150StructDefs.h"
#include "I2CBusCtrl.h"
#include "DebugLoggerBase.h"
#include "TimerBase.h"

class InvMPU
{
    public:
        InvMPU(I2CBusCtrl* busCtrl,TimerBase* systemTimer, DebugLoggerBase* debugLogger);
        
        /* Set up APIs */
        void MpuInitStructures();
        int MpuInit(MPU9150StructDefs::IntParamS *intParam);
        int MpuInitSlave(void);
        int MpuSetBypass(unsigned char bypassOn);
        int MpuSelectDevice(int device);
        
        /* Configuration APIs */
        int MpuLpAccelMode(unsigned char rate);
        int MpuLpMotionInterrupt(unsigned short thresh, unsigned char time, unsigned char lpaFreq);
        int MpuSetIntLevel(unsigned char activeLow);
        int MpuSetIntLatched(unsigned char enable);
        int MpuSetDmpState(unsigned char enable);
        int MpuGetDmpState(unsigned char *enabled);
        int MpuGetLpf(unsigned short *lpf);
        int MpuSetLpf(unsigned short lpf);   
        int MpuGetGyroFsr(unsigned short *fsr);
        int MpuSetGyroFsr(unsigned short fsr);  
        int MpuGetAccelFsr(unsigned char *fsr);
        int MpuSetAccelFsr(unsigned char fsr);  
        int MpuGetCompassFsr(unsigned short *fsr);  
        int MpuGetGyroSens(float *sens);
        int MpuGetAccelSens(unsigned short *sens);     
        int MpuGetSampleRate(unsigned short *rate);
        int MpuSetSampleRate(unsigned short rate);
        int MpuGetCompassSampleRate(unsigned short *rate);
        int MpuSetCompassSampleRate(unsigned short rate);     
        int MpuGetFifoConfig(unsigned char *sensors);
        int MpuConfigureFifo(unsigned char sensors);     
        int MpuGetPowerState(unsigned char *powerOn);
        int MpuSetSensors(unsigned char sensors);    
        int MpuSetAccelBias(const long *accelBias);
        
        /* Data getter/setter APIs */
        int MpuGetGyroReg(short *data, unsigned long *timestamp);
        int MpuGetAccelReg(short *data, unsigned long *timestamp);
        int MpuGetCompassReg(short *data, unsigned long *timestamp);
        int MpuGetTemperature(long *data, unsigned long *timestamp);
        int MpuGetIntStatus(short *status);
        int MpuReadFifo(short *gyro, short *accel, unsigned long *timestamp, unsigned char *sensors, unsigned char *more);
        int MpuReadFifoStream(unsigned short length, char *data, unsigned char *more);
        int MpuResetFifo(void);
        int MpuWriteMem(unsigned short memAddr, unsigned short length, unsigned char *data);
        int MpuReadMem(unsigned short memAddr, unsigned short length, unsigned char *data);
        int MpuLoadFirmware(unsigned short length, const unsigned char *firmware, unsigned short startAddr, unsigned short sampleRate);
        int MpuRegDump(void);
        int MpuReadReg(unsigned char reg, unsigned char *data);
    private:
        int SetIntEnable (unsigned char enable);
        int SetupCompass(void);
    private:
        MPU9150StructDefs::DriverRegisters regArray[MPU9150Consts::MPU_MAX_DEVICES];
        MPU9150StructDefs::DeviceInfo hwArray[MPU9150Consts::MPU_MAX_DEVICES];
        MPU9150StructDefs::SelfTest testArray[MPU9150Consts::MPU_MAX_DEVICES];
        MPU9150StructDefs::State stateArray[MPU9150Consts::MPU_MAX_DEVICES];
        I2CBusCtrl* busCtrl;
        MPU9150StructDefs::DriverRegisters *reg;
        MPU9150StructDefs::DeviceInfo *deviceInfo;
        MPU9150StructDefs::SelfTest *selfTest;
        MPU9150StructDefs::State *state;
        int deviceIndex;
        TimerBase* systemTimer;
        DebugLoggerBase* debugLogger;
};
#endif