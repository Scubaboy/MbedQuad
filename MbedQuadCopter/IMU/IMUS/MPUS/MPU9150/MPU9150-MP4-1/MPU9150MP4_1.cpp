#include "MPU9150MP4_1.h"
#include "MPU9150Registers.h"
#include "DMPHexMotionDriver4_1.h"
#include "BusUsers.h"

const BusCtrlUsers::BusUsers busUser = BusCtrlUsers::IMU;
const int DeviceI2CAddrs = 0x68<<1;

MPU9150MP4_1::MPU9150MP4_1(I2CBusCtrl* busCtrl, DebugLoggerBase* debugLogger)
{
    this->busCtrl = busCtrl;
    this->debugLogger = debugLogger;
}

uint8_t MPU9150MP4_1::getDeviceID(){
    uint8_t ret = 0;
    readBits(MPU9150Registers::MPU9150_RA_WHO_AM_I, MPU9150Registers::MPU9150_WHO_AM_I_BIT, MPU9150Registers::MPU9150_WHO_AM_I_LENGTH, &ret);
    
    return ret;
}

bool MPU9150MP4_1::isReady(){
    return (getDeviceID() == (DeviceI2CAddrs >> 2));
}

void MPU9150MP4_1::DecodeFiFoPacket(char* buffer)
{
    //FIFO Quad data.
    this->quatRaw.w = (float)((((int32_t)buffer[0] << 24) + ((int32_t)buffer[1] << 16) + ((int32_t)buffer[2] << 8) + buffer[3]))* (1.0 / (1<<30));
    this->quatRaw.vect.x = (float)((((int32_t)buffer[4] << 24) + ((int32_t)buffer[5] << 16) + ((int32_t)buffer[6] << 8) + buffer[7]))* (1.0 / (1<<30));
    this->quatRaw.vect.y = (float)((((int32_t)buffer[8] << 24) + ((int32_t)buffer[9] << 16) + ((int32_t)buffer[10] << 8) + buffer[11]))* (1.0 / (1<<30));
    this->quatRaw.vect.z = (float)((((int32_t)buffer[12] << 24) + ((int32_t)buffer[13] << 16) + ((int32_t)buffer[14] << 8) + buffer[15]))* (1.0 / (1<<30));
    
    //FIFO Gyro data Raw.
    this->gyroRaw.x = (float)((((int32_t)buffer[16] << 24) + ((int32_t)buffer[17] << 16) + ((int32_t)buffer[18] << 8) + (int32_t)buffer[19])>>16);
    this->gyroRaw.y = (float)((((int32_t)buffer[20] << 24) + ((int32_t)buffer[21] << 16) + ((int32_t)buffer[22] << 8) + (int32_t)buffer[23])>>16);
    this->gyroRaw.z = (float)((((int32_t)buffer[24] << 24) + ((int32_t)buffer[25] << 16) + ((int32_t)buffer[26] << 8) + (int32_t)buffer[27])>>16);
    
    //Gyro scaled data
    this->gyroScaled.x = this->gyroRaw.x*250.0f/32768.0f;
    this->gyroScaled.y = this->gyroRaw.y*250.0f/32768.0f;
    this->gyroScaled.z = this->gyroRaw.z*250.0f/32768.0f;
    
    //FIFO Accel Data Raw.
    this->accelRaw.x = (float)((((int32_t)buffer[34] << 24) + ((int32_t)buffer[35] << 16) + ((int32_t)buffer[36] << 8) + (int32_t)buffer[37])>>16);
    this->accelRaw.y = (float)((((int32_t)buffer[38] << 24) + ((int32_t)buffer[39] << 16) + ((int32_t)buffer[40] << 8) + (int32_t)buffer[41])>>16);
    this->accelRaw.z = (float)((((int32_t)buffer[42] << 24) + ((int32_t)buffer[43] << 16) + ((int32_t)buffer[44] << 8) + (int32_t)buffer[45])>>16);
    
    //Accel scaled data.
    this->accelScaled.x = this->accelRaw.x*2.0f/32768.0f;
    this->accelScaled.y = this->accelRaw.y*2.0f/32768.0f;
    this->accelScaled.z = this->accelRaw.z*2.0f/32768.0f;

    //FIFO Mag Data Raw.
    this->magRaw.x = (float)(buffer[29] << 8) + buffer[28];
    this->magRaw.y = (float)(buffer[31] << 8) + buffer[30];
    this->magRaw.z = (float)(buffer[33] << 8) + buffer[32];
    
    //Mag scaled data
    this->magScaled.x = this->magRaw.x * 10.0f*1229.0f/4096.0f;
    this->magScaled.y = this->magRaw.y * 10.0f*1229.0f/4096.0f;
    this->magScaled.z = this->magRaw.z * 10.0f*1229.0f/4096.0f;
}

