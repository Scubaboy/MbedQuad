#include "XYZAxisCalController.h"
#include "mbed.h"

const int RequiredSamples = 10;
static MsgDataPck::MsgData request;


XYZAxisCalController::XYZAxisCalController (BaseDataStorageController* dataStorageController, 
                           XYZAxisMinMax::XYZAxisMinMaxCalData *calData, 
                           BaseThreeAxisFlightData::BaseThreeAxisFlightDataStruct* flightData,
                           UserResponce::UserResponceStruct* userResponce, 
                           SensorBase* sensor,
                           const XYZAxisCalCtrlTypes::AxisCalNameLookup* lookUp) : test(LED1)
{
     this->sensor = sensor;
     this->calData = calData;
     this->calComplete = false;
     this->flightData = flightData;
     this->dataStorageController = dataStorageController;
     this->calStage = XYZAxisCalCtrlTypes::CheckStorage;
     this->userResponce = userResponce; 
     this->axisUnderCal = XYZAxisCalCtrlTypes::XAxisCal;
     this->axisMag = XYZAxisCalCtrlTypes::Min;
     this->axisCalStage = XYZAxisCalCtrlTypes::SendAxisStartRequest;
     this->lookUp = lookUp;

}

void XYZAxisCalController::SendInstruction(XYZAxisCalCtrlTypes::Axis axis, XYZAxisCalCtrlTypes::AxisMinMax minMax)
{
    
    this->CompileRequest((*this->lookUp)[axis][minMax], strlen((*this->lookUp)[axis][minMax]), true);
}

float XYZAxisCalController::AverageReading(XYZAxisCalCtrlTypes::Axis axis)
{
    int samplesTaken = 0;
    float axisReading = 0.0;
           
    while (samplesTaken < RequiredSamples)
    {
        this->sensor->GetRawSensorData();
        
        switch (axis)
        {
            case XYZAxisCalCtrlTypes::XAxisCal:
            {
                axisReading = this->flightData->rawXAxis;
                break;
            }
            case XYZAxisCalCtrlTypes::YAxisCal:
            {
                axisReading = this->flightData->rawYAxis;
                break;
            }
            case XYZAxisCalCtrlTypes::ZAxisCal:
            {
                axisReading = this->flightData->rawZAxis;
                break;
            }
        }
        
        samplesTaken++;
    }
    
    return axisReading/(float)RequiredSamples;
}

bool XYZAxisCalController::RunAxisCalibration(XYZAxisCalCtrlTypes::Axis axis, XYZAxisCalCtrlTypes::AxisMinMax minMax)
{
    float axisReading = 0.0;
    bool complete = false;
    
    axisReading = this->AverageReading(axis);
    
    switch (axis)
    {
        case XYZAxisCalCtrlTypes::XAxisCal:
        {
            switch (this->axisMag)
            {
                case XYZAxisCalCtrlTypes::Min:
                {
                    this->calData->xMin = axisReading;
                    complete = true;
                    break;
                }
                case XYZAxisCalCtrlTypes::Max:
                {
                    this->calData->xMax = axisReading;
                    complete = true;
                    break;
                }
            }
            
            break;   
        }
        case XYZAxisCalCtrlTypes::YAxisCal:
        {
            switch (this->axisMag)
            {
                case XYZAxisCalCtrlTypes::Min:
                {
                    this->calData->yMin = axisReading;
                    complete = true;
                    break;
                }
                case XYZAxisCalCtrlTypes::Max:
                {
                    this->calData->yMax = axisReading;
                    complete = true;
                    break;
                }
            }
            
            break;   
        }
        case XYZAxisCalCtrlTypes::ZAxisCal:
        {
            switch (this->axisMag)
            {
                case XYZAxisCalCtrlTypes::Min:
                {
                    this->calData->zMin = axisReading;
                    complete = true;
                    break;
                }
                case XYZAxisCalCtrlTypes::Max:
                {
                    this->calData->zMax = axisReading;
                    complete = true;
                    break;
                }
            }
            
            break;   
        } 
    }
    return complete;
}

