#include "HeartBeatSendTask.h"

HeartBeatSendTask::HeartBeatSendTask(HeartBeatMonitor* heartBeatMonitor)
{
	this->heartBeatMonitor = heartBeatMonitor;
}

void HeartBeatSendTask::ExecuteTask(int interval)
{
	if (this->heartBeatMonitor != NULL)
	{
		this->heartBeatMonitor->SendHeartBeat();
	}

}

void HeartBeatSendTask::Init()
{

}
