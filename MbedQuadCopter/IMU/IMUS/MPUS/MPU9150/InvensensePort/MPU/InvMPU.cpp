#include "InvMPU.h"
#include "MPU9150Enums.h"
#include "mbed.h"
#include "Range.h"

const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::IMU;

/* Must divide evenly into st.hw->bank_size to avoid bank crossings. */
const int LOADCHUNK = 16;
    
InvMPU::InvMPU(I2CBusCtrl* busCtrl, TimerBase* systemTimer, DebugLoggerBase* debugLogger)
{
    this->busCtrl =  busCtrl;
    this->deviceIndex = 0;  
    this->systemTimer = systemTimer; 
    this->debugLogger = debugLogger; 
}
        
/* Set up APIs */
void InvMPU::MpuInitStructures()
{
    this->reg->whoAmI         = 0x75;
    this->reg->rateDiv         = 0x19;
    this->reg->lpf              = 0x1A;
    this->reg->prodId          = 0x0C;
    this->reg->userCtrl        = 0x6A;
    this->reg->fifoEn          = 0x23;
    this->reg->gyroCfg         = 0x1B;
    this->reg->accelCfg        = 0x1C;
    this->reg->motionThr       = 0x1F;
    this->reg->motionDur       = 0x20;
    this->reg->fifoCountH     = 0x72;
    this->reg->fifoRW         = 0x74;
    this->reg->rawGyro         = 0x43;
    this->reg->rawAccel        = 0x3B;
    this->reg->temp             = 0x41;
    this->reg->intEnable       = 0x38;
    this->reg->dmpIntStatus   = 0x39;
    this->reg->intStatus       = 0x3A;
    this->reg->pwrMgmt1       = 0x6B;
    this->reg->pwrMgmt2       = 0x6C;
    this->reg->intPinCfg      = 0x37;
    this->reg->memRW          = 0x6F;
    this->reg->accelOffs       = 0x06;
    this->reg->i2cMst          = 0x24;
    this->reg->bankSel         = 0x6D;
    this->reg->memStartAddr   = 0x6E;
    this->reg->prgmStartH     = 0x70;
    this->reg->rawCompass      = 0x49;
    this->reg->ygOffsTc       = 0x01;
    this->reg->s0Addr          = 0x25;
    this->reg->s0Reg           = 0x26;
    this->reg->s0Ctrl          = 0x27;
    this->reg->s1Addr          = 0x28;
    this->reg->s1Reg           = 0x29;
    this->reg->s1Ctrl          = 0x2A;
    this->reg->s4Ctrl          = 0x34;
    this->reg->s0Do            = 0x63;
    this->reg->s1Do            = 0x64;
    this->reg->i2cDelayCtrl   = 0x67;
    
    switch (deviceIndex) {
      case 0:
        deviceInfo->addr = 0x68<<1;
        break;

      case 1:
        deviceInfo->addr = 0x69<<1;
        break;
    }
    
    this->deviceInfo->maxFifo          = 1024;
    this->deviceInfo->numReg           = 118;
    this->deviceInfo->tempSens         = 340;
    this->deviceInfo->tempOffset       = -521;
    this->deviceInfo->bankSize         = 256;
    this->deviceInfo->compassFsr      = MPU9150Consts::AK89xx_FSR;

    this->selfTest->gyroSens      = 32768/250;
    this->selfTest->accelSens     = 32768/16;
    this->selfTest->regRateDiv   = 0;    /* 1kHz. */
    this->selfTest->regLpf        = 1;    /* 188Hz. */
    this->selfTest->regGyroFsr   = 0;    /* 250dps. */
    this->selfTest->regAccelFsr  = 0x18; /* 16g. */
    this->selfTest->waitMs        = 50;
    this->selfTest->packetThresh  = 5;    /* 5% */
    this->selfTest->minDps        = 10.f;
    this->selfTest->maxDps        = 105.f;
    this->selfTest->maxGyroVar   = 0.14f;
    this->selfTest->minG          = 0.3f;
    this->selfTest->maxG          = 0.95f;
    this->selfTest->maxAccelVar  = 0.14f;

    state->reg = this->reg;
    state->hw = this->deviceInfo;
    state->test = this->selfTest;
}

int InvMPU::MpuInit(MPU9150StructDefs::IntParamS *intParam)
{
    char data[2];
    int errCode;

    /* Reset device. */
    data[0] = this->state->reg->pwrMgmt1;
    data[1] = MPU9150Consts::BIT_RESET;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -1;
    wait_ms(100);

    /* Wake up chip. */
    data[0] = this->state->reg->pwrMgmt1;
    data[1] = 0x00;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -2;

/*
    // Check product revision.
    if (i2c_read(st->hw->addr, st->reg->accel_offs, 6, data))
        return -3;
    rev = ((data[5] & 0x01) << 2) | ((data[3] & 0x01) << 1) |
        (data[1] & 0x01);

    if (rev) 
    {
        // Congrats, these parts are better.
        if (rev == 1)
            st->chip_cfg.accel_half = 1;
        else if (rev == 2)
            st->chip_cfg.accel_half = 0;
        else {

            Serial.print("Unsupported software product rev: "); Serial.println(rev);

            return -4;
        }
    } else 
    {
        if (i2c_read(st->hw->addr, st->reg->prod_id, 1, data))
            return -5;
        rev = data[0] & 0x0F;
        if (!rev) 
        {

            Serial.println("Product ID read as 0 indicates device is either incompatible or an MPU3050");

            return -6;
        } 
        else if (rev == 4) 
        {

            Serial.println("Half sensitivity part found.");

            st->chip_cfg.accel_half = 1;
        } 
        else
            st->chip_cfg.accel_half = 0;
    }
*/

    /* Set to invalid values to ensure no I2C writes are skipped. */
    this->state->chipCfg.accelHalf = 0;
    this->state->chipCfg.sensors = 0xFF;
    this->state->chipCfg.gyroFsr = 0xFF;
    this->state->chipCfg.accelFsr = 0xFF;
    this->state->chipCfg.lpf = 0xFF;
    this->state->chipCfg.sampleRate = 0xFFFF;
    this->state->chipCfg.fifoEnable = 0xFF;
    this->state->chipCfg.bypassMode = 0xFF;
    /* mpu_set_sensors always preserves this setting. */
    this->state->chipCfg.clkSrc = MPU9150Enums::INV_CLK_PLL;
    /* Handled in next call to mpu_set_bypass. */
    this->state->chipCfg.activeLowInt = 1;
    this->state->chipCfg.latchedInt = 0;
    this->state->chipCfg.intMotionOnly = 0;
    this->state->chipCfg.lpAccelMode = 0;
    memset(&this->state->chipCfg.cache, 0, sizeof(this->state->chipCfg.cache));
    this->state->chipCfg.dmpOn = 0;
    this->state->chipCfg.dmpLoaded = 0;
    this->state->chipCfg.dmpSampleRate = 0;

    if (this->MpuSetGyroFsr(2000))
        return -10;
    if (this->MpuSetAccelFsr(2))
        return -11;
    if (this->MpuSetLpf(42))
        return -12;
    if (this->MpuSetSampleRate(50))
        return -13;
    if (this->MpuConfigureFifo(0))
        return -14;

    errCode = SetupCompass();
    if (MpuSetCompassSampleRate(10))
        return -15;
    
    //if (inParam)
     //   ReIntCb(intParam);
/*
#ifdef AK89xx_SECONDARY
#ifdef MPU_DEBUG
    Serial.println("Setting up compass");
#endif
    errCode = setup_compass();
    if (errCode != 0) {
#ifdef MPU_DEBUG
       Serial.print("Setup compass failed: "); Serial.println(errCode); 
#endif
    }
    if (mpu_set_compass_sample_rate(10))
        return -15;
#else
*/
    /* Already disabled by setup_compass. */
    //if (MpuSetBypass(0))
    //    return -16;
//#endif

    MpuSetSensors(0);
    return 0;    
}