void MPU9150MP4_1::GetUpdate(Vector3f* gyroRaw, 
                               Vector3f* gyroScaled, 
                               Vector3f* accelRaw, 
                               Vector3f* accelScaled,
                               Vector3f* magRaw,
                               Vector3f* magScaled,
                               Vector3f* eulerAngles,
                               Quaternion::Quat* quat)
{
    if(this->getFifoCount() >= 48)
    {
        char buffer[200];
        this->getFifoBuffer(buffer,  48);
    
    
        this->DecodeFiFoPacket(buffer);
        Vector3f vect= Quaternion::getEulerAngles(this->quatRaw);
        
        gyroRaw->x = this->gyroRaw.x;
        gyroRaw->y = this->gyroRaw.y;
        gyroRaw->z = this->gyroRaw.z;
        gyroScaled->x = this->gyroScaled.x;
        gyroScaled->y = this->gyroScaled.y;
        gyroScaled->z = this->gyroScaled.z;
        
        accelRaw->x = this->accelRaw.x;
        accelRaw->y = this->accelRaw.y;
        accelRaw->z = this->accelRaw.z;
        accelScaled->x = this->accelScaled.x;
        accelScaled->y = this->accelScaled.y;
        accelScaled->z = this->accelScaled.z;
        
        magRaw->x = this->magRaw.x;
        magRaw->y = this->magRaw.y;
        magRaw->z = this->magRaw.z;
        magScaled->x = this->magScaled.x;
        magScaled->y = this->magScaled.y;
        magScaled->z = this->magScaled.z;
        
        quat->w = this->quatRaw.w;
        quat->vect.x = this->quatRaw.vect.x;
        quat->vect.y = this->quatRaw.vect.y;
        quat->vect.z = this->quatRaw.vect.z;
        
        eulerAngles->x = vect.x;
        eulerAngles->y = vect.y;
        eulerAngles->z = vect.z;
     
    }
    /*
    do 
    {
        ret = this->DmpReadFifo(this->gyro,NULL,this->quat._l,NULL,&this->sensors,&this->fifoCount);
     will return:
            0 - if ok
            1 - no packet available
            2 - if BIT_FIFO_OVERFLOWN is set
            3 - if frame corrupted
               <0 - if error
        
    } while (fifoCount<0);// && ret == 0);
    
    if (ret == 0)
    {
        this->quat._f.w = (float)this->quat._l[0] / (float)QUAT_SENS;
        this->quat._f.x = (float)this->quat._l[1] / (float)QUAT_SENS;
        this->quat._f.y = (float)this->quat._l[2] / (float)QUAT_SENS;
        this->quat._f.z = (float)this->quat._l[3] / (float)QUAT_SENS;
        
        Quaternion::QuaternionToEuler(&this->quat._f, roll, pitch, yaw);
    }*/
}

void MPU9150MP4_1::Init()
{
    if(this->isReady())
    {
        this->debugLogger->Log("MPU9150 is ready\r\n");
    } 
    else 
    {
         this->debugLogger->Log("MPU9150 initialisation failure\r\n");
    }
    
    this->initialiseDMP();
    this->setFifoReset(true);    
    this->setDMPEnabled(true);
}

void MPU9150MP4_1::initialise(){
    reset();
    wait_ms(20);//wait for reset
    
    sleep(false);
    clockSelect(MPU9150Registers::MPU9150_CLOCK_PLL_XGYRO); //use the gyro clock as its more reliable
    setGyroFullScaleRange(MPU9150Registers::MPU9150_GYRO_FS_250);
    setAccelFullScaleRange(MPU9150Registers::MPU9150_ACCEL_FS_2);   
    setStandbyAccX(true);
    setI2CMasterClock(MPU9150Registers::MPU9150_CLOCK_DIV_400);
    setDigitalLowPassFilter(MPU9150Registers::MPU9150_DLPF_BW_42);
    setSampleRateDivider(4);
    
    initialiseMagnetometer();
    
    setFifoReset(true);
    
    setTemperatureFifo(true);
    setAccelFifo(true);
    setGyroFifo(true);
    setSlave0Fifo(true);
    
    setInterruptDataReadyEnable(true);   
    setEnableFifo(true);
}

void MPU9150MP4_1::initialiseMagnetometer(){
    //set up slave 0 to read the magnetometor data
    setWaitForExternalSensor(true);
    //read data
    setI2cSlaveRW(0, true);
    setI2cSlaveAddress(0, 0x0C);
    setI2cSlaveRegister(0, 3);
    setI2cSlaveEnable(0, true);   
    setI2cSlaveTransactionLength(0, 6);

    
    //set up slave 1 to request a new magnetometor reading by writing 0x01 to 0xA
    setI2cSlaveAddress(1, 0x0C);
    setI2cSlaveRegister(1, 0x0A);
    setI2cSlaveTransactionLength(1, 1);
    setI2cSlaveEnable(1, true);
    setI2cSlaveDataOut(1, 1); 
    
    //configure update rates
    setI2cMasterDelay(4);    
    setI2cSlaveDelay(0, true);
    setI2cSlaveDelay(1, true);
    
    //Enable the aux i2c bus with MPU9150MP4_1 as master
    setI2cMasterEnable(true); 
}

