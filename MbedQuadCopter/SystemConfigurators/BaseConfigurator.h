#ifndef BaseConfigurator_H
#define BaseConfigurator_H
#include "DataRequestPck.h"

class FunctionPtrInternal;

class BaseConfigurator
{
    public:
        virtual bool Configure() = 0;
    private:
        virtual void SendRequest(DataRequestPck::DataRequest* dataRequest) = 0;
};
#endif