int InvMPU::MpuInitSlave(void)
{
    
}

int InvMPU::MpuSetBypass(unsigned char bypassOn)
{
    char tmp;
    char writeData[2];
    
    if (this->state->chipCfg.bypassMode == bypassOn)
        return 0;

    if (bypassOn) 
    {
        if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->userCtrl,&tmp,1))
            return -1;
        tmp &= ~MPU9150Consts::BIT_AUX_IF_EN;
        
        writeData[0] = this->state->reg->userCtrl;
        writeData[1] = tmp; 
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, writeData , 2))
            return -1;
        wait_ms(3);
        
        tmp = MPU9150Consts::BIT_BYPASS_EN;
        if (this->state->chipCfg.activeLowInt)
            tmp |= MPU9150Consts::BIT_ACTL;
        if (this->state->chipCfg.latchedInt)
            tmp |= MPU9150Consts::BIT_LATCH_EN | MPU9150Consts::BIT_ANY_RD_CLR;
        writeData[0] = this->state->reg->intPinCfg;
        writeData[1] = tmp;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, writeData , 2))
            return -1;
    } 
    else 
    {
        /* Enable I2C master mode if compass is being used. */
        if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->userCtrl,&tmp,1))
            return -1;
            
        if (this->state->chipCfg.sensors & MPU9150Consts::INV_XYZ_COMPASS)
            tmp |= MPU9150Consts::BIT_AUX_IF_EN;
        else
            tmp &= ~MPU9150Consts::BIT_AUX_IF_EN;
            
        writeData[0] = this->state->reg->userCtrl;
        writeData[1] = tmp; 
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, writeData , 2))
            return -1;
        wait_ms(3);
        
        if (this->state->chipCfg.activeLowInt)
            tmp = MPU9150Consts::BIT_ACTL;
        else
            tmp = 0;
        if (this->state->chipCfg.latchedInt)
            tmp |= MPU9150Consts::BIT_LATCH_EN | MPU9150Consts::BIT_ANY_RD_CLR;
        writeData[0] = this->state->reg->intPinCfg;
        writeData[1] = tmp;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, writeData , 2))
            return -1;
    }
    
    this->state->chipCfg.bypassMode = bypassOn;
    return 0;       
}

int InvMPU::MpuSelectDevice(int device)
{
    int result = 0;
    
    if ((device < 0) || (device >= MPU9150Consts::MPU_MAX_DEVICES))
    {
        result = -1;
    }
    else
    {
        this->deviceIndex = device;
        this->reg = this->regArray + device;
        this->deviceInfo = this->hwArray + device;
        this->selfTest = this->testArray + device;
        this->state = this->stateArray + device;
    }

    return result;
}
        
/* Configuration APIs */
int InvMPU::MpuLpAccelMode(unsigned char rate)
{
    char tmp[3];

    if (rate > 40)
        return -1;

    if (!rate) 
    {
        MpuSetIntLatched(0);
        tmp[0] = this->state->reg->pwrMgmt1;
        tmp[1] = 0;
        tmp[2] = MPU9150Consts::BIT_STBY_XYZG;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
            return -1;
            
        this->state->chipCfg.lpAccelMode = 0;
        return 0;
    }
    /* For LP accel, we automatically configure the hardware to produce latched
     * interrupts. In LP accel mode, the hardware cycles into sleep mode before
     * it gets a chance to deassert the interrupt pin; therefore, we shift this
     * responsibility over to the MCU.
     *
     * Any register read will clear the interrupt.
     */
    MpuSetIntLatched(1);
//#if defined MPU6050
    tmp[0] = this->state->reg->pwrMgmt1;
    tmp[1] = MPU9150Consts::BIT_LPA_CYCLE;
    if (rate == 1) 
    {
        tmp[2] = MPU9150Enums::INV_LPA_1_25HZ;
        MpuSetLpf(5);
    } 
    else if (rate <= 5) 
    {
        tmp[2] = MPU9150Enums::INV_LPA_5HZ;
        MpuSetLpf(5);
    } 
    else if (rate <= 20) 
    {
        tmp[2] = MPU9150Enums::INV_LPA_20HZ;
        MpuSetLpf(10);
    } 
    else 
    {
        tmp[2] = MPU9150Enums::INV_LPA_40HZ;
        MpuSetLpf(20);
    }
    
    tmp[2] = (tmp[2] << 6) | MPU9150Consts::BIT_STBY_XYZG;
    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
        return -1;
/*#elif defined MPU6500
    //Set wake frequency.
    if (rate == 1)
        tmp[0] = INV_LPA_1_25HZ;
    else if (rate == 2)
        tmp[0] = INV_LPA_2_5HZ;
    else if (rate <= 5)
        tmp[0] = INV_LPA_5HZ;
    else if (rate <= 10)
        tmp[0] = INV_LPA_10HZ;
    else if (rate <= 20)
        tmp[0] = INV_LPA_20HZ;
    else if (rate <= 40)
        tmp[0] = INV_LPA_40HZ;
    else if (rate <= 80)
        tmp[0] = INV_LPA_80HZ;
    else if (rate <= 160)
        tmp[0] = INV_LPA_160HZ;
    else if (rate <= 320)
        tmp[0] = INV_LPA_320HZ;
    else
        tmp[0] = INV_LPA_640HZ;
    if (i2c_write(st->hw->addr, st->reg->lp_accel_odr, 1, tmp))
        return -1;
    tmp[0] = BIT_LPA_CYCLE;
    if (i2c_write(st->hw->addr, st->reg->pwr_mgmt_1, 1, tmp))
        return -1;
#endif */
    this->state->chipCfg.sensors = MPU9150Consts::INV_XYZ_ACCEL;
    this->state->chipCfg.clkSrc = 0;
    this->state->chipCfg.lpAccelMode = 1;
    MpuConfigureFifo(0);

    return 0;  
}

