#include "DefaultSysConfigController.h"
#include "mbed.h"

DefaultSysConfigController::DefaultSysConfigController(SystemConfigurators::SystemConfiguratorStruct* sysConfigurator, BusCtrl* busCtrl)
{
    this->sysConfigurator = sysConfigurator;
    this->stage = DefaultConfigControllerTypes::Motors; 
    this->busCtrl = busCtrl;   
}

bool DefaultSysConfigController::Configure()
{
    bool configResult = false;
    
    if (this->sysConfigurator != NULL)
    {
        switch (this->stage)
        { 
            case DefaultConfigControllerTypes::Motors:
            {
                if (this->sysConfigurator->motorConfig != NULL)
                {
                    if (this->sysConfigurator->motorConfig->Configure())
                    {
                        this->stage = DefaultConfigControllerTypes::Complete;
                    }
                }
                else
                {
                    this->stage = DefaultConfigControllerTypes::Complete;
                }
                
                break;   
            } 
            case DefaultConfigControllerTypes::Complete:
            {
                configResult = true;
                break;   
            } 
        }    
    }
    
    return configResult;
}