#ifndef FlightDataBinaryEncoderDecoder_H
#define FlightDataBinaryEncoderDecoder_H
#include "EncodersDecodersBase.h"
#include "SensorFlightData.h"
#include "BaseComms.h"
#include "FlightDataEncoderDecoderStruct.h"
#include "FlightDataDataPck.h"
class FunctionPtrInternal;

class FlightDataBinaryEncoderDecoder : public EncodersDecodersBase
{
    public:
      FlightDataBinaryEncoderDecoder(const SensorFlightData::SensorFlightDataStruct* systemFlightData);
      virtual void Encode(); 
    
    public:
        template <class ClassT>
        void SendRequest(ClassT *ptr, bool(ClassT::*pFunc)(char* data, const int DataLength, bool confRequired))
        {
                this->sendRequestrptr = (FunctionPtrInternal*)ptr;  
                this->methodCallbackSendRequest = (bool (FunctionPtrInternal::*)(char* data, const int DataLength, bool confRequired))pFunc; 
        } 
    
    private:
        void BuildLocalFlightData();  
        void SendRequest(FlightDataDataPck::FlightData* data);
    private:
        FunctionPtrInternal* sendRequestrptr;
        bool (FunctionPtrInternal::*methodCallbackSendRequest)(char* data, const int DataLength, bool confRequired);
        virtual void Decode();
        const SensorFlightData::SensorFlightDataStruct* systemFlightData;
        FlightDataDataPck::FlightData localFlightData;
};
#endif