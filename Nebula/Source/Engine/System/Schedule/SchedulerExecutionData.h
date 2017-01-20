/**
A data structure for storing information about the execution of a scheduler.

@date edited 20/01/2017
@date authored 05/01/2017

@author Nathan Sainsbury */

#ifndef SCHEDULER_EXECUTION_DATA_H
#define SCHEDULER_EXECUTION_DATA_H

struct SchedulerExecutionData
{
	/**
	The number of frames that were executed by the scheduler. */
	unsigned long long uiFramesExecuted;

	/**
	The total number of frames that were delayed during execution by the scheduler for any 
	reason. */
	unsigned long long uiFramesDelayedTotal;

	/**
	The number of frames that were delayed during execution by the scheduler due to threads waking
	up too late. */
	unsigned long long uiFramesDelayedThreadWake;

	/**
	The number of scheduled item update calls that were skipped at the request of scheduled 
	items. */
	unsigned long long uiSkippedUpdateCalls;

	/**
	The number of refused scheduler item stop requests. */
	unsigned long long uiRefusedStopRequests;
};

#endif