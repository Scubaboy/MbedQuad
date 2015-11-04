#include "InvMpuDmpMotionDriver.h"
#include "mbed.h"
#include "DMPHexMotionDriver6-1.h"

const unsigned short sStartAddress = 0x0400;
const unsigned char regsEnd[12] = {MPU9150Consts::DINAFE, 
                                    MPU9150Consts::DINAF2, 
                                    MPU9150Consts::DINAAB, 0xc4, 
                                    MPU9150Consts::DINAAA, 
                                    MPU9150Consts::DINAF1, 
                                    MPU9150Consts::DINADF, 
                                    MPU9150Consts::DINADF, 
                                    0xBB, 
                                    0xAF, 
                                    MPU9150Consts::DINADF, 
                                    MPU9150Consts::DINADF};

const unsigned char regsContinuous[11] =
        {0xd8, 0xb1, 0xb9, 0xf3, 0x8b, 0xa3, 0x91, 0xb6, 0x09, 0xb4, 0xd9};

const unsigned char regsGesture[11] =
        {0xda, 0xb1, 0xb9, 0xf3, 0x8b, 0xa3, 0x91, 0xb6, 0xda, 0xb4, 0xda};

const unsigned char gyro_axes[3] = {MPU9150Consts::DINA4C, MPU9150Consts::DINACD, MPU9150Consts::DINA6C};

const unsigned char accel_axes[3] = {MPU9150Consts::DINA0C, MPU9150Consts::DINAC9, MPU9150Consts::DINA2C};

const unsigned char gyro_sign[3] = {MPU9150Consts::DINA36, MPU9150Consts::DINA56, MPU9150Consts::DINA76};

const unsigned char accel_sign[3] = {MPU9150Consts::DINA26, MPU9150Consts::DINA46, MPU9150Consts::DINA66};
                                                    
InvMpuDmpMotionDriver::InvMpuDmpMotionDriver(InvMPU* invMPU, TimerBase* systemTimer)
{
    this->invMPU = invMPU;  
    this->systemTimer = systemTimer;
}

void InvMpuDmpMotionDriver::DmpInitStructures()
{
    dmp->tap_cb = NULL;
    dmp->android_orient_cb = NULL;
    dmp->orient = 0;
    dmp->featureMask = 0;
    dmp->fifoRate = 0;
    dmp->packetLength = 0;    
}
        
