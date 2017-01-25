/**
A data structure for storing information about the execution of a scheduler.

@date edited 25/01/2017
@date authored 05/01/2017

@author Nathan Sainsbury */

#ifndef SCHEDULER_EXECUTION_DATA_H
#define SCHEDULER_EXECUTION_DATA_H

#include <stdint.h>

struct SchedulerExecutionData
{
	/**
	The number of frames that were executed by the scheduler. */
	std::uint64_t uiFramesExecuted;

	/**
	The total number of frames that were delayed during execution by the scheduler for any 
	reason. */
	std::uint64_t uiFramesDelayedTotal;

	/**
	The number of frames that were delayed during execution by the scheduler due to threads waking
	up too late. */
	std::uint64_t uiFramesDelayedThreadWake;

	/**
	The number of scheduled item update calls that were skipped at the request of scheduled 
	items. */
	std::uint64_t uiSkippedUpdateCalls;

	/**
	The number of refused scheduler item stop requests. */
	std::uint64_t uiRefusedStopRequests;
};

#endif