#include "SysConfigCtrl.h"
#include "MLP115A2.h"
#include "AltimeterCalibrator.h"
#include "DefaultSysCalController.h"

void SysCtrlConfigCtrl::CreateAltimeter(SystemConfigurationTypes::AltimeterOption altOption)
{
   
   const unsigned int GPSAltitude = 0;
    const unsigned int AltimeterAltitude = 1;
    const unsigned int GPSAltimeterAltitude = 2; 
    
    switch (altOption)
    {
        case SystemConfigurationTypes::AltimeterAltitude:
        {
            selectedSysConf->altimeterController = new MLP115A2(selectedSysConf->sensorCalibrators->altCal,
                                            this->busCtrl1,
                                            &this->sysStatus->altStatus,
                                            this->flightData->altimeterFlightData);
                                            selectedSysConf->altimeterBusCtrl = this->busCtrl1;
                                           
            this->busCtrl1->TakeCtrl(BusCtrlUsers::Alt);
                selectedSysConf->altimeterController->Init();
            this->busCtrl1->ReleaseCtrl(BusCtrlUsers::Alt);
    
            //Setup altimeter calibration controller
            AltimeterCalibrator* altCal = new AltimeterCalibrator(selectedSysConf->altimeterController,
                                                                  this->flightData->altimeterFlightData,
                                                                  selectedSysConf->sensorCalibrators->altCalData,
                                                                  &this->dataRequestRcvs->altDataReqRcvs); 
                                                                  
            selectedSysConf->sensorCalibrators->altCalibrator = altCal;   
            
            if (selectedSysConf->calibrationCtrl == NULL)
            {
                selectedSysConf->calibrationCtrl = new DefaultSysCalController(this->busCtrl1, this->selectedSysConf->sensorCalibrators);
            }
                                                                            
            altCal->SendRequest(this->selectedSysConf->systemComms, &BaseComms::SendDataPacket);      
            selectedSysConf->altimeterBusCtrl = this->busCtrl1;
            break;   
        }
        case SystemConfigurationTypes::GPSAltitude:
        {
            
            break;   
        }
        case SystemConfigurationTypes::GPSAltimeterAltitude:
        {
            break;   
        }
    }
}