int InvMpuDmpMotionDriver::DmpSelectDevice(int device)
{
    if ((device < 0) || (device >= MPU9150Consts::MPU_MAX_DEVICES))
    
    return -1;

    deviceIndex = device;
    dmp = dmpArray + device;
    return 0; 
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpLoadMotionDriverFirmware(void)
{
    return this->invMPU->MpuLoadFirmware(
                                        MPU9150Consts::DMP_CODE_SIZE, 
                                        MotionDriver6_1::dmpMemory, 
                                        sStartAddress, 
                                        MPU9150Consts::DMP_SAMPLE_RATE);    
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpSetFifoRate(unsigned short rate)
{
    unsigned short div;
    unsigned char tmp[8];

    if (rate > MPU9150Consts::DMP_SAMPLE_RATE)
        return -1;
        
    div = MPU9150Consts::DMP_SAMPLE_RATE / rate - 1;
    tmp[0] = (unsigned char)((div >> 8) & 0xFF);
    tmp[1] = (unsigned char)(div & 0xFF);
    
    if (this->invMPU->MpuWriteMem(MPU9150Consts::D_0_22, 2, tmp))
        return -1;
        
    if (this->invMPU->MpuWriteMem(MPU9150Consts::CFG_6, 12, (unsigned char*)regsEnd))
        return -1;

    dmp->fifoRate = rate;
    
    return 0;   
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpGetFifoRate(unsigned short *rate)
{
    rate[0] = dmp->fifoRate;
    return 0;
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpEnableFeature(unsigned short mask)
{
    unsigned char tmp[10];

    /* TODO: All of these settings can probably be integrated into the default
     * DMP image.
     */
    /* Set integration scale factor. */
    tmp[0] = (unsigned char)((MPU9150Consts::GYRO_SF >> 24) & 0xFF);
    tmp[1] = (unsigned char)((MPU9150Consts::GYRO_SF >> 16) & 0xFF);
    tmp[2] = (unsigned char)((MPU9150Consts::GYRO_SF >> 8) & 0xFF);
    tmp[3] = (unsigned char)(MPU9150Consts::GYRO_SF & 0xFF);
    
    this->invMPU->MpuWriteMem(MPU9150Consts::D_0_104, 4, tmp);

    /* Send sensor data to the FIFO. */
    tmp[0] = 0xA3;
    
    if (mask & MPU9150Consts::DMP_FEATURE_SEND_RAW_ACCEL) 
    {
        tmp[1] = 0xC0;
        tmp[2] = 0xC8;
        tmp[3] = 0xC2;
    } 
    else 
    {
        tmp[1] = 0xA3;
        tmp[2] = 0xA3;
        tmp[3] = 0xA3;
    }
    
    if (mask & MPU9150Consts::DMP_FEATURE_SEND_ANY_GYRO) 
    {
        tmp[4] = 0xC4;
        tmp[5] = 0xCC;
        tmp[6] = 0xC6;
    } 
    else 
    {
        tmp[4] = 0xA3;
        tmp[5] = 0xA3;
        tmp[6] = 0xA3;
    }
    
    tmp[7] = 0xA3;
    tmp[8] = 0xA3;
    tmp[9] = 0xA3;
    this->invMPU->MpuWriteMem(MPU9150Consts::CFG_15,10,tmp);

    /* Send gesture data to the FIFO. */
    if (mask & (MPU9150Consts::DMP_FEATURE_TAP | MPU9150Consts::DMP_FEATURE_ANDROID_ORIENT))
        tmp[0] = MPU9150Consts::DINA20;
    else
        tmp[0] = 0xD8;
        
    this->invMPU->MpuWriteMem(MPU9150Consts::CFG_27,1,tmp);

    if (mask & MPU9150Consts::DMP_FEATURE_GYRO_CAL)
        this->DmpEnableGyroCal(1);
    else
        this->DmpEnableGyroCal(0);

    if (mask & MPU9150Consts::DMP_FEATURE_SEND_ANY_GYRO) 
    {
        if (mask & MPU9150Consts::DMP_FEATURE_SEND_CAL_GYRO) 
        {
            tmp[0] = 0xB2;
            tmp[1] = 0x8B;
            tmp[2] = 0xB6;
            tmp[3] = 0x9B;
        } 
        else 
        {
            tmp[0] = MPU9150Consts::DINAC0;
            tmp[1] = MPU9150Consts::DINA80;
            tmp[2] = MPU9150Consts::DINAC2;
            tmp[3] = MPU9150Consts::DINA90;
        }
        
        this->invMPU->MpuWriteMem(MPU9150Consts::CFG_GYRO_RAW_DATA, 4, tmp);
    }

    if (mask & MPU9150Consts::DMP_FEATURE_LP_QUAT)
        this->DmpEnableLpQuat(1);
    else
        this->DmpEnableLpQuat(0);

    if (mask & MPU9150Consts::DMP_FEATURE_6X_LP_QUAT)
        this->DmpEnable6xLpQuat(1);
    else
        this->DmpEnable6xLpQuat(0);

    /* Pedometer is always enabled. */
    dmp->featureMask = mask | MPU9150Consts::DMP_FEATURE_PEDOMETER;
    this->invMPU->MpuResetFifo();

    dmp->packetLength = 0;
    
    if (mask & MPU9150Consts::DMP_FEATURE_SEND_RAW_ACCEL)
        dmp->packetLength += 6;
        
    if (mask & MPU9150Consts::DMP_FEATURE_SEND_ANY_GYRO)
        dmp->packetLength += 6;
        
    if (mask & (MPU9150Consts::DMP_FEATURE_LP_QUAT | MPU9150Consts::DMP_FEATURE_6X_LP_QUAT))
        dmp->packetLength += 16;
        
    if (mask & (MPU9150Consts::DMP_FEATURE_TAP | MPU9150Consts::DMP_FEATURE_ANDROID_ORIENT))
        dmp->packetLength += 4;

    return 0;   
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpGetEnabledFeatures(unsigned short *mask)
{
    mask[0] = this->dmp->featureMask;
    return 0;   
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpSetInterruptMode(unsigned char mode)
{
    switch (mode) 
    {
        case MPU9150Consts::DMP_INT_CONTINUOUS:
            return this->invMPU->MpuWriteMem(MPU9150Consts::CFG_FIFO_ON_EVENT, 11, (unsigned char*)regsContinuous);
        case MPU9150Consts::DMP_INT_GESTURE:
            return this->invMPU->MpuWriteMem(MPU9150Consts::CFG_FIFO_ON_EVENT, 11, (unsigned char*)regsGesture);
        default:
            return -1;
    }    
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpSetOrientation(unsigned short orient)
{
    unsigned char gyro_regs[3], accel_regs[3];
    
    gyro_regs[0] = gyro_axes[orient & 3];
    gyro_regs[1] = gyro_axes[(orient >> 3) & 3];
    gyro_regs[2] = gyro_axes[(orient >> 6) & 3];
    accel_regs[0] = accel_axes[orient & 3];
    accel_regs[1] = accel_axes[(orient >> 3) & 3];
    accel_regs[2] = accel_axes[(orient >> 6) & 3];

    /* Chip-to-body, axes only. */
    if (this->invMPU->MpuWriteMem(MPU9150Consts::FCFG_1, 3, gyro_regs))
        return -1;
    if (this->invMPU->MpuWriteMem(MPU9150Consts::FCFG_2, 3, accel_regs))
        return -1;

    memcpy(gyro_regs, gyro_sign, 3);
    memcpy(accel_regs, accel_sign, 3);
    
    if (orient & 4) 
    {
        gyro_regs[0] |= 1;
        accel_regs[0] |= 1;
    }
    
    if (orient & 0x20) 
    {
        gyro_regs[1] |= 1;
        accel_regs[1] |= 1;
    }
    
    if (orient & 0x100) 
    {
        gyro_regs[2] |= 1;
        accel_regs[2] |= 1;
    }

    /* Chip-to-body, sign only. */
    if (this->invMPU->MpuWriteMem(MPU9150Consts::FCFG_3, 3, gyro_regs))
        return -1;
        
    if (this->invMPU->MpuWriteMem(MPU9150Consts::FCFG_7, 3, accel_regs))
        return -1;
        
    dmp->orient = orient;
    
    return 0;    
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpSetGyroBias(long *bias)
{
    long gyroBiasBody[3];
    unsigned char regs[4];

    gyroBiasBody[0] = bias[dmp->orient & 3];
    if (dmp->orient & 4)
        gyroBiasBody[0] *= -1;
    gyroBiasBody[1] = bias[(dmp->orient >> 3) & 3];
    if (dmp->orient & 0x20)
        gyroBiasBody[1] *= -1;
    gyroBiasBody[2] = bias[(dmp->orient >> 6) & 3];
    if (dmp->orient & 0x100)
        gyroBiasBody[2] *= -1;

#ifdef EMPL_NO_64BIT
    gyroBiasBody[0] = (long)(((float)gyroBiasBody[0] * MPU9150Consts::GYRO_SF) / 1073741824.f);
    gyroBiasBody[1] = (long)(((float)gyroBiasBody[1] * MPU9150Consts::GYRO_SF) / 1073741824.f);
    gyroBiasBody[2] = (long)(((float)gyroBiasBody[2] * MPU9150Consts::GYRO_SF) / 1073741824.f);
#else
    gyroBiasBody[0] = (long)(((long long)gyroBiasBody[0] * MPU9150Consts::GYRO_SF) >> 30);
    gyroBiasBody[1] = (long)(((long long)gyroBiasBody[1] * MPU9150Consts::GYRO_SF) >> 30);
    gyroBiasBody[2] = (long)(((long long)gyroBiasBody[2] * MPU9150Consts::GYRO_SF) >> 30);
#endif

    regs[0] = (unsigned char)((gyroBiasBody[0] >> 24) & 0xFF);
    regs[1] = (unsigned char)((gyroBiasBody[0] >> 16) & 0xFF);
    regs[2] = (unsigned char)((gyroBiasBody[0] >> 8) & 0xFF);
    regs[3] = (unsigned char)(gyroBiasBody[0] & 0xFF);
    if (this->invMPU->MpuWriteMem(MPU9150Consts::D_EXT_GYRO_BIAS_X, 4, regs))
        return -1;

    regs[0] = (unsigned char)((gyroBiasBody[1] >> 24) & 0xFF);
    regs[1] = (unsigned char)((gyroBiasBody[1] >> 16) & 0xFF);
    regs[2] = (unsigned char)((gyroBiasBody[1] >> 8) & 0xFF);
    regs[3] = (unsigned char)(gyroBiasBody[1] & 0xFF);
    if (this->invMPU->MpuWriteMem(MPU9150Consts::D_EXT_GYRO_BIAS_Y, 4, regs))
        return -1;

    regs[0] = (unsigned char)((gyroBiasBody[2] >> 24) & 0xFF);
    regs[1] = (unsigned char)((gyroBiasBody[2] >> 16) & 0xFF);
    regs[2] = (unsigned char)((gyroBiasBody[2] >> 8) & 0xFF);
    regs[3] = (unsigned char)(gyroBiasBody[2] & 0xFF);
    return this->invMPU->MpuWriteMem(MPU9150Consts::D_EXT_GYRO_BIAS_Z, 4, regs);  
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpSetAccelBias(long *bias)
{
    long accelBiasBody[3];
    unsigned char regs[12];
    long long accelSf;
    unsigned short accelSens;

    this->invMPU->MpuGetAccelSens(&accelSens);
    accelSf = (long long)accelSens << 15;
    wait_ms(1);

    accelBiasBody[0] = bias[dmp->orient & 3];
    if (dmp->orient & 4)
        accelBiasBody[0] *= -1;
    accelBiasBody[1] = bias[(dmp->orient >> 3) & 3];
    if (dmp->orient & 0x20)
        accelBiasBody[1] *= -1;
    accelBiasBody[2] = bias[(dmp->orient >> 6) & 3];
    if (dmp->orient & 0x100)
        accelBiasBody[2] *= -1;

#ifdef EMPL_NO_64BIT
    accelBiasBody[0] = (long)(((float)accelBiasBody[0] * accelSf) / 1073741824.f);
    accelBiasBody[1] = (long)(((float)accelBiasBody[1] * accelSf) / 1073741824.f);
    accelBiasBody[2] = (long)(((float)accelBiasBody[2] * accelSf) / 1073741824.f);
#else
    accelBiasBody[0] = (long)(((long long)accelBiasBody[0] * accelSf) >> 30);
    accelBiasBody[1] = (long)(((long long)accelBiasBody[1] * accelSf) >> 30);
    accelBiasBody[2] = (long)(((long long)accelBiasBody[2] * accelSf) >> 30);
#endif

    regs[0] = (unsigned char)((accelBiasBody[0] >> 24) & 0xFF);
    regs[1] = (unsigned char)((accelBiasBody[0] >> 16) & 0xFF);
    regs[2] = (unsigned char)((accelBiasBody[0] >> 8) & 0xFF);
    regs[3] = (unsigned char)(accelBiasBody[0] & 0xFF);
    regs[4] = (unsigned char)((accelBiasBody[1] >> 24) & 0xFF);
    regs[5] = (unsigned char)((accelBiasBody[1] >> 16) & 0xFF);
    regs[6] = (unsigned char)((accelBiasBody[1] >> 8) & 0xFF);
    regs[7] = (unsigned char)(accelBiasBody[1] & 0xFF);
    regs[8] = (unsigned char)((accelBiasBody[2] >> 24) & 0xFF);
    regs[9] = (unsigned char)((accelBiasBody[2] >> 16) & 0xFF);
    regs[10] = (unsigned char)((accelBiasBody[2] >> 8) & 0xFF);
    regs[11] = (unsigned char)(accelBiasBody[2] & 0xFF);
    return this->invMPU->MpuWriteMem(MPU9150Consts::D_ACCEL_BIAS, 12, regs);       
} 
        
/* LP quaternion functions. */
int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpEnableLpQuat(unsigned char enable)
{
    unsigned char regs[4];
    
    if (enable) 
    {
        regs[0] = MPU9150Consts::DINBC0;
        regs[1] = MPU9150Consts::DINBC2;
        regs[2] = MPU9150Consts::DINBC4;
        regs[3] = MPU9150Consts::DINBC6;
    }
    else
        memset(regs, 0x8B, 4);

    this->invMPU->MpuWriteMem(MPU9150Consts::CFG_LP_QUAT, 4, regs);

    return this->invMPU->MpuResetFifo();   
}

int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpEnable6xLpQuat(unsigned char enable)
{
    unsigned char regs[4];
    
    if (enable) 
    {
        regs[0] = MPU9150Consts::DINA20;
        regs[1] = MPU9150Consts::DINA28;
        regs[2] = MPU9150Consts::DINA30;
        regs[3] = MPU9150Consts::DINA38;
    } 
    else
        memset(regs, 0xA3, 4);

    this->invMPU->MpuWriteMem(MPU9150Consts::CFG_8, 4, regs);

    return this->invMPU->MpuResetFifo();      
}
        
/* DMP gyro calibration functions. */
int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpEnableGyroCal(unsigned char enable)
{
    if (enable) 
    {
        unsigned char regs[9] = {0xb8, 0xaa, 0xb3, 0x8d, 0xb4, 0x98, 0x0d, 0x35, 0x5d};
        return this->invMPU->MpuWriteMem(MPU9150Consts::CFG_MOTION_BIAS, 9, regs);
    } 
    else 
    {
        unsigned char regs[9] = {0xb8, 0xaa, 0xaa, 0xaa, 0xb0, 0x88, 0xc3, 0xc5, 0xc7};
        return this->invMPU->MpuWriteMem(MPU9150Consts::CFG_MOTION_BIAS, 9, regs);
    }
       
}

/* Read function. This function should be called whenever the MPU interrupt is
 * detected.
 */
int InvMpuDmpMotionDriver::InvMpuDmpMotionDriver::DmpReadFifo(short *gyro, short *accel, long *quat,
            unsigned long *timestamp, short *sensors, unsigned char *more)
{
    unsigned char fifoData[MPU9150Consts::MAX_PACKET_LENGTH];
    unsigned char ii = 0;
    int errCode;

    /* TODO: sensors[0] only changes when dmp_enable_feature is called. We can
     * cache this value and save some cycles.
     */
    sensors[0] = 0;

    /* Get a packet. */
    if ((errCode = this->invMPU->MpuReadFifoStream(dmp->packetLength, (char*)fifoData, more)))
        return errCode;

    /* Parse DMP packet. */
    if (dmp->featureMask & (MPU9150Consts::DMP_FEATURE_LP_QUAT | MPU9150Consts::DMP_FEATURE_6X_LP_QUAT)) {
#ifdef FIFO_CORRUPTION_CHECK
        long quatQ14[4], quatMagSq;
#endif
        quat[0] = ((long)fifoData[0] << 24) | ((long)fifoData[1] << 16) |
            ((long)fifoData[2] << 8) | fifoData[3];
        quat[1] = ((long)fifoData[4] << 24) | ((long)fifoData[5] << 16) |
            ((long)fifoData[6] << 8) | fifoData[7];
        quat[2] = ((long)fifoData[8] << 24) | ((long)fifoData[9] << 16) |
            ((long)fifoData[10] << 8) | fifoData[11];
        quat[3] = ((long)fifoData[12] << 24) | ((long)fifoData[13] << 16) |
            ((long)fifoData[14] << 8) | fifoData[15];
        ii += 16;
#ifdef FIFO_CORRUPTION_CHECK
        /* We can detect a corrupted FIFO by monitoring the quaternion data and
         * ensuring that the magnitude is always normalized to one. This
         * shouldn't happen in normal operation, but if an I2C error occurs,
         * the FIFO reads might become misaligned.
         *
         * Let's start by scaling down the quaternion data to avoid long long
         * math.
         */
        quatQ14[0] = quat[0] >> 16;
        quatQ14[1] = quat[1] >> 16;
        quatQ14[2] = quat[2] >> 16;
        quatQ14[3] = quat[3] >> 16;
        quatMagSq = quatQ14[0] * quatQ14[0] + quatQ14[1] * quatQ14[1] +
            quatQ14[2] * quatQ14[2] + quatQ14[3] * quatQ14[3];
        if ((quatMagSq < MPU9150Consts::QUAT_MAG_SQ_MIN) ||
            (quatMagSq > MPU9150Consts::QUAT_MAG_SQ_MAX)) {
            /* Quaternion is outside of the acceptable threshold. */
            mpu_reset_fifo();
            sensors[0] = 0;
            return 3;
        };
        sensors[0] |= MPU9150Consts::INV_WXYZ_QUAT;
#endif
    }

    if (dmp->featureMask & MPU9150Consts::DMP_FEATURE_SEND_RAW_ACCEL) {
        accel[0] = ((short)fifoData[ii+0] << 8) | fifoData[ii+1];
        accel[1] = ((short)fifoData[ii+2] << 8) | fifoData[ii+3];
        accel[2] = ((short)fifoData[ii+4] << 8) | fifoData[ii+5];
        ii += 6;
        sensors[0] |= MPU9150Consts::INV_XYZ_ACCEL;
    }

    if (dmp->featureMask & MPU9150Consts::DMP_FEATURE_SEND_ANY_GYRO) {
        gyro[0] = ((short)fifoData[ii+0] << 8) | fifoData[ii+1];
        gyro[1] = ((short)fifoData[ii+2] << 8) | fifoData[ii+3];
        gyro[2] = ((short)fifoData[ii+4] << 8) | fifoData[ii+5];
        ii += 6;
        sensors[0] |= MPU9150Consts::INV_XYZ_GYRO;
    }
#ifdef MPU_MAXIMAL
    /* Gesture data is at the end of the DMP packet. Parse it and call
     * the gesture callbacks (if registered).
     */
   // if (dmp->featureMask & (MPU9150Consts::DMP_FEATURE_TAP | MPU9150Consts::DMP_FEATURE_ANDROID_ORIENT))
   //     decode_gesture(fifoData + ii);
#endif // MPU_MAXIMAL
    if (timestamp)
    *timestamp = this->systemTimer->Read_Ms();// get_ms(timestamp);
    return 0;   
}