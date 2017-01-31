/**
The system layer.

This layer is responsible for the ownership of a series of "subsystems" that provide very basic
functionality that does not directly depend on any other system.

@date edited 31/01/2017
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef SYSTEM_LAYER_H
#define SYSTEM_LAYER_H

#include <string>

#include "Engine/Layer/Layer.h"
#include "Engine/System/Schedule/Scheduler.h"

class SystemLayer : 
	public Layer
{
	private:
		Scheduler m_scheduler;

	protected:

	public:
		/**
		Constructs a system layer. */
		SystemLayer();

		/**
		Starts up the system layer.
		@return A layer response indicating the result of the operation */
		LayerResponses startLayerUp();

		/**
		Shuts down the system layer.
		@return A layer response indicating the result of the operation */
		LayerResponses shutLayerDown();

		/**
		Sets the scheduler config. Note that the configuration will not be applied until the
		scheduler is restarted.
		@param config The new configuration */
		void setSchedulerConfig(const SchedulerConfig& config);

		/**
		Returns the schedulers current configuration.
		@return A scheduler config */
		const SchedulerConfig& getSchedulerActiveConfig() const;

		/**
		Returns the schedulers pending configuration.
		@return A scheduler config */
		const SchedulerConfig& getSchedulerPendingConfig() const;

		/**
		Retrieves the schedulers execution data.
		@return The scheduler excution data */
		const SchedulerExecutionData& getSchedulerExecutionData() const;

		/**
		Starts the scheduler and applies the pending config. Execution data is reset. This function
		will not return until the scheduler is stopped. */
		void startScheduler();

		/**
		Stops the scheduler after the current update frame concludes. This function is NOT thread
		safe. */
		void stopScheduler();

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