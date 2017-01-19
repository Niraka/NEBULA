/**
A set key comparator is a class that converts a given comparator class in to a comparator
that may be attached to a set container.

@date edited 19/01/2017
@date authored 19/01/2017

@author Nathan Sainsbury */

#ifndef SET_KEY_COMPARATOR_H
#define SET_KEY_COMPARATOR_H

#include <functional>

template <class KeyType, class Comparator = std::less<KeyType>>
class SetKeyComparator
{
	private:
		Comparator m_comparator;

	protected:

	public:
		/**
		Compares two keys for equality.
		@param key1 The first key
		@param key2 The second key
		@return True if the keys are equal, false if they are not */
		bool operator()(KeyType& key1, KeyType& key2) const
		{
			return m_comparator(key1, key2);
		}

		/**
		Compares the address of two keys for equality.
		@param pKey1 A pointer to a key
		@param pKey2 A pointer to a key
		@return True if the keys are equal, false if they are not */
		bool operator()(KeyType* pKey1, KeyType* pKey2) const
		{
			return pKey1 == pKey2;
		}
};

#endif