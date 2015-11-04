#ifndef AltimeterCalBase_H
#define AltimeterCalBase_H

class AltimeterCalBase
{
    public:
        virtual const float GetPressureOffset() const  = 0;
        virtual const float GetAltitudeOffset() const = 0;
};

#endif