void MPU9150MP4_1::initialiseDMP(){
    reset();
    wait_ms(20);
    sleep(false);

    setMemoryBank(0x10, true, true);
    setMemoryStartAddress(0x06);
//    debug.printf("Hardware Version: %d\r\n", readMemoryByte());

    setMemoryBank(0);
    // check OTP bank valid
    uint8_t otpValid = getOTPBankValid();
//    debug.printf("optValid: %d\r\n", otpValid);
    
    //Enabling interrupt latch, clear on any read, AUX bypass enabled
    write(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, 0x32);
    
    if (writeMemoryBlock(DMPHexMotionDriver4_1::dmpMemory, DMPHexMotionDriver4_1::MPU9150_DMP_CODE_SIZE, 0 ,0, true)) {
 //       debug.printf("Success! DMP code written and verified.\r\n");
        if (writeDMPConfigurationSet(DMPHexMotionDriver4_1::dmpConfig, DMPHexMotionDriver4_1::MPU9150_DMP_CONFIG_SIZE)) {
//            debug.printf("Success! DMP configuration written and verified.\r\n");
            setIntDMPEnabled(true);
            setInterruptFifoOverflowEnable(true);
            setSampleRateDivider(4);
            clockSelect(MPU9150Registers::MPU9150_CLOCK_PLL_XGYRO);
            setDigitalLowPassFilter(MPU9150Registers::MPU9150_DLPF_BW_42);
            setGyroFullScaleRange(MPU9150Registers::MPU9150_GYRO_FS_2000);
            setAccelFullScaleRange(MPU9150Registers::MPU9150_ACCEL_FS_2);  
            setExternalFrameSync(MPU9150Registers::MPU9150_EXT_SYNC_TEMP_OUT_L);
            setDMPConfig1(0x03);
            setDMPConfig2(0x00);
            
            unsigned char *update_ptr = (unsigned char*)DMPHexMotionDriver4_1::dmpUpdates;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            
            setFifoReset(true);
            
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            
            write(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, 0x00);
            setInterruptAnyReadClear(true);
            setInterruptLatch(true);
            
            setI2cSlaveRW(0, true);
            setI2cSlaveAddress(0, 0x0C);
            setI2cSlaveRegister(0, 1);
            setI2cSlaveEnable(0, true);   
            setI2cSlaveTransactionLength(0, 10);
        
            //set up slave 1 to request a new magnetometor reading by writing 0x01 to 0xA
            setI2cSlaveAddress(2, 0x0C);
            setI2cSlaveRegister(2, 0x0A);
            setI2cSlaveTransactionLength(2, 1);
            setI2cSlaveEnable(2, true);
            setI2cSlaveDataOut(2, 1); 
            
            //configure update rates
            setI2cMasterDelay(4);    
            setI2cSlaveDelay(0, true);
            setI2cSlaveDelay(2, true);
            
            //Enable the aux i2c bus with MPU9150MP4_1 as master
            setI2cMasterEnable(true);
            
            write(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, 0x00);
            
            // enable I2C master mode and reset DMP/FIFO
            //DEBUG_PRINTLN(F("Enabling I2C master mode..."));
            write( MPU9150Registers::MPU9150_RA_USER_CTRL, 0x20);
            //DEBUG_PRINTLN(F("Resetting FIFO..."));
            write(MPU9150Registers::MPU9150_RA_USER_CTRL, 0x24);
            //DEBUG_PRINTLN(F("Rewriting I2C master mode enabled because...I don't know"));
            write(MPU9150Registers::MPU9150_RA_USER_CTRL, 0x20);
            //DEBUG_PRINTLN(F("Enabling and resetting DMP/FIFO..."));
            write(MPU9150Registers::MPU9150_RA_USER_CTRL, 0xE8);
            
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);

            //read?
            update_ptr += update_ptr[2] + 3;
            //stalls?
            //readMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1]);            


            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            
            int fifoCount = 0;
            while ((fifoCount = getFifoCount()) < 46);
            uint8_t buffer[128];
            getFifoBuffer((char *)buffer, fifoCount);
            getInterruptStatus();            
            
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            
            fifoCount = 0;
            while ((fifoCount = getFifoCount()) < 48);
            getFifoBuffer((char *)buffer, fifoCount);
            getInterruptStatus();
            fifoCount = 0;
            while ((fifoCount = getFifoCount()) < 48);
            getFifoBuffer((char *)buffer, fifoCount);
            getInterruptStatus();   
            
            update_ptr += update_ptr[2] + 3;
            writeMemoryBlock(update_ptr + 3, update_ptr[2], update_ptr[0], update_ptr[1], true);
            
            setDMPEnabled(false);
            
 //           debug.printf("finished\r\n");

        }
    }
    
    
}

//PWR_MGMT_1 Control Register
//*****************************/
void MPU9150MP4_1::reset(){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_1, MPU9150Registers::MPU9150_PWR1_DEVICE_RESET_BIT, true);
}

void MPU9150MP4_1::sleep(bool state){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_1, MPU9150Registers::MPU9150_PWR1_SLEEP_BIT, state);
}

/*
cycle between sleep mode and waking up to take a single sample of data from 
active sensors at a rate determined by LP_WAKE_CTRL (register 108). 
*/
void MPU9150MP4_1::cycleMode(bool state){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_1, MPU9150Registers::MPU9150_PWR1_CYCLE_BIT, state);
}
void MPU9150MP4_1::disableTemperatureSensor(bool state){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_1, MPU9150Registers::MPU9150_PWR1_TEMP_DIS_BIT, state);
}
void MPU9150MP4_1::clockSelect(uint8_t clk){
    writeBits(MPU9150Registers::MPU9150_RA_PWR_MGMT_1, MPU9150Registers::MPU9150_PWR1_CLKSEL_BIT, MPU9150Registers::MPU9150_PWR1_CLKSEL_LENGTH, clk);
}