int InvMPU::MpuLpMotionInterrupt(unsigned short thresh, unsigned char time, unsigned char lpaFreq)
{
     char data[3];

    if (lpaFreq) 
    {
        unsigned char threshHw;

//#if defined MPU6050
        /* TODO: Make these const/#defines. */
        /* 1LSb = 32mg. */
        if (thresh > 8160)
            threshHw = 255;
        else if (thresh < 32)
            threshHw = 1;
        else
            threshHw = thresh >> 5;
/*#elif defined MPU6500
       // 1LSb = 4mg. 
        if (thresh > 1020)
            thresh_hw = 255;
        else if (thresh < 4)
            thresh_hw = 1;
        else
            thresh_hw = thresh >> 2;
#endif*/

        if (!time)
            /* Minimum duration must be 1ms. */
            time = 1;

//#if defined MPU6050
        if (lpaFreq > 40)
//#elif defined MPU6500
//        if (lpa_freq > 640)
//#endif
            /* At this point, the chip has not been re-configured, so the
             * function can safely exit.
             */
            return -1;

        if (!this->state->chipCfg.intMotionOnly) 
        {
            /* Store current settings for later. */
            if (this->state->chipCfg.dmpOn) 
            {
                MpuSetDmpState(0);
                this->state->chipCfg.cache.dmpOn = 1;
            } 
            else
                this->state->chipCfg.cache.dmpOn = 0;
            
            MpuGetGyroFsr(&this->state->chipCfg.cache.gyroFsr);
            MpuGetAccelFsr(&this->state->chipCfg.cache.accelFsr);
            MpuGetLpf(&this->state->chipCfg.cache.lpf);
            MpuGetSampleRate(&this->state->chipCfg.cache.sampleRate);
            this->state->chipCfg.cache.sensorsOn = this->state->chipCfg.sensors;
            MpuGetFifoConfig(&this->state->chipCfg.cache.fifoSensors);
        }

//#ifdef MPU6050
        /* Disable hardware interrupts for now. */
        this->SetIntEnable(0);

        /* Enter full-power accel-only mode. */
        MpuLpAccelMode(0);

        /* Override current LPF (and HPF) settings to obtain a valid accel
         * reading.
         */
        data[0] = this->state->reg->lpf;
        data[1] = MPU9150Enums::INV_FILTER_256HZ_NOLPF2;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data, 2))
            return -1;

        /* NOTE: Digital high pass filter should be configured here. Since this
         * driver doesn't modify those bits anywhere, they should already be
         * cleared by default.
         */

        /* Configure the device to send motion interrupts. */
        /* Enable motion interrupt. */
        data[0] = this->state->reg->intEnable;
        data[1] = MPU9150Consts::BIT_MOT_INT_EN;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data, 2))
            goto lp_int_restore;

        /* Set motion interrupt parameters. */
        data[0] = this->state->reg->motionThr;
        data[1] = threshHw;
        data[2] = time;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data, 3))
            goto lp_int_restore;

        /* Force hardware to "lock" current accel sample. */
        wait_ms(5);
        data[0] = this->state->reg->accelCfg;
        data[1] = (this->state->chipCfg.accelFsr << 3) | MPU9150Consts::BITS_HPF;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data, 2))
            goto lp_int_restore;

        /* Set up LP accel mode. */
        data[0] = this->state->reg->pwrMgmt1;
        data[1] = MPU9150Consts::BIT_LPA_CYCLE;
        if (lpaFreq == 1)
            data[2] = MPU9150Enums::INV_LPA_1_25HZ;
        else if (lpaFreq <= 5)
            data[2] = MPU9150Enums::INV_LPA_5HZ;
        else if (lpaFreq <= 20)
            data[2] = MPU9150Enums::INV_LPA_20HZ;
        else
            data[2] = MPU9150Enums::INV_LPA_40HZ;
        data[2] = (data[2] << 6) | MPU9150Consts::BIT_STBY_XYZG;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data, 3))
            goto lp_int_restore;

       this->state->chipCfg.intMotionOnly = 1;
        return 0;
/*#elif defined MPU6500
        // Disable hardware interrupts. 
        set_int_enable(0);

        // Enter full-power accel-only mode, no FIFO/DMP.
        data[0] = 0;
        data[1] = 0;
        data[2] = BIT_STBY_XYZG;
        if (i2c_write(st->hw->addr, st->reg->user_ctrl, 3, data))
            goto lp_int_restore;

        // Set motion threshold. 
        data[0] = thresh_hw;
        if (i2c_write(st->hw->addr, st->reg->motion_thr, 1, data))
            goto lp_int_restore;

        // Set wake frequency. 
        if (lpa_freq == 1)
            data[0] = INV_LPA_1_25HZ;
        else if (lpa_freq == 2)
            data[0] = INV_LPA_2_5HZ;
        else if (lpa_freq <= 5)
            data[0] = INV_LPA_5HZ;
        else if (lpa_freq <= 10)
            data[0] = INV_LPA_10HZ;
        else if (lpa_freq <= 20)
            data[0] = INV_LPA_20HZ;
        else if (lpa_freq <= 40)
            data[0] = INV_LPA_40HZ;
        else if (lpa_freq <= 80)
            data[0] = INV_LPA_80HZ;
        else if (lpa_freq <= 160)
            data[0] = INV_LPA_160HZ;
        else if (lpa_freq <= 320)
            data[0] = INV_LPA_320HZ;
        else
            data[0] = INV_LPA_640HZ;
        if (i2c_write(st->hw->addr, st->reg->lp_accel_odr, 1, data))
            goto lp_int_restore;

        // Enable motion interrupt (MPU6500 version). 
        data[0] = BITS_WOM_EN;
        if (i2c_write(st->hw->addr, st->reg->accel_intel, 1, data))
            goto lp_int_restore;

        // Enable cycle mode. 
        data[0] = BIT_LPA_CYCLE;
        if (i2c_write(st->hw->addr, st->reg->pwr_mgmt_1, 1, data))
            goto lp_int_restore;

        // Enable interrupt. 
        data[0] = BIT_MOT_INT_EN;
        if (i2c_write(st->hw->addr, st->reg->int_enable, 1, data))
            goto lp_int_restore;

        st->chip_cfg.int_motion_only = 1;
        return 0;
#endif */
    } else {
        /* Don't "restore" the previous state if no state has been saved. */
        int ii;
        char *cache_ptr = (char*)&this->state->chipCfg.cache;
        for (ii = 0; ii < sizeof(this->state->chipCfg.cache); ii++) {
            if (cache_ptr[ii] != 0)
                goto lp_int_restore;
        }
        /* If we reach this point, motion interrupt mode hasn't been used yet. */
        return -1;
    }
lp_int_restore:
    /* Set to invalid values to ensure no I2C writes are skipped. */
    this->state->chipCfg.gyroFsr = 0xFF;
    this->state->chipCfg.accelFsr = 0xFF;
    this->state->chipCfg.lpf = 0xFF;
    this->state->chipCfg.sampleRate = 0xFFFF;
    this->state->chipCfg.sensors = 0xFF;
    this->state->chipCfg.fifoEnable = 0xFF;
    this->state->chipCfg.clkSrc = MPU9150Enums::INV_CLK_PLL;
    MpuSetSensors(this->state->chipCfg.cache.sensorsOn);
    MpuSetGyroFsr(this->state->chipCfg.cache.gyroFsr);
    MpuSetAccelFsr(this->state->chipCfg.cache.accelFsr);
    MpuSetLpf(this->state->chipCfg.cache.lpf);
    MpuSetSampleRate(this->state->chipCfg.cache.sampleRate);
    MpuConfigureFifo(this->state->chipCfg.cache.fifoSensors);

    if (this->state->chipCfg.cache.dmpOn)
        MpuSetDmpState(1);

//#ifdef MPU6500
//    /* Disable motion interrupt (MPU6500 version). */
 //   data[0] = 0;
//    if (i2c_write(st->hw->addr, st->reg->accel_intel, 1, data))
//        goto lp_int_restore;
//#endif

    this->state->chipCfg.intMotionOnly = 0;
    return 0;  
}

int InvMPU::MpuSetIntLevel(unsigned char activeLow)
{
    this->state->chipCfg.activeLowInt = activeLow;
    return 0;   
}

