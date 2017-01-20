/**
The scheduler event types enum lists and documents the available scheduler event types.

@date edited 20/01/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_EVENT_TYPES_H
#define SCHEDULER_EVENT_TYPES_H

enum class SchedulerEventTypes
{
	/**
	The event was of an unknown type. To be used in exceptional circumstances or to signal
	currently unsupported features. */
	UNKNOWN_EVENT,

	/**
	Indicates that the scheduler is unable to meet the target execution rate. This event has an 
	internal (configurable) cooldown that prevents it from being launched too frequently. */
	SCHEDULER_FALLING_BEHIND,

	/**
	Indicates that the scheduler has started updating its scheduled items. This event occurs 
	before any internal configurations are applied meaning it is viable to configure the scheduler
	in response to this event. */
	SCHEDULER_STARTED,

	/**
	Indicates that the scheduler has stopped updating its scheduled items. This event occurs after
	the update loop exits but before the scheduler update function returns. */
	SCHEDULER_STOPPED,
};

#endif