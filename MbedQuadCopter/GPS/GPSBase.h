#ifndef GPSBase_H
#define GPSBase_H

class GPSBase
{
    public:
        virtual bool Initialise() = 0;
        virtual void ReadPositionData() = 0;   
};
#endif