int InvMPU::MpuSetIntLatched(unsigned char enable)
{
    char tmp[2];
    
    if (this->state->chipCfg.latchedInt == enable)
        return 0;

    if (enable)
        tmp[1] = MPU9150Consts::BIT_LATCH_EN | MPU9150Consts::BIT_ANY_RD_CLR;
    else
        tmp[1] = 0;
    if (this->state->chipCfg.bypassMode)
        tmp[1] |= MPU9150Consts::BIT_BYPASS_EN;
    if (this->state->chipCfg.activeLowInt)
        tmp[1] |= MPU9150Consts::BIT_ACTL;
        
    tmp[0] =  this->state->reg->intPinCfg;    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
        return -1;
        
   this->state->chipCfg.latchedInt = enable;
    return 0;   
}

int InvMPU::MpuSetDmpState(unsigned char enable)
{
    char tmp[2];
    
    if (this->state->chipCfg.dmpOn == enable)
        return 0;

    if (enable) 
    {
        if (!this->state->chipCfg.dmpLoaded)
            return -1;
        /* Disable data ready interrupt. */
        this->SetIntEnable(0);
        /* Disable bypass mode. */
        MpuSetBypass(0);
        /* Keep constant sample rate, FIFO rate controlled by DMP. */
        MpuSetSampleRate(this->state->chipCfg.dmpSampleRate);
        /* Remove FIFO elements. */
        tmp[0] = 0x23;
        tmp[1] = 0;
        this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp));
        this->state->chipCfg.dmpOn = 1;
        /* Enable DMP interrupt. */
        this->SetIntEnable(1);
        MpuResetFifo();
    } 
    else 
    {
        /* Disable DMP interrupt. */
        this->SetIntEnable(0);
        /* Restore FIFO settings. */
        tmp[0] = 0x23;
        tmp[1] = this->state->chipCfg.fifoEnable;
        this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp));
        this->state->chipCfg.dmpOn = 0;
        MpuResetFifo();
    }
    return 0;  
}

int InvMPU::MpuGetDmpState(unsigned char *enabled)
{
    enabled[0] = this->state->chipCfg.dmpOn;
    return 0;
}

int InvMPU::MpuGetLpf(unsigned short *lpf)
{
    switch (this->state->chipCfg.lpf) 
    {
        case MPU9150Enums::INV_FILTER_188HZ:
            lpf[0] = 188;
            break;
        case MPU9150Enums::INV_FILTER_98HZ:
            lpf[0] = 98;
            break;
        case MPU9150Enums::INV_FILTER_42HZ:
            lpf[0] = 42;
            break;
        case MPU9150Enums::INV_FILTER_20HZ:
            lpf[0] = 20;
            break;
        case MPU9150Enums::INV_FILTER_10HZ:
            lpf[0] = 10;
            break;
        case MPU9150Enums::INV_FILTER_5HZ:
            lpf[0] = 5;
            break;
        case MPU9150Enums::INV_FILTER_256HZ_NOLPF2:
        case MPU9150Enums::INV_FILTER_2100HZ_NOLPF:
        default:
            lpf[0] = 0;
            break;
    }
    return 0; 
}

int InvMPU::MpuSetLpf(unsigned short lpf)
{
    char data[2];
    
    if (!(this->state->chipCfg.sensors))
        return -1;

    if (lpf >= 188)
        data[1] = MPU9150Enums::INV_FILTER_188HZ;
    else if (lpf >= 98)
        data[1] = MPU9150Enums::INV_FILTER_98HZ;
    else if (lpf >= 42)
        data[1] = MPU9150Enums::INV_FILTER_42HZ;
    else if (lpf >= 20)
        data[1] = MPU9150Enums::INV_FILTER_20HZ;
    else if (lpf >= 10)
        data[1] = MPU9150Enums::INV_FILTER_10HZ;
    else
        data[1] = MPU9150Enums::INV_FILTER_5HZ;

    if (this->state->chipCfg.lpf == data[1])
        return 0;
        
    data[0] = this->state->reg->lpf;   
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
    this->state->chipCfg.lpf = data[1];
    return 0;   
}
   
int InvMPU::MpuGetGyroFsr(unsigned short *fsr)
{
    switch (this->state->chipCfg.gyroFsr) 
    {
        case MPU9150Enums::INV_FSR_250DPS:
            fsr[0] = 250;
            break;
        case MPU9150Enums::INV_FSR_500DPS:
            fsr[0] = 500;
            break;
        case MPU9150Enums::INV_FSR_1000DPS:
            fsr[0] = 1000;
            break;
        case MPU9150Enums::INV_FSR_2000DPS:
            fsr[0] = 2000;
            break;
        default:
            fsr[0] = 0;
            break;
    }
    
    return 0;  
}

int InvMPU::MpuSetGyroFsr(unsigned short fsr)
{
    char data[2];

    if (!(this->state->chipCfg.sensors))
        return -1;

    switch (fsr) 
    {
        case 250:
            data[1] = MPU9150Enums::INV_FSR_250DPS << 3;
            break;
        case 500:
            data[1] = MPU9150Enums::INV_FSR_500DPS << 3;
            break;
        case 1000:
            data[1] = MPU9150Enums::INV_FSR_1000DPS << 3;
            break;
        case 2000:
            data[1] = MPU9150Enums::INV_FSR_2000DPS << 3;
            break;
        default:
            return -1;
    }

    if (this->state->chipCfg.gyroFsr == (data[1] >> 3))
        return 0;
        
    data[0] = this->state->reg->gyroCfg;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
        
    this->state->chipCfg.gyroFsr = data[1] >> 3;
    return 0;   
}
  
int InvMPU::MpuGetAccelFsr(unsigned char *fsr)
{
     switch (this->state->chipCfg.accelFsr) 
     {
        case MPU9150Enums::INV_FSR_2G:
            fsr[0] = 2;
            break;
        case MPU9150Enums::INV_FSR_4G:
            fsr[0] = 4;
            break;
        case MPU9150Enums::INV_FSR_8G:
            fsr[0] = 8;
            break;
        case MPU9150Enums::INV_FSR_16G:
            fsr[0] = 16;
            break;
        default:
            return -1;
    }
    
    if (this->state->chipCfg.accelHalf)
        fsr[0] <<= 1;
        
    return 0;  
}

int InvMPU::MpuSetAccelFsr(unsigned char fsr)
{
    char data[2];

    if (!(this->state->chipCfg.sensors))
        return -1;

    switch (fsr) {
    case 2:
        data[1] = MPU9150Enums::INV_FSR_2G << 3;
        break;
    case 4:
        data[1] = MPU9150Enums::INV_FSR_4G << 3;
        break;
    case 8:
        data[1] = MPU9150Enums::INV_FSR_8G << 3;
        break;
    case 16:
        data[1] = MPU9150Enums::INV_FSR_16G << 3;
        break;
    default:
        return -1;
    }

    if (this->state->chipCfg.accelFsr == (data[1] >> 3))
        return 0;
    
    data[0] =  this->state->reg->accelCfg;   
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
        
    this->state->chipCfg.accelFsr = data[1] >> 3;
    return 0;   
}
  
int InvMPU::MpuGetCompassFsr(unsigned short *fsr)
{
    fsr[0] = this->state->hw->compassFsr;
    return 0; 
}
  
int InvMPU::MpuGetGyroSens(float *sens)
{
    switch (this->state->chipCfg.gyroFsr) 
    {
        case MPU9150Enums::INV_FSR_250DPS:
            sens[0] = 131.f;
            break;
        case MPU9150Enums::INV_FSR_500DPS:
            sens[0] = 65.5f;
            break;
        case MPU9150Enums::INV_FSR_1000DPS:
            sens[0] = 32.8f;
            break;
        case MPU9150Enums::INV_FSR_2000DPS:
            sens[0] = 16.4f;
            break;
        default:
            return -1;
    }
    
    return 0;    
}

