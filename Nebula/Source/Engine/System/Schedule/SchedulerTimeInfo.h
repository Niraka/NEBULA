/**
A scheduler time info structure contains information about the current time as it is reported
by a scheduler.

@date edited 20/01/2017
@date authored 25/08/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_TIME_INFO_H
#define SCHEDULER_TIME_INFO_H

#include <chrono>

struct SchedulerTimeInfo
{
	/**
	The time in nanoseconds that the scheduler began executing. This value is consistent across all
	scheduled items being updated this frame. */
	std::chrono::nanoseconds timeEpoch;
	
	/**
	The elapsed time in nanoseconds since the scheduler began executing. This value is recomputed 
	for each scheduled item. */
	std::chrono::nanoseconds timeSinceEpoch;

	/**
	The time in nanoseconds at the start of the frame of execution (A frame is 16.6ms when running
	at 60fps). This value is consistent across all scheduled items being updated this frame. */
	std::chrono::nanoseconds timeFrameStart;

	/**
	The interpolation is the difference between the expected call time and the actual call
	time as a multiplicative factor. For example, an interpolation of 1.15 means that the
	onUpdate call arrived 15% later than intended. This value is recomputed for each scheduled 
	item. */
	double fInterpolation;

	/**
	The time in nanoseconds now. This value is recomputed for each scheduled item. */
	std::chrono::nanoseconds timeNow;

	/**
	The time in nanoseconds since the last call to onUpdate on this scheduled item. This value is
	recomputed for each scheduled item. */
	std::chrono::nanoseconds timeLastUpdate;
};

#endif