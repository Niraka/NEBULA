/**
A standard pair.

@date edited 25/11/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef PAIR_H
#define PAIR_H

template <class Type1, class Type2>
struct Pair
{
	/**
	The first value. */
	Type1 first;

	/**
	The second value. */
	Type2 second;

	/**
	Constructs a pair with uninitialised elements. */
	Pair()
	{
	}

	/**
	Constructs a pair with given values.
	@param first The first elemet
	@param second The second element */
	Pair(const Type1& first, const Type2& second) :
		first(first), 
		second(second)
	{
	}

	/**
	Copy-constructs a pair with the given values.
	@param pair The pair to copy */
	Pair(const Pair<Type1, Type2>& pair) :
		first(pair.first),
		second(pair.second)
	{
	}
};

#endif