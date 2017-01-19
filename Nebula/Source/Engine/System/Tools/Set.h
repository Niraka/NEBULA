/**
A standard set.

@date edited 19/01/2017
@date authored 19/10/2016

@author Nathan Sainsbury */

#ifndef SET_H
#define SET_H

#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/RBTree.h"
#include "Engine/System/Tools/SetKeyComparator.h"

template <class ElementType, class Comparator = std::less<ElementType>>
class Set
{
	private:
		typedef RBTree<const ElementType, 
					   const ElementType, 
			           SetKeyComparator<const ElementType, Comparator>> Tree;
		Tree m_tree;

	protected:

	public:
		typedef typename Tree::Iterator Iterator;
		typedef typename Tree::ConstIterator ConstIterator;

		/**
		Constructs an empty set. */
		Set()
		{
			reset();
		}

		/**
		Constructs an empty set with the given maximum number of elements. 
		@param uiMaxElements The maximum elements */
		Set(unsigned int uiMaxElements)
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
		Sets the maximum number of elements that may exist within the set at any one time. This
		function will not remove elements from the set should the maximum be set to less than the
		current number of elements.
		@param uiMax The maximum number of elements */
		void setMaxElements(unsigned int uiMax)
		{
			m_tree.setMaxElements(uiMax);
		}

		/**
		Retrieves the maximum number of elements that the set can contain at any one time.
		@return The maximum number of elements */
		unsigned int maxElements() const
		{
			return m_tree.maxElements();
		}

		/**
		Returns the current number of elements that the set contains.
		@return The current number of elements */
		unsigned int numElements() const
		{
			return m_tree.numElements();
		}

		/**
		Queries the existence of an element that compares equal to the given element.
		@param element The element to search for
		@return True if the element was found, false if it was not */
		bool exists(const ElementType& element)
		{
			return m_tree.exists(element);
		}

		/**
		Queries the existence of an element.
		@param pElement A pointer to the element to search for
		@return True if the key was found, false otherwise */
		bool exists(typename std::conditional<std::is_pointer<ElementType>::value, FakeParam,
			const ElementType*>::type pElement)
		{
			return m_tree.exists(pElement);
		}

		/**
		Searches for and returns a pointer to an element within the set. If the element could not 
		be found, a nullptr is returned instead.
		@param element The element to search for
		@return A pointer to an element, or a nullptr
		@see get */
		const ElementType* tryToGet(const ElementType& element)
		{
			return m_tree.tryToGet(element);
		}

		/**
		Searches for and returns a reference to an element within the set. If the element did not
		exist it will be inserted. If the default insertion	failed undefined behaviour will be 
		invoked.
		@param element The element to search for
		@return A reference to an element, or undefined 
		@see exists
		@see tryToGet */
		const ElementType& get(const ElementType& element)
		{
			return m_tree.get(element);
		}

		/**
		Searches for and returns a reference to an element within the set. If the element did not
		exist it will be inserted. If the default insertion	failed undefined behaviour will be
		invoked.
		@param element The element to search for
		@return A reference to an element, or undefined
		@see exists
		@see tryToGet */
		const ElementType& operator[](const ElementType& element)
		{
			return m_tree.get(element);
		}

		/**
		Inserts an element in to the set. If the element already existed or would cause the set
		to exceed its maximum capacity, the element will not be inserted.
		@param element The element to insert
		@return True if the element was inserted, false if it was not 
		@see insertAndGet
		@see remove */
		bool insert(const ElementType& element)
		{
			return m_tree.insert(element);
		}
		
		/**
		Inserts an element in to the set and returns a pointer to it. If the element already 
		existed or would cause the set to exceed its maximum capacity the element will not be
		inserted.
		@param element The element to insert
		@return A pointer to the inserted element, or a nullptr
		@see insertAndGet
		@see remove */
		const ElementType* insertAndGet(const ElementType& element)
		{
			if (m_tree.insert(element))
			{
				return m_tree.tryToGet(element);
			}
			else
			{
				return nullptr;
			}
		}

		/**
		Removes an element from the set. If the element did not exist no action is taken.
		@param element The element to remove
		@return True if the element was removed, false otherwise
		@see insert */
		bool remove(const ElementType& element)
		{
			return m_tree.remove(element);
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