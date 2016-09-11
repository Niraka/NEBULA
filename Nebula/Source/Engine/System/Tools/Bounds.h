/**
A bounds object specifies a numeric boundary using an upper and lower value.

Note that the lower bound being less than the upper bound is not enforced.

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef BOUNDS_H
#define BOUNDS_H

template <class NumericType>
struct Bounds
{
	/**
	The lower bound. */
	NumericType lower;

	/**
	The upper bound. */
	NumericType upper;
		
	/**
	Constructs a bounds object. */
	Bounds()
	{
	}

	/**
	Constructs a bounds object with the given values.
	@param lower The lower bound
	@param upper The upper bound */
	Bounds(NumericType lower, NumericType upper)
	{
		this->lower = lower;
		this->upper = upper;
	}

	/**
	Gets the range of values encompassed by the bounds. This operation is equivalent 
	to std::abs(upper - lower).
	@return The range encompased by the bounds */
	NumericType range() const
	{
		return std::abs(upper - lower);
	}
};

#endif