/**
An engine limits structure is used during engine initialisation to specify a series of properties
regarding the desired configuration of the engines components.

@date edited 31/01/2017
@date authored 10/09/2016

@author Nathan Sainsbury */

#ifndef ENGINE_LIMITS_H
#define ENGINE_LIMITS_H

struct EngineLimits
{
	/**
	Constructs a default engine limits structure. */
	EngineLimits()
	{
		setToDefaults();
	}

	/**
	Sets all fields to their default values. */
	void setToDefaults()
	{
	}

	/**
	Queries whether the current limits are valid or not.
	@return True if the limits were valid, false otherwise */
	bool isValid() const
	{
		return true;
	}
};

#endif
