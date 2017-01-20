/**
Scheduler intervals define a selection of human readable intervals that can be combined with an
integral value to produce a Scheduler Rate.

@date edited 19/01/2017
@date authored 19/01/2017

@author Nathan Sainsbury */

#ifndef SCHEDULER_INTERVALS_H
#define SCHEDULER_INTERVALS_H

enum class SchedulerIntervals
{
	/**
	Per hour. */
	PER_HOUR,

	/**
	Per minute. */
	PER_MINUTE,

	/**
	Per second. */
	PER_SECOND,

	/**
	Per millisecond. */
	PER_MILLISECOND
};

#endif