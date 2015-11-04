#ifndef DefaultHighLevelController_H
#define DefaultHighLevelController_H
#include "BaseHighLevelRateController.h"
#include "BasicLowLevelRateControllerAtts.h"
#include "HighLevelControllerTypes.h"
#include "BaseResponse.h"
#include "FlightCtrlData.h"

class DefaultHighLevelController : public BaseHighLevelRateController
{
    public:
        DefaultHighLevelController(BasicLowLevelRateControllerAtts* lowLevelRateControllerAtts,
                                   FlightCtrlData::FlightCtrlDataStruct* flightCtrlData);
        virtual void Init(); 
        virtual bool AddAction(HighLevelCtrlActionTypes::ActionRequest actionRequest);
        virtual void Run();
    private:
        void ResetActionList();
        void AddRequestedAction(HighLevelCtrlActionTypes::Actions action);
        void RemoveRequestedAction(HighLevelCtrlActionTypes::Actions action);
    private:
        BasicLowLevelRateControllerAtts* lowLevelRateControllerAtts; 
        BaseHighLevelCtrlAction* actionList[HighLevelControllerTypes::MaxActions];
        int actionInsertIter;
        int takeActionIter;
        bool ready;
        bool actionsConfigured;
        BaseHighLevelCtrlAction* activeAction;
        BaseResponse::BaseResp* actionResponse;
        FlightCtrlData::FlightCtrlDataStruct* flightCtrlData;
};
#endif