int InvMPU::MpuGetAccelSens(unsigned short *sens)
{
    switch (this->state->chipCfg.accelFsr) 
    {
        case MPU9150Enums::INV_FSR_2G:
            sens[0] = 16384;
            break;
        case MPU9150Enums::INV_FSR_4G:
            sens[0] = 8092;
            break;
        case MPU9150Enums::INV_FSR_8G:
            sens[0] = 4096;
            break;
        case MPU9150Enums::INV_FSR_16G:
            sens[0] = 2048;
            break;
        default:
            return -1;
    }
    
    if (this->state->chipCfg.accelHalf)
        sens[0] >>= 1;
    return 0;
}
     
int InvMPU::MpuGetSampleRate(unsigned short *rate)
{
     if (this->state->chipCfg.dmpOn)
        return -1;
    else
        rate[0] = this->state->chipCfg.sampleRate;
    return 0; 
}

int InvMPU::MpuSetSampleRate(unsigned short rate)
{
    char data[2];

    if (!(this->state->chipCfg.sensors))
        return -1;

    if (this->state->chipCfg.dmpOn)
        return -1;
    else 
    {
        if (this->state->chipCfg.lpAccelMode) 
        {
            if (rate && (rate <= 40)) 
            {
                /* Just stay in low-power accel mode. */
                MpuLpAccelMode(rate);
                return 0;
            }
            
            /* Requested rate exceeds the allowed frequencies in LP accel mode,
             * switch back to full-power mode.
             */
            MpuLpAccelMode(0);
        }
        
        if (rate < 4)
            rate = 4;
        else if (rate > 1000)
            rate = 1000;

        data[1] = 1000 / rate - 1;
        data[0] = this->state->reg->rateDiv;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;

        this->state->chipCfg.sampleRate = 1000 / (1 + data[1]);

//#ifdef AK89xx_SECONDARY
       MpuSetCompassSampleRate(Range::Min(this->state->chipCfg.compassSampleRate, MPU9150Consts::MAX_COMPASS_SAMPLE_RATE));
//#endif

        /* Automatically set LPF to 1/2 sampling rate. */
        MpuSetLpf(this->state->chipCfg.sampleRate >> 1);
        return 0;
    } 
}

int InvMPU::MpuGetCompassSampleRate(unsigned short *rate)
{
    rate[0] = this->state->chipCfg.compassSampleRate;
    return 0;   
}

int InvMPU::MpuSetCompassSampleRate(unsigned short rate)
{
    char div[2];
    if (!rate || rate > this->state->chipCfg.sampleRate || rate > MPU9150Consts::MAX_COMPASS_SAMPLE_RATE)
        return -1;

    div[1] = this->state->chipCfg.sampleRate / rate - 1;
    
    div[0] = this->state->reg->s4Ctrl;
    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, div , sizeof(div)))
        return -1;
        
    this->state->chipCfg.compassSampleRate = this->state->chipCfg.sampleRate / (div[1] + 1);
    return 0; 
}
     
int InvMPU::MpuGetFifoConfig(unsigned char *sensors)
{
    sensors[0] = this->state->chipCfg.fifoEnable;
    return 0;   
}

int InvMPU::MpuConfigureFifo(unsigned char sensors)
{
    char prev[2];
    int result = 0;

    /* Compass data isn't going into the FIFO. Stop trying. */
    sensors &= ~MPU9150Consts::INV_XYZ_COMPASS;

    if (this->state->chipCfg.dmpOn)
        return 0;
    else 
    {
        if (!(this->state->chipCfg.sensors))
            return -1;
            
        prev[1] = this->state->chipCfg.fifoEnable;
        this->state->chipCfg.fifoEnable = sensors & this->state->chipCfg.sensors;
        
        if (this->state->chipCfg.fifoEnable != sensors)
            /* You're not getting what you asked for. Some sensors are
             * asleep.
             */
            result = -1;
        else
            result = 0;
            
        if (sensors || this->state->chipCfg.lpAccelMode)
            this->SetIntEnable(1);
        else
            this->SetIntEnable(0);
            
        if (sensors) 
        {
            if (this->MpuResetFifo()) 
            {
                this->state->chipCfg.fifoEnable = prev[1];
                return -1;
            }
        }
    }

    return result;  
}
     
int InvMPU::MpuGetPowerState(unsigned char *powerOn)
{
     if (this->state->chipCfg.sensors)
        powerOn[0] = 1;
    else
        powerOn[0] = 0;
    return 0;   
}

int InvMPU::MpuSetSensors(unsigned char sensors)
{
    char data[2];
//#ifdef AK89xx_SECONDARY
    char userCtrl[2];
//#endif

    if (sensors & MPU9150Consts::INV_XYZ_GYRO)
        data[1] = MPU9150Enums::INV_CLK_PLL;
    else if (sensors)
        data[1] = 0;
    else
        data[1] = MPU9150Consts::BIT_SLEEP;
        
        data[0] = this->state->reg->pwrMgmt1;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data))) 
    {
        this->state->chipCfg.sensors = 0;
        return -1;
    }
    
    this->state->chipCfg.clkSrc = data[1] & ~MPU9150Consts::BIT_SLEEP;

    data[1] = 0;
    
    if (!(sensors & MPU9150Consts::INV_X_GYRO))
        data[1] |= MPU9150Consts::BIT_STBY_XG;
    if (!(sensors & MPU9150Consts::INV_Y_GYRO))
        data[1] |= MPU9150Consts::BIT_STBY_YG;
    if (!(sensors & MPU9150Consts::INV_Z_GYRO))
        data[1] |= MPU9150Consts::BIT_STBY_ZG;
    if (!(sensors & MPU9150Consts::INV_XYZ_ACCEL))
        data[1] |= MPU9150Consts::BIT_STBY_XYZA;
    
    data[0] = this->state->reg->pwrMgmt2;    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data))) 
    {
        this->state->chipCfg.sensors = 0;
        return -1;
    }

    if (sensors && (sensors != MPU9150Consts::INV_XYZ_ACCEL))
       this->MpuSetIntLatched(0);

//#ifdef AK89xx_SECONDARY
//#ifdef AK89xx_BYPASS
 //   if (sensors & INV_XYZ_COMPASS)
 //       mpu_set_bypass(1);
 //   else
//        mpu_set_bypass(0);
//#else

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->userCtrl,&userCtrl[1],1))
        return -1;
        
    /* Handle AKM power management. */
    if (sensors & MPU9150Consts::INV_XYZ_COMPASS) 
    {
        data[1] = MPU9150Consts::AKM_SINGLE_MEASUREMENT;
        userCtrl[1] |= MPU9150Consts::BIT_AUX_IF_EN;
    } 
    else 
    {
        data[1] = MPU9150Consts::AKM_POWER_DOWN;
        userCtrl[1] &= ~MPU9150Consts::BIT_AUX_IF_EN;
    }
    
    if (this->state->chipCfg.dmpOn)
        userCtrl[1] |= MPU9150Consts::BIT_DMP_EN;
    else
        userCtrl[1] &= ~MPU9150Consts::BIT_DMP_EN;
        
    data[0] = state->reg->s1Do; 
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
        
    /* Enable/disable I2C master mode. */
    userCtrl[0] = state->reg->userCtrl;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, userCtrl , sizeof(userCtrl)))
        return -1;
        
