#ifndef MLP115A2_H
#define MLP115A2_H
#include "AltimeterBase.h"
#include "AltimeterStatus.h"
#include "AltimeterCalBase.h"
#include "I2CBusCtrl.h"
#include "AltMeterFlightData.h"

class MLP115A2 : public AltiMeterBase
{
    public:
        MLP115A2(AltimeterCalBase* calObj,I2CBusCtrl* busCtrl, AltimeterStatus::AltimeterStatusStruct* altStatus, AltMeterFlightData::AltMeterFlightDataStruct* flightData);
        virtual void UpdateAltitude();  
        virtual void UpdatePresure();
        virtual void UpdateTemperature();
        virtual bool ReadSensor();
        virtual void Init();
        virtual void SetOffsets();
    private:
        const AltimeterCalBase* calObj;
        AltimeterStatus::AltimeterStatusStruct* altStatus;
        AltMeterFlightData::AltMeterFlightDataStruct* flightData;
        I2CBusCtrl* busCtrl;
        bool sensorReady;
        short sia0, sib1, sib2, sic12, sic11, sic22;
        unsigned short uiPadc, uiTadc;
};

#endif