bool XYZAxisCalController::CalibrateAxis()
{
    bool axisCalComplete = false;
    
    switch (this->axisCalStage) 
    {
        case XYZAxisCalCtrlTypes::SendAxisStartRequest:
        {
            this->SendInstruction(this->axisUnderCal, this->axisMag);
            if (!this->waitingLastSend)
            {
                this->axisCalStage =  XYZAxisCalCtrlTypes::WaitingAxisStartAck;   
            }
            
            break;
        }
        case XYZAxisCalCtrlTypes::WaitingAxisStartAck:
        {
            
            if (this->userResponce->userAck)
            {
                this->axisCalStage =  XYZAxisCalCtrlTypes::CalibratingAxis;
                this->userResponce->userAck = false;
                test = !test;     
            }
            else
            {

                float axisReading = this->AverageReading(this->axisUnderCal);  
                
               // if (this->lastReading > axisReading)
               // {
                char msg[9];
                sprintf(msg,"<%.1f>\0", axisReading);
                this->CompileRequest(msg, strlen(msg), false); 
               // }
                
               // this->lastReading = axisReading;
            }
            break;   
        }
        case XYZAxisCalCtrlTypes::CalibratingAxis:
        {
            switch (this->axisMag)
            {
                case XYZAxisCalCtrlTypes::Min:
                {
                    if (this->RunAxisCalibration(this->axisUnderCal, this->axisMag))
                    {
                        this->axisMag = XYZAxisCalCtrlTypes::Max;
                        axisCalStage = XYZAxisCalCtrlTypes::SendAxisStartRequest;
                    }
                    
                    break;
                }
                case XYZAxisCalCtrlTypes::Max:
                {
                    if (this->RunAxisCalibration(this->axisUnderCal, this->axisMag))
                    {
                        axisCalStage = XYZAxisCalCtrlTypes::AxisCalComplete;
                    }
                    
                    break;
                }
            }
            break;
        }
        case XYZAxisCalCtrlTypes::AxisCalComplete:
        {
            this->calibrationData.accelCal.xMin = calData->xMin;
            this->calibrationData.accelCal.xMax = calData->xMax;
            this->calibrationData.accelCal.yMin = calData->yMin;
            this->calibrationData.accelCal.yMax = calData->xMax;
            this->calibrationData.accelCal.zMin = calData->zMin;
            this->calibrationData.accelCal.zMax = calData->zMax;
            axisCalComplete = true;
            break;
        }
    }
    
    return axisCalComplete;
}

bool XYZAxisCalController::RunCalibration()
{
    bool calResult = false;
    
    switch (this->calStage)
    {
        case XYZAxisCalCtrlTypes::CheckStorage:
        {
            if (this->CheckAndUseStoredCalData())
            {
                this->calStage = XYZAxisCalCtrlTypes::Complete;
            }
            else
            {
                this->calStage = XYZAxisCalCtrlTypes::Calibrate;
            }
            
            break;   
        }
        case XYZAxisCalCtrlTypes::Calibrate:
        {
            switch (this->axisUnderCal)
            {
                case XYZAxisCalCtrlTypes::XAxisCal:
                {
                    if (this->CalibrateAxis())
                    {
                        this->axisMag = XYZAxisCalCtrlTypes::Min;
                        this->axisCalStage = XYZAxisCalCtrlTypes::SendAxisStartRequest;
                        this->axisUnderCal = XYZAxisCalCtrlTypes::YAxisCal;
                    }
                    break;   
                }
                case XYZAxisCalCtrlTypes::YAxisCal:
                {
                    if (this->CalibrateAxis())
                    {
                        this->axisMag = XYZAxisCalCtrlTypes::Min;
                        this->axisCalStage = XYZAxisCalCtrlTypes::SendAxisStartRequest;
                        this->axisUnderCal = XYZAxisCalCtrlTypes::ZAxisCal;
                    }
                    
                    break;
                }
                case XYZAxisCalCtrlTypes::ZAxisCal:
                {
                    if (this->CalibrateAxis())
                    {
                        //this->axisMag = XYZAxisCalCtrlTypes::Min;
                        //this->axisCalStage = XYZAxisCalCtrlTypes::SendAxisStartRequest;
                        this->calStage = XYZAxisCalCtrlTypes::Complete;
                    }
                    
                    break;
                }
            }
            break;
        }
        case XYZAxisCalCtrlTypes::Complete:
        {
            this->WriteCalDataToDataStore();
            calResult = true;   
            break; 
        }
    }
    
    return calResult; 
}


bool XYZAxisCalController::SendRequest(char* msg, int len, bool reSend)
{
    if (this->sendRequestrptr != NULL && this->methodCallbackSendRequest != NULL)
    {
        if (!reSend)
        {
            (this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)msg, sizeof(MsgDataPck::MsgData),reSend);
        }
        else
        {
            if (!(this->sendRequestrptr->*this->methodCallbackSendRequest)((char*)msg, sizeof(MsgDataPck::MsgData),reSend))
            {
                this->waitingLastSend = true;
            }
            else
            {
                this->waitingLastSend = false;
            }
        }
    }
}

void XYZAxisCalController::CompileRequest(const char* msg, int len, bool reSend)
{
    //Send Instruction
       
    request.type = reSend ? DataPckTypes::Message : DataPckTypes::FreeTxtMsg;
    request.ackRequired = reSend;
    memset (request.msg,'1',sizeof(request.msg));
    memcpy(request.msg,msg,len);
    this->SendRequest((char*)&request,sizeof(request), reSend); 

}