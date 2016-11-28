/**
A map key comparator is a class that converts a given comparator class in to a comparator
that may be attached to a map container.

@date edited 24/11/2016
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
		Compares two keys. */
		bool operator()(KeyType& key1, KeyType& key2) const
		{
			return m_comparator(key1, key2);
		}

		/**
		Compares a mapping to a key. */
		bool operator()(const Pair<KeyType, ValueType>& mapped, KeyType& key) const
		{
			return m_comparator(mapped.first, key);
		}
		
		/**
		Compares a key to a mapping. */
		bool operator()(KeyType& key, const Pair<KeyType, ValueType>& mapped) const
		{
			return m_comparator(key, mapped.first);
		}
		
		/**
		Compares two mappings */
		bool operator()(const Pair<KeyType, ValueType>& mapped1, const Pair<KeyType, ValueType>& mapped2) const
		{
			return m_comparator(mapped1.first, mapped2.first);
		}
};

#endif