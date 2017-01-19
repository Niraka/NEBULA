/**
A map key comparator is a class that converts a given comparator class in to a comparator
that may be attached to a map container.

@date edited 19/01/2017
@date authored 16/11/2016

@author Nathan Sainsbury */

#ifndef MAP_KEY_COMPARATOR_H
#define MAP_KEY_COMPARATOR_H

#include "Engine/System/Tools/Pair.h"
#include <functional>

template <class KeyType, class ValueType, class Comparator = std::less<KeyType>>
class MapKeyComparator
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
		Compares two keys for equality.
		@param key1 A mapping containing first key
		@param key2 The second key
		@return True if the keys are equal, false if they are not */
		bool operator()(const Pair<KeyType, ValueType>& mapped, KeyType& key) const
		{
			return m_comparator(mapped.first, key);
		}

		/**
		Compares two keys for equality.
		@param key1 The first key
		@param key2 A mapping containing second key
		@return True if the keys are equal, false if they are not */
		bool operator()(KeyType& key, const Pair<KeyType, ValueType>& mapped) const
		{
			return m_comparator(key, mapped.first);
		}

		/**
		Compares two keys for equality.
		@param key1 A mapping containing the first key
		@param key2 A mapping containing the second key
		@return True if the keys are equal, false if they are not */
		bool operator()(const Pair<KeyType, ValueType>& mapped1, const Pair<KeyType, ValueType>& mapped2) const
		{
			return m_comparator(mapped1.first, mapped2.first);
		}

		/**
		Compares the address of two keys for equality.
		@param pKey1 A pointer to a key
		@param pKey2 A pointer to a mapping containing the key
		@return True if the keys are equal, false if they are not */
		bool operator()(KeyType* pKey, const Pair<KeyType, ValueType>* pMapped) const
		{
			return pKey == &(pMapped->first);
		}
};

#endif