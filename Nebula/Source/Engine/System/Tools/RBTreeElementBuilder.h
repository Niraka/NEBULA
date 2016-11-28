/**
A default implementation for the construction of elements that are inserted in to red black trees.

A replacement for this class is really only needed when the ElementType cannot be copy-constructed
from an instance of the KeyType.

@date edited 24/11/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef RB_TREE_ELEMENT_BUILDER_H
#define RB_TREE_ELEMENT_BUILDER_H

template <class ElementType, 
	class KeyType = ElementType>
class RBTreeElementBuilder
{
	private:
	protected:
	public:
		/**
		Constructs and returns an element. 
		@param key The key to build from
		@return An element */
		ElementType build(KeyType& key)
		{
			return ElementType(key);
		}
};

#endif