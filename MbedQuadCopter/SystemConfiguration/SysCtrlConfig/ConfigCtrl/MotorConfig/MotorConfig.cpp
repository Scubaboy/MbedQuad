#include "SysConfigCtrl.h"
#include "MotorConfig.h"
#include "DefaultSysConfigController.h"
#include "PCA9685PWMCtrl.h"
#include "QuadMotorController.h"
#include "BasicThrYawPitchRollMix.h"

void SysCtrlConfigCtrl::CreateMotor()
{
    //Create motor controller  
    selectedSysConf->pwmController = new PCA9685PWMCtrl(&this->motorConfigFlightData->pwmChannelMotorMapping, this->busCtrl1);   
    
    selectedSysConf->motorController = new QuadMotorController(this->motorConfigFlightData,
                                                                new BasicThrYawPitchRollMix(&this->motorConfigFlightData->pwmChannelMotorMapping),
                                                               selectedSysConf->pwmController);
                                                               
    
    selectedSysConf->motorBusCtrl = this->busCtrl1;
    
    //Create calibration/config controllers
   MotorConfig* motorConfigurator= new MotorConfig (selectedSysConf->systemConfigurators->motorConfigData,
                                                   selectedSysConf->dataStorageController,
                                                   &this->dataRequestRcvs->motorConfigRcvs); 
    motorConfigurator->SendRequest(this->selectedSysConf->systemComms, &BaseComms::SendDataPacket);
    
    selectedSysConf->systemConfigurators->motorConfig = motorConfigurator;
    
    this->selectedSysConf->systemConfigCtrl = new DefaultSysConfigController(selectedSysConf->systemConfigurators, this->busCtrl1); 
    this->selectedSysConf->configurationCtrlBusCtrl = this->busCtrl1;
}