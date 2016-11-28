/**
An element builder for bridging the gap between a Map and RBTree.

@date edited 24/11/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef MAP_ELEMENT_BUILDER_H
#define MAP_ELEMENT_BUILDER_H

#include "Engine/System/Tools/Pair.h"

template <class KeyType,
	class ValueType>
class MapElementBuilder
{
	private:
	protected:
	public:
		/**
		Constructs and returns a pair of types keytype, valuetype.
		@param key The key to build with
		@return A pair */
		Pair<KeyType, ValueType> build(KeyType& key)
		{
			return Pair<KeyType, ValueType>(key, ValueType());
		}
};

#endif