//PWR_MGMT_2 Control Register
//*****************************/
void MPU9150MP4_1::setCycleWakeFrequency(uint8_t freq){
    writeBits(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_LP_WAKE_CTRL_BIT, MPU9150Registers::MPU9150_PWR2_LP_WAKE_CTRL_LENGTH, freq);    
}
void MPU9150MP4_1::setStandbyAccX(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_STBY_XA_BIT, value);
}
void MPU9150MP4_1::setStandbyAccY(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_STBY_YA_BIT, value);
}
void MPU9150MP4_1::setStandbyAccZ(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_STBY_ZA_BIT, value);
}
void MPU9150MP4_1::setStandbyGyroX( bool value){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_STBY_XG_BIT, value);
}
void MPU9150MP4_1::setStandbyGyroY( bool value){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_STBY_YG_BIT, value);
}
void MPU9150MP4_1::setStandbyGyroZ( bool value){
    writeBit(MPU9150Registers::MPU9150_RA_PWR_MGMT_2, MPU9150Registers::MPU9150_PWR2_STBY_ZG_BIT, value);
}

//SMPRT_DIV  Sample Rate Divider 
//*****************************/
void MPU9150MP4_1::setSampleRateDivider(uint8_t value){
    write(MPU9150Registers::MPU9150_RA_SMPLRT_DIV, value);
}

//CONFIG
void MPU9150MP4_1::setExternalFrameSync(uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_CONFIG, MPU9150Registers::MPU9150_CFG_EXT_SYNC_SET_BIT, MPU9150Registers::MPU9150_CFG_EXT_SYNC_SET_LENGTH, value);    
}
void MPU9150MP4_1::setDigitalLowPassFilter(uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_CONFIG, MPU9150Registers::MPU9150_CFG_DLPF_CFG_BIT, MPU9150Registers::MPU9150_CFG_DLPF_CFG_LENGTH, value);    
}

//GYRO_CONFIG
void MPU9150MP4_1::setGyroSelfTest(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_GYRO_CONFIG, 7, value); //X
    writeBit(MPU9150Registers::MPU9150_RA_GYRO_CONFIG, 6, value); //Y
    writeBit(MPU9150Registers::MPU9150_RA_GYRO_CONFIG, 5, value); //Z
}

void MPU9150MP4_1::setGyroFullScaleRange(uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_GYRO_CONFIG, MPU9150Registers::MPU9150_GCONFIG_FS_SEL_BIT, MPU9150Registers::MPU9150_GCONFIG_FS_SEL_LENGTH, value);
}

//ACCEL_CONFIG
void MPU9150MP4_1::setAccelSelfTest(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_ACCEL_CONFIG, MPU9150Registers::MPU9150_ACONFIG_XA_ST_BIT, value);
    writeBit(MPU9150Registers::MPU9150_RA_ACCEL_CONFIG, MPU9150Registers::MPU9150_ACONFIG_YA_ST_BIT, value);
    writeBit(MPU9150Registers::MPU9150_RA_ACCEL_CONFIG, MPU9150Registers::MPU9150_ACONFIG_ZA_ST_BIT, value);
}
void MPU9150MP4_1::setAccelFullScaleRange(uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_ACCEL_CONFIG, MPU9150Registers::MPU9150_ACONFIG_AFS_SEL_BIT , MPU9150Registers::MPU9150_ACONFIG_AFS_SEL_LENGTH, value);
}

//FIFO_EN
void MPU9150MP4_1::setTemperatureFifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_TEMP_FIFO_EN_BIT, value);
}
void MPU9150MP4_1::setGyroFifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_XG_FIFO_EN_BIT, value);
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_YG_FIFO_EN_BIT, value);
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_ZG_FIFO_EN_BIT, value);
}
void MPU9150MP4_1::setAccelFifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_ACCEL_FIFO_EN_BIT, value);
}
void MPU9150MP4_1::setSlave2Fifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_SLV2_FIFO_EN_BIT, value);
}
void MPU9150MP4_1::setSlave1Fifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_SLV1_FIFO_EN_BIT, value);
}
void MPU9150MP4_1::setSlave0Fifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_FIFO_EN, MPU9150Registers::MPU9150_SLV0_FIFO_EN_BIT, value);
}

//I2C_MST_CTRL
void MPU9150MP4_1::setMultiMaster(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_MST_CTRL, MPU9150Registers::MPU9150_MULT_MST_EN_BIT, value);
}
void MPU9150MP4_1::setWaitForExternalSensor(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_MST_CTRL, MPU9150Registers::MPU9150_WAIT_FOR_ES_BIT, value);
}
void MPU9150MP4_1::setSlave3Fifo(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_MST_CTRL, MPU9150Registers::MPU9150_SLV_3_FIFO_EN_BIT, value);
}
void MPU9150MP4_1::setMasterStartStop(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_MST_CTRL, MPU9150Registers::MPU9150_I2C_MST_P_NSR_BIT, value);
}
void MPU9150MP4_1::setI2CMasterClock(uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_I2C_MST_CTRL, MPU9150Registers::MPU9150_I2C_MST_CLK_BIT, MPU9150Registers::MPU9150_I2C_MST_CLK_LENGTH, value);
}

