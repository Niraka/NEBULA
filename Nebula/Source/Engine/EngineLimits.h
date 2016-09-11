/**
An engine limits structure is used during engine initialisation to specify a series of properties
regarding the desired configuration of the engines components.

@date edited 10/09/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef ENGINE_LIMITS_H
#define ENGINE_LIMITS_H

struct EngineLimits
{
	/**
	Constructs a set of limits. */
	EngineLimits();

	/**
	Sets all limit properties to their default values. */
	void setDefaults();

	/**
	Queries whether the current limits are valid or not.
	@return True if the limits were valid, false otherwise */
	bool isValid() const;
};

#endif
