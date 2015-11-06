#include "HeartBeatMonitorTask.h"
#include <time.h>

HeartBeatMonitorTask::HeartBeatMonitorTask(HeartBeatMonitor* heartBeatMonitor, TimeSynchStatus::TimeSynchStatusStruct* timeSynchStatus)
{
	this->heartBeatMonitor = heartBeatMonitor;
	this->timeSynchStatus = timeSynchStatus;
	this->startTimesRecorded = false;
}

void HeartBeatMonitorTask::ExecuteTask(int interval)
{
	if (this->heartBeatMonitor != NULL && this->timeSynchStatus != NULL)
	{
		if (this->timeSynchStatus->timeSynched)
		{
			if (!this->startTimesRecorded)
			{
				this->previousSendTime = time(NULL);
				this->previousMonitorTime = time(NULL);
				this->startTimesRecorded = true;
			}

			//Check heart beat send time
			double sendTimeDiff = difftime(time(NULL), previousSendTime);
			if (sendTimeDiff >= 10.0)
			{
				this->heartBeatMonitor->SendHeartBeat();
				previousSendTime = time(NULL);
			}

			//Check heart beat monitor time
			double monitorTimeDiff = difftime(time(NULL), previousMonitorTime);
			if (monitorTimeDiff >= 60.0)
			{
				this->heartBeatMonitor->MonitorHeartBeat();
				previousMonitorTime = time(NULL);
			}
		}
	}

}

void HeartBeatMonitorTask::Init()
{
}