//I2C slaves 0 to 3
//I2C_SLV0_ADDR
void MPU9150MP4_1::setI2cSlaveRW(uint8_t slave_id, bool value){
    if(slave_id > 3)return;
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV0_ADDR + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_RW_BIT, value);    
}
void MPU9150MP4_1::setI2cSlaveAddress(uint8_t slave_id, uint8_t value){
    if(slave_id > 3)return;
    writeBits(MPU9150Registers::MPU9150_RA_I2C_SLV0_ADDR + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_ADDR_BIT, MPU9150Registers::MPU9150_I2C_SLV_ADDR_LENGTH, value);
}
//I2C_SLV0_REG,
void MPU9150MP4_1::setI2cSlaveRegister(uint8_t slave_id, uint8_t value){
    if(slave_id > 3)return;
    write(MPU9150Registers::MPU9150_RA_I2C_SLV0_REG + (slave_id * 3), value);
}
//I2C_SLV0_CTRL
void MPU9150MP4_1::setI2cSlaveEnable(uint8_t slave_id, bool value){
    if(slave_id > 3)return;
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV0_CTRL + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_EN_BIT, value);     
}
void MPU9150MP4_1::setI2cSlaveByteSwap(uint8_t slave_id, bool value){
    if(slave_id > 3)return;
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV0_CTRL + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_BYTE_SW_BIT, value);   
}
void MPU9150MP4_1::setI2cSlaveRegDisable(uint8_t slave_id, bool value){
    if(slave_id > 3)return;
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV0_CTRL + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_REG_DIS_BIT, value);   
}
void MPU9150MP4_1::setI2cSlaveByteGrouping(uint8_t slave_id, bool value){
    if(slave_id > 3)return;
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV0_CTRL + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_GRP_BIT, value);   
}
void MPU9150MP4_1::setI2cSlaveTransactionLength(uint8_t slave_id, uint8_t value){
    if(slave_id > 3)return;
    writeBits(MPU9150Registers::MPU9150_RA_I2C_SLV0_CTRL + (slave_id * 3), MPU9150Registers::MPU9150_I2C_SLV_LEN_BIT, MPU9150Registers::MPU9150_I2C_SLV_LEN_LENGTH, value);
}
//I2C_SLV0_DO
void MPU9150MP4_1::setI2cSlaveDataOut(uint8_t slave_id, uint8_t value){
    if(slave_id > 3)return;
    write(MPU9150Registers::MPU9150_RA_I2C_SLV0_DO + slave_id, value);
}
//I2C_MST_DELAY_CTRL 
void MPU9150MP4_1::setI2cSlaveDelay(uint8_t slave_id, uint8_t value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_MST_DELAY_CTRL, slave_id, value);    
}
void MPU9150MP4_1::setI2cSlaveShadowDelay(uint8_t value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_MST_DELAY_CTRL, 7, value);    
}

//I2C slave4
//I2C_SLV4_ADDR
void MPU9150MP4_1::setI2cSlave4RW( bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV4_ADDR, MPU9150Registers::MPU9150_I2C_SLV4_RW_BIT, value);    
}
void MPU9150MP4_1::setI2cSlave4Address( uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_I2C_SLV4_ADDR, MPU9150Registers::MPU9150_I2C_SLV4_ADDR_BIT, MPU9150Registers::MPU9150_I2C_SLV4_ADDR_LENGTH, value);
}
//I2C_SLV4_REG,
void MPU9150MP4_1::setI2cSlave4Register(uint8_t value){
    write(MPU9150Registers::MPU9150_RA_I2C_SLV4_REG, value);
}
//I2C_SLV4_DO
void MPU9150MP4_1::setI2cSlave4DataOut(uint8_t value){
    write(MPU9150Registers::MPU9150_RA_I2C_SLV4_DO, value);
}

//I2C_SLV4_CTRL
void MPU9150MP4_1::setI2cSlave4Enable(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV4_CTRL, MPU9150Registers::MPU9150_I2C_SLV4_EN_BIT, value);     
}

void MPU9150MP4_1::setI2cSlave4IntEnable(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV4_CTRL, MPU9150Registers::MPU9150_I2C_SLV4_INT_EN_BIT, value);  
}

void MPU9150MP4_1::setI2cSlave4RegDisable(bool value){
    writeBit(MPU9150Registers::MPU9150_RA_I2C_SLV4_CTRL, MPU9150Registers::MPU9150_I2C_SLV4_REG_DIS_BIT, value);
}

void MPU9150MP4_1::setI2cMasterDelay(uint8_t value){
    writeBits(MPU9150Registers::MPU9150_RA_I2C_SLV4_CTRL, MPU9150Registers::MPU9150_I2C_SLV4_MST_DLY_BIT, MPU9150Registers::MPU9150_I2C_SLV4_MST_DLY_LENGTH, value);
}

uint8_t MPU9150MP4_1::getI2cSlave4Di(){
    return get8(MPU9150Registers::MPU9150_RA_I2C_SLV4_DI);
}

//I2C_MST_STATUS
bool MPU9150MP4_1::setI2cPassthrough()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_PASS_THROUGH_BIT);
}

