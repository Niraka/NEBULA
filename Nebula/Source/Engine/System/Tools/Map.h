/**
A standard map.

@date edited 09/01/2017
@date authored 10/11/2016

@author Nathan Sainsbury */

#ifndef MAP_H
#define MAP_H

#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/Pair.h"
#include "Engine/System/Tools/RBTree.h"
#include "Engine/System/Tools/MapKeyComparator.h"
#include "Engine/System/Tools/MapElementBuilder.h"

template <class KeyType, 
	class ValueType, 
	class Comparator = std::less<KeyType>>
class Map
{
	private:
		typedef RBTree<Pair<const KeyType, ValueType>,
			const KeyType,
			MapKeyComparator<const KeyType,
				ValueType,
				Comparator>,
			MapElementBuilder<const KeyType,
				ValueType>> Tree;
		Tree m_tree;

	protected:

	public:
		typedef typename Tree::Iterator Iterator;
		typedef typename Tree::ConstIterator ConstIterator;

		/**
		Constructs an empty map. */
		Map()
		{
			reset();
		}

		/**
		Constructs an empty map with the given maximum number of elements.
		@param uiMaxElements The maximum elements */
		Map(unsigned int uiMaxElements)
		{
			reset();
			setMaxElements(uiMaxElements);
		}

		/**
		Destructs all elements. Functionally equivalent to clear. */
		void reset()
		{
			m_tree.reset();
		}

		/**
		Destructs all elements. Functionally equivalent to reset. */
		void clear()
		{
			m_tree.clear();
		}

		/**
		Sets the maximum number of elements that may exist within the map at any one time. This
		function will not remove elements from the map should the maximum be set to less than the
		current number of elements.
		@param uiMax The maximum number of elements */
		void setMaxElements(unsigned int uiMax)
		{
			m_tree.setMaxElements(uiMax);
		}

		/**
		Retrieves the maximum number of elements that the map can contain at any one time.
		@return The maximum number of elements */
		unsigned int maxElements() const
		{
			return m_tree.maxElements();
		}

		/**
		Returns the current number of elements that the map contains.
		@return The current number of elements */
		unsigned int numElements() const
		{
			return m_tree.numElements();
		}

		/**
		Queries the existence of a key within the map.
		@param key The key to search for
		@return True if the key was found, false if it was not */
		bool exists(const KeyType& key)
		{
			return m_tree.exists(key);
		}

		/**
		Searches for and returns a pointer to a value within the map. If the key-value mapping 
		could not be found, a nullptr is returned instead.
		@param key The key to search for
		@return A pointer to a value, or a nullptr
		@see get */
		ValueType* tryToGet(const KeyType& key)
		{
			Pair<KeyType, ValueType>* pRet = m_tree.tryToGet(key);
			if (notnullptr(pRet))
			{
				return &pRet->second;
			}
			else
			{
				return nullptr;
			}
		}

		/**
		Searches for and returns a reference to a value within the map. If the key-value mapping
		did not exist it will be inserted with a default value. If the default insertion failed
		undefined behaviour will be invoked.
		@param key The key to search for
		@return A reference to a value, or undefined
		@see exists
		@see tryToGet */
		ValueType& get(const KeyType& key)
		{
			return m_tree.get(key).second;
		}

		/**
		Searches for and returns a reference to a value within the map. If the key-value mapping
		did not exist it will be inserted with a default value. If the default insertion failed
		undefined behaviour will be invoked.
		@param key The key to search for
		@return A reference to a value, or undefined
		@see exists
		@see tryToGet */
		ValueType& operator[](const KeyType& key)
		{
			return m_tree.get(key).second;
		}

		/**
		Inserts a key-value mapping in to the map with a default initialised value. If the key 
		already existed or would cause the map to exceed its maximum capacity the insertion will
		fail.
		@param key The key to insert
		@return True if the pair was inserted, false if it was not
		@see insertAndGet
		@see remove */
		bool insert(const KeyType& key)
		{
			return m_tree.insert(Pair<const KeyType, ValueType>(key, ValueType()));
		}

		/**
		Inserts a key-value mapping in to the map. If the key already existed or would cause the
		map to exceed its maximum capacity the insertion will fail.
		@param key The key to insert
		@param value The value to map to the key
		@return True if the pair was inserted, false if it was not
		@see insertAndGet
		@see remove */
		bool insert(const KeyType& key, const ValueType& value)
		{
			Pair<const KeyType, ValueType> p(key, value);
			return m_tree.insert(p);
		}

		/**
		Inserts a key-value mapping in to the map with a default initialised value and returns a 
		pointer to the value. If the key already existed or would cause the map to exceed its 
		maximum capacity the insertion will fail.
		@param key The key to insert
		@return A pointer to the inserted value, or a nullptr
		@see insertAndGet
		@see remove */
		ValueType* insertAndGet(const KeyType& key)
		{
			if (m_tree.insert(Pair<const KeyType, ValueType>(key, ValueType())))
			{
				return *m_tree.get(key);
			}
			else
			{
				return nullptr;
			}
		}

		/**
		Inserts a key-value mapping in to the map and returns a pointer to the value. If the key
		already existed or would cause the map to exceed its maximum capacity the insertion will 
		fail.
		@param key The key to insert
		@param value The value to map to the key
		@return A pointer to the inserted value, or a nullptr
		@see insertAndGet
		@see remove */
		ValueType* insertAndGet(const KeyType& key, const ValueType& value)
		{
			if (m_tree.insert(Pair<const KeyType, ValueType>(key, value)))
			{
				return &m_tree.get(key);
			}
			else
			{
				return nullptr;
			}
		}

		/**
		Removes a key-value mapping from the map. If the key did not exist no action is taken.
		@param key The key to remove
		@return True if the pair was removed, false otherwise
		@see insert */
		bool remove(const KeyType& key)
		{
			return m_tree.remove(key);
		}

		/**
		Creates an iterator targetting the first element. When the vector is empty this iterator is
		equal to the iterator created via a call to end().
		@return An iterator targetting the first element in the vector
		@see end */
		Iterator begin() const
		{
			return m_tree.begin();
		}

		/**
		Creates an iterator targetting the theoretical element one past the last element in the
		vector.
		@return An iterator targetting the theoretical element one past the last element */
		Iterator end() const
		{
			return m_tree.end();
		}

		/**
		Creates a const iterator targetting the first element. When the vector is empty this
		iterator is equal to the iterator created via a call to cend().
		@return A const iterator targetting the first element in the vector */
		ConstIterator cbegin() const
		{
			return m_tree.cbegin();
		}

		/**
		Creates a const iterator targetting the theoretical element one past the last element in
		the vector.
		@return A const iterator targetting the theoretical element one past the last element */
		ConstIterator cend() const
		{
			return m_tree.cend();
		}
};

#endif