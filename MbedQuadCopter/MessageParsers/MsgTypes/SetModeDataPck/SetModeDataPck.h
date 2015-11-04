#ifndef SetModeDataPck_H
#define SetModeDataPck_H

namespace SetModeDataPck
{
    struct SetMode : public DataPckTypeBase::DataPck
    {
        unsigned int mode;
    };  
};

#endif