bool MPU9150MP4_1::setI2cSlave4Done()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_SLV4_DONE_BIT);
}

bool MPU9150MP4_1::setI2cLostArbitration()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_LOST_ARB_BIT);
}

bool MPU9150MP4_1::setI2cSlave0Nack()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_SLV0_NACK_BIT);
}

bool MPU9150MP4_1::setI2cSlave1Nack()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_SLV1_NACK_BIT);
}

bool MPU9150MP4_1::setI2cSlave2Nack()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_SLV2_NACK_BIT);
}

bool MPU9150MP4_1::setI2cSlave3Nack()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_SLV3_NACK_BIT);
}

bool MPU9150MP4_1::setI2cSlave4Nack()
{
   return this->getBit(MPU9150Registers::MPU9150_RA_I2C_MST_STATUS, MPU9150Registers::MPU9150_MST_I2C_SLV4_NACK_BIT); 
}

//INT_PIN_CFG
void MPU9150MP4_1::setInterruptActiveLow(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_INT_LEVEL_BIT, value);
}

void MPU9150MP4_1::setInterruptOpenDrain(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_INT_OPEN_BIT, value);
}

void MPU9150MP4_1::setInterruptLatch(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_LATCH_INT_EN_BIT, value);
}

void MPU9150MP4_1::setInterruptAnyReadClear(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_INT_RD_CLEAR_BIT, value);
}

void MPU9150MP4_1::setFsyncInterruptActiveLow(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_FSYNC_INT_LEVEL_BIT, value);
}

void MPU9150MP4_1::setFsyncInterruptEnable(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_FSYNC_INT_EN_BIT, value);
}

void MPU9150MP4_1::setI2cAuxBypassEnable(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_PIN_CFG, MPU9150Registers::MPU9150_INTCFG_I2C_BYPASS_EN_BIT, value);
}

//INT_ENABLE
void MPU9150MP4_1::setInterruptFifoOverflowEnable(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_ENABLE, MPU9150Registers::MPU9150_INTERRUPT_FIFO_OFLOW_BIT, value);
}

void MPU9150MP4_1::setInterruptMasterEnable(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_ENABLE, MPU9150Registers::MPU9150_INTERRUPT_I2C_MST_INT_BIT, value);
}

void MPU9150MP4_1::setInterruptDataReadyEnable(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_INT_ENABLE, MPU9150Registers::MPU9150_INTERRUPT_DATA_RDY_BIT, value);
}

//INT_STATUS
bool MPU9150MP4_1::getInterruptFifoOverflow()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_INT_STATUS, MPU9150Registers::MPU9150_INTERRUPT_FIFO_OFLOW_BIT);
}

bool MPU9150MP4_1::getInterruptMaster()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_INT_STATUS, MPU9150Registers::MPU9150_INTERRUPT_I2C_MST_INT_BIT);
}

bool MPU9150MP4_1::getInterruptDataReady()
{
    return this->getBit(MPU9150Registers::MPU9150_RA_INT_STATUS, MPU9150Registers::MPU9150_INTERRUPT_DATA_RDY_BIT);
}

uint8_t MPU9150MP4_1::getInterruptStatus()
{
    return this->get8(MPU9150Registers::MPU9150_RA_INT_STATUS);    
}

//SIGNAL_PATH_RESET
void MPU9150MP4_1::resetGyroSignalPath()
{
    this->writeBit(MPU9150Registers::MPU9150_RA_SIGNAL_PATH_RESET, MPU9150Registers::MPU9150_PATHRESET_GYRO_RESET_BIT, true);
}

void MPU9150MP4_1::resetAccelSignalPath()
{
    this->writeBit(MPU9150Registers::MPU9150_RA_SIGNAL_PATH_RESET, MPU9150Registers::MPU9150_PATHRESET_ACCEL_RESET_BIT, true);    
}

void MPU9150MP4_1::resetTempSignalPath()
{
    this->writeBit(MPU9150Registers::MPU9150_RA_SIGNAL_PATH_RESET, MPU9150Registers::MPU9150_PATHRESET_TEMP_RESET_BIT, true);    
}

//USER_CTRL 
void MPU9150MP4_1::setEnableFifo(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_FIFO_EN_BIT, value);    
}

void MPU9150MP4_1::setI2cMasterEnable(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_I2C_MST_EN_BIT, value);       
}

void MPU9150MP4_1::setFifoReset(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_FIFO_RESET_BIT, value);       
}

void MPU9150MP4_1::setI2cMasterReset(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_I2C_MST_RESET_BIT, value);       
}

void MPU9150MP4_1::setFullSensorReset(bool value)
{
    this->writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_SIG_COND_RESET_BIT, value);       
}

//FIFO_COUNT_H and FIFO_COUNT_L
int16_t MPU9150MP4_1::getFifoCount()
{
    return this->get16(MPU9150Registers::MPU9150_RA_FIFO_COUNTH);
}

//FIFO_R_W
bool MPU9150MP4_1::getFifoBuffer(char* buffer, int16_t length)
{
    return this->read(MPU9150Registers::MPU9150_RA_FIFO_R_W, buffer, length);
}

