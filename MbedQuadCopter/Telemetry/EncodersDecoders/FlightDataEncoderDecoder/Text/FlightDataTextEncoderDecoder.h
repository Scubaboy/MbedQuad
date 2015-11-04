#ifndef FlightDataTextEncoderDecoder_H
#define FlightDataTextEncoderDecoder_H
#include "EncodersDecodersBase.h"
#include "SensorFlightData.h"
#include "BaseComms.h"
#include "FlightDataEncoderDecoderStruct.h"

class FlightDataTextEncoderDecoder : public EncodersDecodersBase
{
    public:
      FlightDataTextEncoderDecoder(const SensorFlightData::SensorFlightDataStruct* systemFlightData, BaseComms* comms);
      virtual void Encode();  
    private:
        void BuildAccelFlightData(char* sendBuffer); 
        void BuildGyroFlightData(char* sendBuffer);
        void BuildMagFlightData(char* sendBuffer);
        void BuildIMUFlightData(char* sendBuffer);
        void BuildFlightPackData(char* sendBuffer);
    private:
        virtual void Decode();
        const SensorFlightData::SensorFlightDataStruct* systemFlightData;
        BaseComms* comms;
};
#endif