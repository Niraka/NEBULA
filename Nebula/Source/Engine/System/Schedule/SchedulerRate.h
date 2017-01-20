/**
A scheduler rate describes an update rate to be used by the scheduler.

Update rates are stored as nanosecond timesteps internally. For example, an update rate of
60 ticks per second will be represented by 16600000 nanoseconds.

@date edited 20/01/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_RATE_H
#define SCHEDULER_RATE_H

#include <chrono>

#include "Engine/System/Schedule/SchedulerIntervals.h"
#include "Engine/System/Schedule/SchedulerRatePresets.h"

class SchedulerRate
{
	private:
		std::chrono::nanoseconds m_timestep;

	protected:

	public:
		/**
		Constructs a rate object. Defaults to 60 ticks per second. */
		SchedulerRate();

		/**
		Constructs a rate with the given count at the given interval.
		@param iCount The count
		@param interval The interval */
		SchedulerRate(int iCount, SchedulerIntervals interval);

		/**
		Constructs a rate with the given preset.
		@param preset The preset */
		SchedulerRate(SchedulerRatePresets preset);

		/**
		Sets the rate.
		@param iCount The count
		@param interval The interval */
		void set(int iCount, SchedulerIntervals interval);

		/**
		Sets the rate.
		@param preset The preset */
		void set(SchedulerRatePresets preset);

		/**
		Gets the rate.
		@return The rate */
		const std::chrono::nanoseconds& getTimestep() const;
};

#endif