/**
A scheduler event contains information about an event that occurred within a Scheduler.

The event type field is always valid.

@see SchedulerEventTypes.h

@date edited 26/03/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_EVENT_H
#define SCHEDULER_EVENT_H

#include "Engine/System/Schedule/SchedulerEventTypes.h"

struct SchedulerEvent
{
	/**
	The event type. */
	SchedulerEventTypes type;
	
	/**
	Constructs a scheduler event. */
	SchedulerEvent() :
		type(SchedulerEventTypes::UNKNOWN_EVENT)
	{
	}

	/**
	Constructs a scheduler event of the given type.
	@param type The event type */
	SchedulerEvent(SchedulerEventTypes type) :
		type(type)
	{
	}
};

#endif