#include "SysConfigCtrl.h"

void SysCtrlConfigCtrl::ReleaseCalConfigObjects()
{
    if (this->selectedSysConf->sensorCalibrators->gyroCalibrator != NULL)
            {
                delete this->selectedSysConf->sensorCalibrators->gyroCalibrator;   
            }
            
            if (this->selectedSysConf->sensorCalibrators->accelCalibrator != NULL)
            {
                delete this->selectedSysConf->sensorCalibrators->accelCalibrator;   
            }
            
            if (this->selectedSysConf->sensorCalibrators->magCalibrator != NULL)
            {
                delete this->selectedSysConf->sensorCalibrators->magCalibrator;
            }
            
            if (selectedSysConf->calibrationCtrl != NULL)
            {
                delete selectedSysConf->calibrationCtrl;    
            }
            
            if (selectedSysConf->sensorCalibrators->altCalibrator != NULL)
            {
                delete selectedSysConf->sensorCalibrators->altCalibrator;   
            }
}