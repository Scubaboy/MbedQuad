#include "DefaultHighLevelController.h"
#include "BaseResponse.h"
#include "mbed.h"

DefaultHighLevelController::DefaultHighLevelController(BasicLowLevelRateControllerAtts* lowLevelRateControllerAtts,
                                                       FlightCtrlData::FlightCtrlDataStruct* flightCtrlData)
{
    this->lowLevelRateControllerAtts = lowLevelRateControllerAtts;
    this->actionInsertIter = 0;
    this->ready = false;
    this->actionsConfigured = false;
    this->activeAction = NULL;
    this->actionResponse = NULL;
    this->flightCtrlData = flightCtrlData;
}

void DefaultHighLevelController::Init()
{
    this->ResetActionList();
    this->ready = true;
}

void DefaultHighLevelController::ResetActionList()
{
    for (int iter = 0; iter < HighLevelControllerTypes::MaxActions; iter++)
    {
        this->actionList[iter] = NULL;   
    }
    this->takeActionIter = 0;
    this->actionsConfigured = false;
    this->activeAction = NULL;
}

bool DefaultHighLevelController::AddAction(HighLevelCtrlActionTypes::ActionRequest actionRequest)
{
    switch (actionRequest.request)
    {
        case HighLevelCtrlActionTypes::Add:
        {
            this->AddRequestedAction(actionRequest.action);
            break;   
        }
        
        case HighLevelCtrlActionTypes::Remove:
        {
            this->RemoveRequestedAction(actionRequest.action);
            break;   
        }   
    }
   // bool result = false;
    
   // if (this->actionInsertIter < HighLevelControllerTypes::MaxActions && action != NULL)
    //{
    //   this->actionList[this->actionInsertIter++] = action;
    //   result = true;
    //   this->actionsConfigured = true;
   // }
    
   // return result;
}

void DefaultHighLevelController::AddRequestedAction(HighLevelCtrlActionTypes::Actions action)
{
    
}

void DefaultHighLevelController::RemoveRequestedAction(HighLevelCtrlActionTypes::Actions action)
{
    
}
void DefaultHighLevelController::Run()
{
    if (this->ready && this->actionsConfigured)
    {
        if (this->activeAction == NULL)
        {
            //Take first action.
            this->activeAction = this->actionList[this->takeActionIter];
            this->takeActionIter++;
        }
        
        if (this->activeAction->Compteleted())
        {
            //Start a new action if one exsists else run the current one.
            if (this->takeActionIter < this->actionInsertIter)
            {
                this->activeAction = this->actionList[this->takeActionIter++];
            }
        } 
        
        this->actionResponse = this->activeAction->Run(); 
        
        switch (this->actionResponse->type)
        {
            case BaseResponse::Hover:
            {
              //  HoverResponse::HoverResp* resp = (HoverResponse::HoverResp*)this->actionResponse;
                
             //   this->lowLevelRateControllerAtts->SetThrottleOut(resp->throttle,false);
             //   this->lowLevelRateControllerAtts->AngleRollPitchRateYaw(resp->rollDegrees, resp->pitchDegrees, resp->yawDegrees);
            }
        }
        
        //what about throttle???? ctrl
    }
}