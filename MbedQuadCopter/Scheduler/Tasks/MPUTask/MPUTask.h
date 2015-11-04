#ifndef MPUTask_H
#define MPUTask_H
#include "Task.h"
#include "IMUBase.h"
#include "BusCtrl.h"
#include "IMUFlightData.h"

class MPUTask : public Task
{
    public:
        MPUTask(IMUBase* mpu, BusCtrl* busCtrl);
        virtual void ExecuteTask(int interval);
        virtual void Init();
    private:
        IMUBase* mpu;
        BusCtrl* busCtrl;
        bool initRun;
};
#endif