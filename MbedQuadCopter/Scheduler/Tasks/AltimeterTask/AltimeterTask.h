#ifndef AltimeterTask_H
#define AltimeterTask_H
#include "Task.h"
#include "AltimeterBase.h"
#include "BusCtrl.h"

class AltimeterTask : public Task
{
        public:
        AltimeterTask(AltiMeterBase* altimeter,BusCtrl* busCtrl);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        AltiMeterBase* altimeter;
        BusCtrl* busCtrl;
};
#endif