#ifndef DefaultSysConfigController_H
#define DefaultSysConfigController_H
#include "BaseSysConfigController.h"
#include "SystemConfigurator.h"
#include "DefaultConfigControllerTypes.h"
#include "BusCtrl.h"

class DefaultSysConfigController : public BaseSysConfigController
{
    public:
        DefaultSysConfigController(SystemConfigurators::SystemConfiguratorStruct* sysConfigurator, BusCtrl* busCtrl);
        virtual bool Configure();
    private:
        SystemConfigurators::SystemConfiguratorStruct* sysConfigurator;
        DefaultConfigControllerTypes::Stages stage;
        BusCtrl* busCtrl;
        
};
#endif