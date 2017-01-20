/**
A scheduler config structure contains information about a current or desired configuration of a
scheduler.

"Fixed timestepping" AKA every frame of execution reportedly takes exactly the same amount of time
can be enabled by setting the interpolation cap to 1.

@date edited 20/01/2017
@date authored 07/01/2017

@author Nathan Sainsbury */

#ifndef SCHEDULER_CONFIG_H
#define SCHEDULER_CONFIG_H

#include "Engine/System/Schedule/SchedulerRate.h"

struct SchedulerConfig
{
	/**
	The update rate of the scheduler. Scheduled items cannot be updated faster than the rate of
	the scheduler itself. Unlimited and very high rates are supported but are not recommended.
	Note that update rates below 1 or above 120 are considered unlimited. */
	SchedulerRate updateRate;

	/**
	The maximum possible interpolation value. To disable interpolation capping, use any value 
	below 1. Note that large interpolation values (>2.0) can result in unusual effects in time-
	dependent systems (E.G: Physics simulations). */
	double fInterpolationCap;
	
	/**
	Sets the interpolation value that registers a frame of execution as 'delayed'. Disabled if
	interpolation capping is also disabled. Suggested values are between 1.05 and 1.10. Minimum
	1.01. */
	double fInterpolationLagThreshold;

	/**
	The number of seconds between each lag warning. Setting this to 0 will disable the warning
	entirely. */
	unsigned int uiLagWarningFrequency;
	
	/**
	Enables or disables the ability to stop the scheduler via a scheduled items requestStop flag. */
	bool bRefuseStopRequests;

	/**
	Constructs a default configured scheduler config. */
	SchedulerConfig()
	{
		setDefaults();
	}

	/**
	Sets all fields to their default values. */
	void setDefaults()
	{
		updateRate = SchedulerRate(SchedulerRatePresets::_60_PER_SECOND);
		fInterpolationCap = 1.1;
		fInterpolationLagThreshold = 1.025;
		uiLagWarningFrequency = 10;
		bRefuseStopRequests = true;
	}
};

#endif