//UNDOCUMENTED (again reimplemention from sparkfun github) can't find any origional documentation
// XG_OFFS_TC
uint8_t MPU9150MP4_1::getOTPBankValid() 
{
    return this->getBit(MPU9150Registers::MPU9150_RA_XG_OFFS_TC, MPU9150Registers::MPU9150_TC_OTP_BNK_VLD_BIT);
}

//INT_ENABLE 
void MPU9150MP4_1::setIntPLLReadyEnabled(bool value) 
{
    this->writeBit( MPU9150Registers::MPU9150_RA_INT_ENABLE, MPU9150Registers::MPU9150_INTERRUPT_PLL_RDY_INT_BIT, value);
}
void MPU9150MP4_1::setIntDMPEnabled(bool value) 
{
    this->writeBit( MPU9150Registers::MPU9150_RA_INT_ENABLE, MPU9150Registers::MPU9150_INTERRUPT_DMP_INT_BIT, value);
}

// INT_STATUS
bool MPU9150MP4_1::getIntPLLReadyStatus() 
{
    return this->getBit( MPU9150Registers::MPU9150_RA_INT_STATUS, MPU9150Registers::MPU9150_INTERRUPT_PLL_RDY_INT_BIT);
}

bool MPU9150MP4_1::getIntDMPStatus() 
{
    return this->getBit( MPU9150Registers::MPU9150_RA_INT_STATUS, MPU9150Registers::MPU9150_INTERRUPT_DMP_INT_BIT);
}

// USER_CTRL
bool MPU9150MP4_1::getDMPEnabled() 
{
    return this->getBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_DMP_EN_BIT);
}

void MPU9150MP4_1::setDMPEnabled(bool value) 
{
    this->writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_DMP_EN_BIT, value);
}

void MPU9150MP4_1::resetDMP() {
    writeBit(MPU9150Registers::MPU9150_RA_USER_CTRL, MPU9150Registers::MPU9150_USERCTRL_DMP_RESET_BIT, true);
}

// BANK_SEL
void MPU9150MP4_1::setMemoryBank(uint8_t bank, bool prefetchEnabled, bool userBank) 
{
    bank &= 0x1F;
    
    if (userBank)
    {
        bank |= 0x20;
    }
    
    if (prefetchEnabled)
    {
        bank |= 0x40;
    }
    
    this->write( MPU9150Registers::MPU9150_RA_BANK_SEL, bank);
}

// MEM_START_ADDR
void MPU9150MP4_1::setMemoryStartAddress(uint8_t address) 
{
    write(MPU9150Registers::MPU9150_RA_MEM_START_ADDR, address);
}

// MEM_R_W
uint8_t MPU9150MP4_1::readMemoryByte() 
{
    return get8(MPU9150Registers::MPU9150_RA_MEM_R_W);
}

void MPU9150MP4_1::writeMemoryByte(uint8_t value) 
{
    write(MPU9150Registers::MPU9150_RA_MEM_R_W, value);
}

void MPU9150MP4_1::readMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address) 
{    
    uint8_t chunkSize;
    
    setMemoryBank(bank);
    setMemoryStartAddress(address);
       
    for (uint16_t i = 0; i < dataSize;) 
    {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU9150Registers::MPU9150_DMP_MEMORY_CHUNK_SIZE;
    
        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) 
        {
            chunkSize = dataSize - i;
        }
    
        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) 
        {
            chunkSize = 256 - address;
        }
        
        //debug.printf("reading %d", chunkSize);
        // read the chunk of data as specified
        read(MPU9150Registers::MPU9150_RA_MEM_R_W, (char*)(data+i), chunkSize);
        //debug.printf("read");
        // increase byte index by [chunkSize]
        i += chunkSize;
    
        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;
    
        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) 
        {
            if (address == 0) 
            {
                bank++;
            }
            
            setMemoryBank(bank);
            setMemoryStartAddress(address);
        }
    }
}

bool MPU9150MP4_1::writeMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify) 
{
    uint8_t chunkSize;
    uint8_t *verifyBuffer = 0;
    uint8_t *progBuffer = 0;
    uint16_t i;
    
    setMemoryBank(bank);
    setMemoryStartAddress(address);
       
    if (verify) 
    {
        verifyBuffer = (uint8_t *)malloc(MPU9150Registers::MPU9150_DMP_MEMORY_CHUNK_SIZE);
    }
    
    for (i = 0; i < dataSize;) 
    {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU9150Registers::MPU9150_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) 
        {
            chunkSize = dataSize - i;
        }

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) 
        {
            chunkSize = 256 - address;
        }
        
        progBuffer = (uint8_t *)data + i;

        write(MPU9150Registers::MPU9150_RA_MEM_R_W, (char*)progBuffer, chunkSize);
        
       
        // verify data if needed
        if (verify && verifyBuffer) 
        {
            setMemoryBank(bank);
            setMemoryStartAddress(address);
            read(MPU9150Registers::MPU9150_RA_MEM_R_W, (char*)verifyBuffer, chunkSize);
            
            if (memcmp(progBuffer, verifyBuffer, chunkSize) != 0) 
            {
                free(verifyBuffer);
                //debug.printf("invalid(%d, %d)\r\n", bank, read_errors, write_errors);
                return false; // uh oh.
            }
        }

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) 
        {
            if (address == 0) bank++;
            setMemoryBank(bank);
            setMemoryStartAddress(address);
        }
    }
    
    if (verify)
    { 
        free(verifyBuffer);
    }
    
    return true;
}