//#endif
//#endif

    this->state->chipCfg.sensors = sensors;
    this->state->chipCfg.lpAccelMode = 0;
    wait_ms(50);
    return 0;  
}
    
int InvMPU::MpuSetAccelBias(const long *accelBias)
{
    char data[7];
    short accelHw[3];
    short gotAccel[3];
    short fg[3];

    if (!accelBias)
        return -1;
        
    if (!accelBias[0] && !accelBias[1] && !accelBias[2])
        return 0;

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, 3,data,3))
        return -1;
        
    fg[0] = ((data[0] >> 4) + 8) & 0xf;
    fg[1] = ((data[1] >> 4) + 8) & 0xf;
    fg[2] = ((data[2] >> 4) + 8) & 0xf;

    accelHw[0] = (short)(accelBias[0] * 2 / (64 + fg[0]));
    accelHw[1] = (short)(accelBias[1] * 2 / (64 + fg[1]));
    accelHw[2] = (short)(accelBias[2] * 2 / (64 + fg[2]));

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, 0x06,data,6))
        return -1;

    gotAccel[0] = ((short)data[0] << 8) | data[1];
    gotAccel[1] = ((short)data[2] << 8) | data[3];
    gotAccel[2] = ((short)data[4] << 8) | data[5];

    accelHw[0] += gotAccel[0];
    accelHw[1] += gotAccel[1];
    accelHw[2] += gotAccel[2];

    data[0] = 0x06;
    data[1] = (accelHw[0] >> 8) & 0xff;
    data[2] = (accelHw[0]) & 0xff;
    data[3] = (accelHw[1] >> 8) & 0xff;
    data[4] = (accelHw[1]) & 0xff;
    data[5] = (accelHw[2] >> 8) & 0xff;
    data[6] = (accelHw[2]) & 0xff;

    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
    return 0;      
}

        
/* Data getter/setter APIs */
int InvMPU::MpuGetGyroReg(short *data, unsigned long *timestamp)
{
    char tmp[6];

    if (!(this->state->chipCfg.sensors & MPU9150Consts::INV_XYZ_GYRO))
        return -1;

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->rawGyro,tmp,6))
        return -1;
        
    data[0] = (tmp[0] << 8) | tmp[1];
    data[1] = (tmp[2] << 8) | tmp[3];
    data[2] = (tmp[4] << 8) | tmp[5];
    
    if (timestamp)
        *timestamp = this->systemTimer->Read_Ms();// get_ms(timestamp);
        
    return 0;   
}

int InvMPU::MpuGetAccelReg(short *data, unsigned long *timestamp)
{
    char tmp[6];

    if (!(this->state->chipCfg.sensors & MPU9150Consts::INV_XYZ_ACCEL))
        return -1;

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->rawAccel,tmp,6))
        return -1;
        
    data[0] = (tmp[0] << 8) | tmp[1];
    data[1] = (tmp[2] << 8) | tmp[3];
    data[2] = (tmp[4] << 8) | tmp[5];
    
    if (timestamp)
        *timestamp = this->systemTimer->Read_Ms();// get_ms(timestamp);
        
    return 0;   
}

int InvMPU::MpuGetCompassReg(short *data, unsigned long *timestamp)
{
  //  #ifdef AK89xx_SECONDARY
    char tmp[9];

    if (!(this->state->chipCfg.sensors & MPU9150Consts::INV_XYZ_COMPASS))
        return -1;

//#ifdef AK89xx_BYPASS
//    if (i2c_read(st->chip_cfg.compass_addr, AKM_REG_ST1, 8, tmp))
//        return -2;
//    tmp[8] = AKM_SINGLE_MEASUREMENT;
//    if (i2c_write(st->chip_cfg.compass_addr, AKM_REG_CNTL, 1, tmp+8))
//        return -3;
//#else
    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->rawCompass,tmp,8))
        return -4;
//#endif

//#if defined AK8975_SECONDARY
    /* AK8975 doesn't have the overrun error bit. */
    if (!(tmp[0] & MPU9150Consts::AKM_DATA_READY))
        return -5;
        
    if ((tmp[7] & MPU9150Consts::AKM_OVERFLOW) || (tmp[7] & MPU9150Consts::AKM_DATA_ERROR))
        return -6;
        
//#elif defined AK8963_SECONDARY
//    /* AK8963 doesn't have the data read error bit. */
//    if (!(tmp[0] & AKM_DATA_READY) || (tmp[0] & AKM_DATA_OVERRUN))
//        return -7;
//   if (tmp[7] & AKM_OVERFLOW)
//        return -8;
//#endif
    data[0] = ((unsigned short)tmp[2] << 8) | (unsigned short)tmp[1];
    data[1] = ((unsigned short)tmp[4] << 8) | (unsigned short)tmp[3];
    data[2] = ((unsigned short)tmp[6] << 8) | (unsigned short)tmp[5];

    data[0] = ((long)data[0] * this->state->chipCfg.magSensAdj[0]) >> 8;
    data[1] = ((long)data[1] * this->state->chipCfg.magSensAdj[1]) >> 8;
    data[2] = ((long)data[2] * this->state->chipCfg.magSensAdj[2]) >> 8;

    if (timestamp)
         *timestamp = this->systemTimer->Read_Ms();// get_ms(timestamp);
         
    return 0;
//#else
//    return -9;
//#endif   
}

int InvMPU::MpuGetTemperature(long *data, unsigned long *timestamp)
{
    char tmp[2];
    short raw;

    if (!(this->state->chipCfg.sensors))
        return -1;

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->temp,tmp,2))
        return -1;
        
    raw = (tmp[0] << 8) | tmp[1];
    
    if (timestamp)
        *timestamp = this->systemTimer->Read_Ms();// get_ms(timestamp);

    data[0] = (long)((35 + ((raw - (float)state->hw->tempOffset) / state->hw->tempSens)) * 65536L);
    
    return 0;   
}

int InvMPU::MpuGetIntStatus(short *status)
{
    char tmp[2];
    
    if (!this->state->chipCfg.sensors)
        return -1;
        
    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->dmpIntStatus,tmp,2))
        return -1;
        
    status[0] = (tmp[0] << 8) | tmp[1];
    
    return 0;   
}

int InvMPU::MpuReadFifo(short *gyro, short *accel, unsigned long *timestamp, unsigned char *sensors, unsigned char *more)
{
    /* Assumes maximum packet size is gyro (6) + accel (6). */
    char data[MPU9150Consts::MAX_PACKET_LENGTH];
    unsigned char packetSize = 0;
    unsigned short fifoCount, index = 0;

    if (this->state->chipCfg.dmpOn)
        return -1;

    sensors[0] = 0;
    if (!this->state->chipCfg.sensors)
        return -2;
        
    if (!this->state->chipCfg.fifoEnable)
        return -3;

    if (this->state->chipCfg.fifoEnable & MPU9150Consts::INV_X_GYRO)
        packetSize += 2;
        
    if (this->state->chipCfg.fifoEnable & MPU9150Consts::INV_Y_GYRO)
        packetSize += 2;
        
    if (this->state->chipCfg.fifoEnable & MPU9150Consts::INV_Z_GYRO)
        packetSize += 2;
        
    if (this->state->chipCfg.fifoEnable & MPU9150Consts::INV_XYZ_ACCEL)
        packetSize += 6;

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->fifoCountH,data,2))
        return -4;
        
    fifoCount = (data[0] << 8) | data[1];
        
    if (fifoCount < packetSize)
        return 1;
        
