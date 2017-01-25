/**
The scheduler is a time-keeping and automatic update delivery system that will periodically update
a set of schedule objects according the specification it is provided.

The scheduler is not thread-safe. All function calls should occur on a single thread (or should be
exceptionally carefully managed).

@date edited 25/01/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <chrono>
#include <algorithm>
#include <cstdint>

#include "Engine/System/Tools/Pair.h"
#include "Engine/System/Tools/Vector.h"
#include "Engine/System/Schedule/SchedulerListener.h"
#include "Engine/System/Schedule/SchedulerTimeInfo.h"
#include "Engine/System/Schedule/SchedulerItemInfo.h"
#include "Engine/System/Schedule/ScheduledItem.h"
#include "Engine/System/Schedule/SchedulerRate.h"
#include "Engine/System/Schedule/SchedulerConfig.h"
#include "Engine/System/Schedule/SchedulerExecutionData.h"

class Scheduler
{
	private:
		const static std::int32_t m_iUnlimitedIndicator = -1;
		bool m_bSchedulerRunning;
		SchedulerExecutionData m_executionData;
		SchedulerConfig m_activeConfig;
		SchedulerConfig m_pendingConfig;
		Vector<Pair<ScheduledItem*, SchedulerItemInfo>> m_schedules;
		Vector<SchedulerListener*> m_schedulerListeners;
		std::chrono::nanoseconds m_lastLagWarning;
		std::chrono::nanoseconds m_lagWarningInterval;

		/**
		Retrieves the current time in nanoseconds.
		@return The current time in nanoseconds */
		std::chrono::nanoseconds getTimeNanos();

		/**
		Resets the scheduler execution data. */
		void resetExecutionData();

	protected:

	public:
		/**
		Constructs a scheduler with a default configuration. */
		Scheduler();

		/**
		Constructs a scheduler with the given configuration.
		@param conf The config */
		Scheduler(const SchedulerConfig& conf);

		/**
		Sets the scheduler config. Note that the configuration will not be applied until the
		scheduler is restarted.
		@param config The new configuration */
		void setConfig(const SchedulerConfig& config);

		/**
		Returns the schedulers current configuration. 
		@return A scheduler config */
		const SchedulerConfig& getActiveConfig() const;

		/**
		Returns the schedulers pending configuration. 
		@return A scheduler config */
		const SchedulerConfig& getPendingConfig() const;

		/**
		Retrieves the schedulers execution data. 
		@return The scheduler excution data */
		const SchedulerExecutionData& getExecutionData() const;

		/**
		Starts the scheduler and applies the pending config. Execution data is reset. This function 
		will not return until the scheduler is stopped. */
		void start();

		/**
		Stops the scheduler after the current update frame concludes. This function is NOT thread
		safe. */
		void stop();

		/**
		Adds a scheduled item.
		@param pItem A pointer to the item to add
		@param updateRate The rate at which the scheduled item should be updated */
		void addScheduledItem(ScheduledItem* const pItem, const SchedulerRate& updateRate);

		/**
		Removes a scheduled item. If the item did not exist, no action is taken.
		@param pItem A pointer to the item to remove */
		void removeScheduledItem(ScheduledItem* const pItem);

		/**
		Queries the existence of a scheduled item.
		@param pItem A pointer to the item to find
		@return True if the item existed, false if it did not */
		bool scheduledItemExists(ScheduledItem* const pItem) const;

		/**
		Adds a scheduler listener.
		@param pListener A pointer to the listener to add */
		void addSchedulerListener(SchedulerListener* const pListener);

		/**
		Removes a scheduler listener.
		@param pListener A pointer to the listener to remove */
		void removeSchedulerListener(SchedulerListener* const pListener);

		/**
		Queries the existence of a scheduler listener.
		@param pListener The listener to search for
		@return True if the listener existed, false if it did not */
		bool schedulerListenerExists(SchedulerListener* const pListener) const;
};

#endif