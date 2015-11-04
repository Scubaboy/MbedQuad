#ifndef I2CBusCtrlTypes_H
#define I2CBusCtrlTypes_H
#include "BusUsers.h"

namespace I2CBusCtrlUser
{
    typedef struct I2CBusUserConfig : public BusCtrlUsers::BusUserConfig
    {
        int busFrequency;
    }I2CBusUserConfig;
};
#endif