//    log_i("FIFO count: %hd\n", fifo_count);
    if (fifoCount > (this->state->hw->maxFifo >> 1)) 
    {
        /* FIFO is 50% full, better check overflow bit. */
        if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->intStatus,data,1))
            return -5;
            
        if (data[0] & MPU9150Consts::BIT_FIFO_OVERFLOW) 
        {
            this->MpuResetFifo();
            return 2;
        }
    }
    
    if (timestamp)
        *timestamp = this->systemTimer->Read_Ms();// get_ms(timestamp);

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->fifoRW,data,packetSize))
        return -7;

    more[0] = fifoCount / packetSize - 1;
    sensors[0] = 0;

    if ((index != packetSize) && this->state->chipCfg.fifoEnable & MPU9150Consts::INV_XYZ_ACCEL) 
    {
        accel[0] = (data[index+0] << 8) | data[index+1];
        accel[1] = (data[index+2] << 8) | data[index+3];
        accel[2] = (data[index+4] << 8) | data[index+5];
        sensors[0] |= MPU9150Consts::INV_XYZ_ACCEL;
        index += 6;
    }
    
    if ((index != packetSize) && this->state->chipCfg.fifoEnable & MPU9150Consts::INV_X_GYRO) 
    {
        gyro[0] = (data[index+0] << 8) | data[index+1];
        sensors[0] |= MPU9150Consts::INV_X_GYRO;
        index += 2;
    }
    
    if ((index != packetSize) && this->state->chipCfg.fifoEnable & MPU9150Consts::INV_Y_GYRO) 
    {
        gyro[1] = (data[index+0] << 8) | data[index+1];
        sensors[0] |= MPU9150Consts::INV_Y_GYRO;
        index += 2;
    }
    
    if ((index != packetSize) && this->state->chipCfg.fifoEnable & MPU9150Consts::INV_Z_GYRO) 
    {
        gyro[2] = (data[index+0] << 8) | data[index+1];
        sensors[0] |= MPU9150Consts::INV_Z_GYRO;
        index += 2;
    }

    return 0;   
}

int InvMPU::MpuReadFifoStream(unsigned short length, char *data, unsigned char *more)
{
     char tmp[2];
     unsigned short fifoCount;
     
    if (!this->state->chipCfg.dmpOn)
        return -1;
        
    if (!this->state->chipCfg.sensors)
        return -2;

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->fifoCountH,tmp,1))
        return -3;
        
    fifoCount = (tmp[0] << 8) | tmp[1];
    
    if (fifoCount < length) 
    {
        more[0] = 0;
        return 1;
    }
    
    if (fifoCount > (this->state->hw->maxFifo >> 1)) 
    {
        /* FIFO is 50% full, better check overflow bit. */
        if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->intStatus,tmp,1))
            return -5;
            
        if (tmp[0] & MPU9150Consts::BIT_FIFO_OVERFLOW) 
        {
            this->MpuResetFifo();
            return 2;
        }
    }

    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->fifoRW,data,length))
        return -7;
        
    more[0] = fifoCount / length - 1;
    
    return 0;  
}

int InvMPU::MpuResetFifo(void)
{
    char data[2];

    if (!(this->state->chipCfg.sensors))
        return -1;
        
    data[0] = this->state->reg->intEnable;
    data[1] = 0;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
    
    data[0] = this->state->reg->fifoEn;    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;
    
    data[0] = this->state->reg->userCtrl;    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
        return -1;

    if (this->state->chipCfg.dmpOn) 
    {
        data[0] = this->state->reg->userCtrl;
        data[1] = MPU9150Consts::BIT_FIFO_RST | MPU9150Consts::BIT_DMP_RST;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
            
        wait_ms(50);
        
        
        data[1] = MPU9150Consts::BIT_DMP_EN | MPU9150Consts::BIT_FIFO_EN;
        if (this->state->chipCfg.sensors & MPU9150Consts::INV_XYZ_COMPASS)
            data[1] |= MPU9150Consts::BIT_AUX_IF_EN;
        
        data[0] = this->state->reg->userCtrl;    
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
            
        if (this->state->chipCfg.intEnable)
            data[1] = MPU9150Consts::BIT_DMP_INT_EN;
        else
            data[1] = 0;
        
        data[0] = this->state->reg->intEnable;    
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
        
        data[0] = this->state->reg->fifoEn;    
        data[1] = 0;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
    } 
    else 
    {
        data[0] = this->state->reg->userCtrl; 
        data[1] = MPU9150Consts::BIT_FIFO_RST;
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
            
        if (this->state->chipCfg.bypassMode || !(this->state->chipCfg.sensors & MPU9150Consts::INV_XYZ_COMPASS))
            data[1] = MPU9150Consts::BIT_FIFO_EN;
        else
            data[1] = MPU9150Consts::BIT_FIFO_EN | MPU9150Consts::BIT_AUX_IF_EN;
        
        data[0] = this->state->reg->userCtrl;     
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
            
        wait_ms(50);
        
        if (this->state->chipCfg.intEnable)
            data[1] = MPU9150Consts::BIT_DATA_RDY_EN;
        else
            data[1] = 0;
        
        data[0] = this->state->reg->intEnable;    
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
        
        data[0] = this->state->reg->fifoEn; 
        data[1] = this->state->chipCfg.fifoEnable;
        
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , sizeof(data)))
            return -1;
    }
    
    return 0;  
}

int InvMPU::MpuWriteMem(unsigned short memAddr, unsigned short length, unsigned char *data)
{
    char tmp[3];

    if (!data)
        return -1;
        
    if (!this->state->chipCfg.sensors)
        return -2;

    tmp[1] = (char)(memAddr >> 8);
    tmp[2] = (char)(memAddr & 0xFF);

    /* Check bank boundaries. */
    if (tmp[2] + length > this->state->hw->bankSize)
        return -3;

    tmp[0] = this->state->reg->bankSel;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
        return -4;
    
    char dataWrite[length+1];
    
    dataWrite[0] = this->state->reg->memRW;
    memcpy(&dataWrite[1],(char*)data,length);
        
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, dataWrite , sizeof(dataWrite)))
        return -5;
        
    return 0;   
}

int InvMPU::MpuReadMem(unsigned short memAddr, unsigned short length, unsigned char *data)
{
    char tmp[3];

    if (!data)
        return -1;
        
    if (!this->state->chipCfg.sensors)
        return -1;

    tmp[1] = (char)(memAddr >> 8);
    tmp[2] = (char)(memAddr & 0xFF);

    /* Check bank boundaries. */
    if (tmp[2] + length > this->state->hw->bankSize)
        return -1;

    tmp[0] = this->state->reg->bankSel;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
        return -1;
        
    if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, state->reg->memRW,(char*)data,length))
        return -1;
    return 0;   
}

int InvMPU::MpuLoadFirmware(unsigned short length, const unsigned char *firmware, unsigned short startAddr, unsigned short sampleRate)
{
    unsigned short ii;
    unsigned short thisWrite;
    char cur[LOADCHUNK], tmp[3];

    if (this->state->chipCfg.dmpLoaded)
        /* DMP should only be loaded once. */
        return -1;

    if (!firmware)
        return -1;
        
    for (ii = 0; ii < length; ii += thisWrite) 
    {
        thisWrite = Range::Min(LOADCHUNK, length - ii);
        
        if (this->MpuWriteMem(ii, thisWrite, (unsigned char*)&firmware[ii]))
            return -1;
            
        if (this->MpuReadMem(ii, thisWrite, (unsigned char*)cur))
            return -1;
            
        if (memcmp(firmware+ii, cur, thisWrite))
            return -2;
    }

    /* Set program start address. */
    tmp[0] = this->state->reg->prgmStartH;
    tmp[1] = startAddr >> 8;
    tmp[2] = startAddr & 0xFF;
    
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
        return -1;

    this->state->chipCfg.dmpLoaded = 1;
    this->state->chipCfg.dmpSampleRate = sampleRate;
    
    return 0;   
}

