/**
A standard pair.

@todo
Add convertible type template constructors similar to std::pair to allow for Pair<int, int> to 
construct from Pair<float, float> (etc, etc).

@date edited 08/01/2017
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

	/**
	Compares this pair with the given pair for equality.
	@param other The pair to compare with
	@return True if the pairs are equal, false if they are not */
	bool operator==(const Pair<Type1, Type2>& other) const
	{
		return first == other.first && second == other.second;
	}
	
	/**
	Compares this pair with the given pair for inequality.
	@param other The pair to compare with
	@return True if the pairs are not equal, false if they are */
	bool operator!=(const Pair<Type1, Type2>& other) const
	{
		return !((*this) == other);
	}
};

#endif