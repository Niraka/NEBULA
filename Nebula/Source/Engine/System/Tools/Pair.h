/**
A standard pair.

@todo
Add convertible type template constructors similar to std::pair to allow for Pair<int, int> to 
construct from Pair<float, float> (etc, etc).

@date edited 05/01/2017
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef PAIR_H
#define PAIR_H

#include <type_traits>

template <class Type1, class Type2 = Type1>
struct Pair
{
	/**
	The first value. */
	Type1 first;

	/**
	The second value. */
	Type2 second;

	/**
	Constructs a pair with default initialised elements. */
	Pair() :
		first(Type1()),
		second(Type2())
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