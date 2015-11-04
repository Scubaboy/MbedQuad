#ifndef IMUBase_h
#define IMUBase_h
#include "IMUTypes.h"

class FunctionPtrInternal;

class IMUBase
{
    public:
        virtual void UpdateEulerAngles(float timeDelta) = 0;
        virtual void Init() = 0;
    private:
        virtual void CallCriticalStop() = 0;
    
};

#endif