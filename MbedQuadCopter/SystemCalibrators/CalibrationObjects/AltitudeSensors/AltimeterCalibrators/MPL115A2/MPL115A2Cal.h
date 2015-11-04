#ifndef MPL115A2Cal_H
#define MPL115A2Cal_H
#include "AltimeterCalBase.h"
#include "MPL115A2CalData.h"
#include "AltimeterStatus.h"

class MPL115A2Cal : public AltimeterCalBase
{
    public:
        MPL115A2Cal(MPL115A2CalData::CalData *calData, AltimeterStatus::AltimeterStatusStruct* altStatus);
        virtual const float GetPressureOffset() const;
        virtual const float GetAltitudeOffset() const; 
    private:
        const MPL115A2CalData::CalData* calData; 
        AltimeterStatus::AltimeterStatusStruct* altStatus;
        bool validCalData;
};


#endif