bool MPU9150MP4_1::writeDMPConfigurationSet(const uint8_t *data, uint16_t dataSize) 
{
    uint8_t *progBuffer;
    uint8_t success, special;
    uint16_t i;

    // config set data is a long string of blocks with the following structure:
    // [bank] [offset] [length] [byte[0], byte[1], ..., byte[length]]
    uint8_t bank, offset, length;
    
    for (i = 0; i < dataSize;) 
    {
        bank = data[i++];
        offset = data[i++];
        length = data[i++];

        // write data or perform special action
        if (length > 0) 
        {
            progBuffer = (uint8_t *)data + i;
            success = writeMemoryBlock(progBuffer, length, bank, offset, true);
            i += length;
        } 
        else 
        {
            // special instruction
            // NOTE: this kind of behavior (what and when to do certain things)
            // is totally undocumented. This code is in here based on observed
            // behavior only, and exactly why (or even whether) it has to be here
            // is anybody's guess for now.
            special = data[i++];

            if (special == 0x01) 
            {
                // enable DMP-related interrupts
                //setIntZeroMotionEnabled(true);
                //setIntFIFOBufferOverflowEnabled(true);
                //setIntDMPEnabled(true);
                write(MPU9150Registers::MPU9150_RA_INT_ENABLE, 0x32);  // single operation
                success = true;
            } 
            else 
            {
                // unknown special command
                success = false;
            }
        }
        
        if (!success) 
        {
            return false;
        }
    }
    
    return true;
}
// DMP_CFG_1
uint8_t MPU9150MP4_1::getDMPConfig1() 
{
   return get8(MPU9150Registers::MPU9150_RA_DMP_CFG_1);

}
void MPU9150MP4_1::setDMPConfig1(uint8_t config) 
{
    write(MPU9150Registers::MPU9150_RA_DMP_CFG_1, config);
}

// DMP_CFG_2
uint8_t MPU9150MP4_1::getDMPConfig2() 
{
    return get8(MPU9150Registers::MPU9150_RA_DMP_CFG_2);

}
void MPU9150MP4_1::setDMPConfig2(uint8_t config) 
{
    write(MPU9150Registers::MPU9150_RA_DMP_CFG_2, config);
}

//Utility Functions
bool MPU9150MP4_1::getBit(char reg_addr, uint8_t bit)
{
    uint8_t data = 0;
    readBit(reg_addr, bit, &data);
    return (bool)data;
}

int8_t MPU9150MP4_1::get8(char reg_addr)
{
    char data;
    read(reg_addr, &data);
    return data;
}
    
int16_t MPU9150MP4_1::get16(char reg_addr)
{
    char data[2];
    read(reg_addr, data, 2);
    return (data[0]<<8) + data[1];
}

int16_t MPU9150MP4_1::get16L(char reg_addr)
{
    char data[2];
    read(reg_addr, data, 2);
    return (data[1]<<8) + data[0];
}

bool MPU9150MP4_1::write(char reg_addr, char data)
{
   return write(reg_addr, &data, 1);
}

bool MPU9150MP4_1::write(char reg_addr, char* data, int length)
{
    bool result = true;
    char dataSend[length+1];
    
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            dataSend[0] = reg_addr;
            memcpy(&dataSend[1],data,length);

            result = !this->busCtrl->WriteToBus(busUser, DeviceI2CAddrs, dataSend , length+1);
        }
    }
    
    return result;
}

bool MPU9150MP4_1::writeBit(char reg_addr, uint8_t bit, bool value)
{
    return writeBits(reg_addr, bit, 1, (uint8_t)value);
}

bool MPU9150MP4_1::writeBits(char reg_addr, uint8_t bit_start, uint8_t length, uint8_t data)
{   
    char ret;
    
    if(!read(reg_addr, &ret)){
        return false;    
    }
    
    uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1); 
    data <<= (bit_start - length + 1);
     
    data &= mask;
    ret &= ~(mask);
    ret |= data;

    return write(reg_addr, ret);
}

bool MPU9150MP4_1::read(char reg_addr, char* data)
{
   return read(reg_addr, data, 1);
}

bool MPU9150MP4_1::read(char reg_addr, char* data, int length)
{
    bool result = false;
    
    if (this->busCtrl != NULL)
    {
        if (this->busCtrl->TakeCtrl(BusCtrlUsers::IMU))
        {
            result = !this->busCtrl->ReadFromBus(busUser,DeviceI2CAddrs,reg_addr, data, length,true);
        }       
    }
    
    return result;
}


bool MPU9150MP4_1::readBit(char reg_addr, uint8_t bit_start, uint8_t *data)
{
    return readBits(reg_addr, bit_start, 1, data);
}

bool MPU9150MP4_1::readBits(char reg_addr, uint8_t bit_start, uint8_t length, uint8_t *data)
{   
    char ret;
    
    if(!read(reg_addr, &ret)){
        return false;    
    }
        
    uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
    ret &= mask;
    ret >>= (bit_start - length + 1);
    *data = ret;
    
    return true;
}