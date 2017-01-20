/**
Scheduler rate presets define a number of human readable presets for scheduler rates.

@date edited 20/01/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_RATE_PRESETS_H
#define SCHEDULER_RATE_PRESETS_H

enum class SchedulerRatePresets
{
	/**
	30 ticks per second. */
	_30_PER_SECOND,

	/**
	60 ticks per second. */
	_60_PER_SECOND,

	/**
	90 ticks per second. */
	_90_PER_SECOND,

	/**
	120 ticks per second. */
	_120_PER_SECOND,	
	
	/**
	An unlimited rate. */
	UNLIMITED
};

#endif