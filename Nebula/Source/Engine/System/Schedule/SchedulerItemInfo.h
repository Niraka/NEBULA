/**
A scheduler item info structure tracks information about a scheduled item that has been added to a
scheduler.

It is intended for internal use only.

@date edited 20/01/2017
@date authored 29/11/2016

@author Nathan Sainsbury */

#ifndef SCHEDULER_ITEM_INFO_H
#define SCHEDULER_ITEM_INFO_H

#include <chrono>

struct SchedulerItemInfo
{
	/**
	The number of nanoseconds between each update that a scheduled item should receive. */
	std::chrono::nanoseconds timestep;

	/**
	The time at which the next update frame should commence for a scheduled item. */
	std::chrono::nanoseconds timeNextFrame;

	/**
	The time at which the last update was received. */
	std::chrono::nanoseconds timeLastUpdate;
};

#endif