/**
A scheduler listener is an interface class that defines the necessary functionality for a class to 
be able to register to listen for scheduler events.

@date edited 26/03/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_LISTENER_H
#define SCHEDULER_LISTENER_H

#include "Engine/System/Schedule/SchedulerEvent.h"

class SchedulerListener
{
	public:
		/**
		This function is called when a scheduler that this scheduler lister is listening to launches
		an event.
		@param eve The event */
		virtual void onSchedulerEvent(const SchedulerEvent& eve) = 0;	

	protected:

	private:

};

#endif