int InvMPU::MpuRegDump(void)
{
    unsigned char ii;
    char data;

    for (ii = 0; ii < this->state->hw->numReg; ii++) 
    {
        if (ii == this->state->reg->fifoRW || ii == this->state->reg->memRW)
            continue;
            
        if (this->busCtrl->ReadFromBus(busUser,state->hw->addr, ii,&data,1))
            return -1;
        
        if (this->debugLogger != NULL)
        {
            char debugMsg[40];
            memset(debugMsg,0,sizeof(debugMsg));
            sprintf(debugMsg,"%#5x: %#5x\r\n", ii, data);
            this->debugLogger->Log(debugMsg);
        }
    }
    
    return 0;   
}

int InvMPU::MpuReadReg(unsigned char reg, unsigned char *data)
{
    if (reg == state->reg->fifoRW || reg == state->reg->memRW)
        return -1;
    if (reg >= state->hw->numReg)
        return -1;
        
    return this->busCtrl->ReadFromBus(busUser,state->hw->addr, reg,(char*)data,1); 
}

int InvMPU::SetIntEnable (unsigned char enable)
{
   char tmp[2];

    if (this->state->chipCfg.dmpOn) 
    {
        if (enable)
            tmp[1] = MPU9150Consts::BIT_DMP_INT_EN;
        else
            tmp[1] = 0x00;
            
        tmp[0] = this->state->reg->intEnable;    
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
            return -1;
        this->state->chipCfg.intEnable = tmp[1];
    } 
    else 
    {
        if (!this->state->chipCfg.sensors)
            return -1;
        if (enable && this->state->chipCfg.intEnable)
            return 0;
        if (enable)
            tmp[1] = MPU9150Consts::BIT_DATA_RDY_EN;
        else
            tmp[1] = 0x00;
        
        tmp[0] = this->state->reg->intEnable; 
        if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, tmp , sizeof(tmp)))
            return -1;
        this->state->chipCfg.intEnable = tmp[1];
    }
    
    return 0; 
}

int InvMPU::SetupCompass(void)
{
  //  #ifdef AK89xx_SECONDARY
    char data[5], akm_addr;

    this->MpuSetBypass(1);

    /* Find compass. Possible addresses range from 0x0C to 0x0F. */
    for (akm_addr = 0x0C; akm_addr <= 0x0F; akm_addr++) 
    {
        int result;
        result = this->busCtrl->ReadFromBus(busUser,(int)(akm_addr<<1), MPU9150Consts::AKM_REG_WHOAMI,&data[0],1); 
        //i2c_read(akm_addr<<1, AKM_REG_WHOAMI, 1, data);
        if (!result && (data[0] == MPU9150Consts::AKM_WHOAMI))
            break;
    }

    if (akm_addr > 0x0F) 
    {
        this->debugLogger->Log("Compass not found.\0");
        return -1;
    }
    else
    {
        char cmpID[20];
        
        sprintf(cmpID,"Compass I2C address %x\r\n\0", akm_addr);
        this->debugLogger->Log(cmpID);
    }

    this->state->chipCfg.compassAddr = akm_addr<<1;

    data[0] = MPU9150Consts::AKM_REG_CNTL;
    data[1] = MPU9150Consts::AKM_POWER_DOWN;
    if (this->busCtrl->WriteToBus(busUser, this->state->chipCfg.compassAddr, data , 2))
        return -2;
    wait_ms(1);

    data[0] = MPU9150Consts::AKM_REG_CNTL;
    data[1] = MPU9150Consts::AKM_FUSE_ROM_ACCESS;
    if (this->busCtrl->WriteToBus(busUser, this->state->chipCfg.compassAddr, data , 2))
        return -3;
    wait_ms(1);

    /* Get sensitivity adjustment data from fuse ROM. */
    if (this->busCtrl->ReadFromBus(busUser,this->state->chipCfg.compassAddr, MPU9150Consts::AKM_REG_ASAX,data,3)) 
        return -4;
        
    this->state->chipCfg.magSensAdj[0] = (long)data[0] + 128;
    this->state->chipCfg.magSensAdj[1] = (long)data[1] + 128;
    this->state->chipCfg.magSensAdj[2] = (long)data[2] + 128;
//#ifdef MPU_DEBUG
//    Serial.print("Compass sens: "); Serial.print(st->chip_cfg.mag_sens_adj[0]); Serial.print(" ");
//    Serial.print(st->chip_cfg.mag_sens_adj[1]); Serial.print(" ");
//    Serial.print(st->chip_cfg.mag_sens_adj[2]); Serial.println();
//#endif

    data[0] = MPU9150Consts::AKM_REG_CNTL;
    data[1] = MPU9150Consts::AKM_POWER_DOWN;
    if (this->busCtrl->WriteToBus(busUser, this->state->chipCfg.compassAddr, data , 2))
        return -5;
    wait_ms(1);

    this->MpuSetBypass(0);

    /* Set up master mode, master clock, and ES bit. */
    data[0] = this->state->reg->i2cMst;
    data[1] = 0x40;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -6;

    /* Slave 0 reads from AKM data registers. */
    data[0] = this->state->reg->s0Addr;
    data[1] = MPU9150Consts::BIT_I2C_READ | this->state->chipCfg.compassAddr;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -7;

    /* Compass reads start at this register. */
    data[0] = this->state->reg->s0Reg;
    data[1] = MPU9150Consts::AKM_REG_ST1;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -8;

    /* Enable slave 0, 8-byte reads. */
    data[0] = this->state->reg->s0Ctrl;
    data[1] = MPU9150Consts::BIT_SLAVE_EN | 8;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -9;

    /* Slave 1 changes AKM measurement mode. */
    data[0] = this->state->reg->s1Addr;
    data[1] = this->state->chipCfg.compassAddr;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -10;

    /* AKM measurement mode register. */
    data[0] = this->state->reg->s1Reg;
    data[1] = MPU9150Consts::AKM_REG_CNTL;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -11;

    /* Enable slave 1, 1-byte writes. */
    data[0] = this->state->reg->s1Ctrl;
    data[1] = MPU9150Consts::BIT_SLAVE_EN | 1;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -12;

    /* Set slave 1 data. */
    data[0] = this->state->reg->s1Do;
    data[1] = MPU9150Consts::AKM_SINGLE_MEASUREMENT;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -13;

    /* Trigger slave 0 and slave 1 actions at each sample. */
    data[0] = this->state->reg->i2cDelayCtrl;
    data[1] = 0x03;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -14;

//#ifdef MPU9150
    /* For the MPU9150, the auxiliary I2C bus needs to be set to VDD. */
    data[0] = this->state->reg->ygOffsTc;
    data[1] = MPU9150Consts::BIT_I2C_MST_VDDIO;
    if (this->busCtrl->WriteToBus(busUser, this->state->hw->addr, data , 2))
        return -15;
//#endif

    return 0;
//#else
//    return -16;
//#endif
    
}