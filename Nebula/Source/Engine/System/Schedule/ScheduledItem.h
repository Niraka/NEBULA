/**
A scheduled item is the base class for any class that wishes to be able to register for updates with 
a scheduler.

Users should inherit from the class, implement the OnUpdate function and register the schedule
with a scheduler using an associated scheduler rate.

@date edited 20/01/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULED_ITEM_H
#define SCHEDULED_ITEM_H

#include "Engine/System/Schedule/SchedulerTimeInfo.h"

class ScheduledItem
{
	public:
		/**
		This function is called by the scheduler as close to the requested update rate as possible. 
		Additional timing information is provided for convenience.
		@param info The time info */
		virtual void onUpdate(const SchedulerTimeInfo& info) = 0;
		
		/**
		This function is called by the scheduler when it starts executing.
		@param info The time info */
		virtual void onSchedulerStart(const SchedulerTimeInfo& info);
		
		/**
		This function is called by the scheduler once it has stopped executing.
		@param info The time info */
		virtual void onSchedulerStop(const SchedulerTimeInfo& info);

		/**
		Queries whether the scheduled item is requesting that the scheduler stops executing.
		@return True if the item is requesting a stop, false otherwise */
		bool isRequestingSchedulerStop() const;

		/**
		Queries whether the scheduled item is requesting that the next call to this items update
		function is skipped.
		@return True if the item is requesting a skip, false otherwise */
		bool isRequestingFrameSkip() const;

		/**
		Resets all request flags to false. */
		void resetFlags();

	protected:
		bool m_bRequestingSchedulerStop;
		bool m_bRequestingFrameSkip;

		/**
		Default constructor. */
		ScheduledItem();

	private:
};

#endif