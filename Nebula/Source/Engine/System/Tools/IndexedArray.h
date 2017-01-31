/**
An indexed array is a fixed size container that specialises in quick insertion and deletion of
versioned elements.

To summarise, an indexed array uses Id objects that track both the version and index number of
an inserted element. The index number is used to move directly to the target index rather than
searching for an element. The version number allows the container to reuse indices whilst still
being able to determine which elements still exist.

Ranged based operations are not implemented for this container as their usage generally indicates
an incorrect choice of container. Users should prefer id-based functionality.

Attempting to retrieve an element with a version number that does not match the current indices'
version number is equivalent to out of bounds access.

Elements must be:
- Non-const
- Default constructible
- Copy assignable
- Copy constructible

@see Id
@see Array
@see TrackedArray
@see CyclicArray

@date edited 30/01/2017
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef INDEXED_ARRAY_H
#define INDEXED_ARRAY_H

#include <type_traits>

#include "Engine/System/Tools/FakeParam.h"
#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/Id.h"
#include "Engine/System/Tools/IndexedArrayEntry.h"
#include "Engine/System/Tools/IndexedArrayIterator.h"
#include "Engine/System/Tools/IndexedArrayConstIterator.h"

template <class ElementType, class IndexType, IndexType m_iMaxElements>
class IndexedArray
{
	private:
		IndexedArrayEntry<ElementType, IndexType> m_data[m_iMaxElements];
		IndexType m_iNumElements;
		IndexType m_iLowestElement;
		IndexType m_iHighestElement;
		IndexType m_iHighestElementPlusOne;

		/**
		Resolves the lowest and highest active element indexes upon the removal of an element.
		@param iModifiedIndex The index of the element that was removed */
		void resolveLowHighOnRemoval(IndexType iModifiedIndex)
		{
			if (m_iNumElements == 0)
			{
				m_iLowestElement = 0;
				m_iHighestElement = 0;
				m_iHighestElementPlusOne = 0;
			}
			else
			{
				if (iModifiedIndex == m_iHighestElement)
				{
					while (!m_data[iModifiedIndex].bActive)
					{
						--iModifiedIndex;
					}

					m_iHighestElement = iModifiedIndex;
					m_iHighestElementPlusOne = m_iHighestElement + 1;
				}
				else if (iModifiedIndex == m_iLowestElement)
				{
					while (!m_data[iModifiedIndex].bActive)
					{
						++iModifiedIndex;
					}

					m_iLowestElement = iModifiedIndex;
				}
			}
		}

	protected:

	public:
		typedef IndexedArrayIterator<ElementType, IndexType> Iterator;
		typedef IndexedArrayConstIterator<ElementType, IndexType> ConstIterator;

		/**
		Constructor. */
		IndexedArray()
		{
			static_assert(!std::is_const<ElementType>::value, "IndexedArray does not support "
				"const element types");
			static_assert(std::is_default_constructible<ElementType>::value, "IndexedArray "
				"requires elements to be default constructible");
			static_assert(std::is_copy_constructible<ElementType>::value, "IndexedArray "
				"requires elements to be copy constructible");
			static_assert(std::is_copy_assignable<ElementType>::value, "IndexedArray requires "
				"elements to be copy assignable");

			reset();
		}

		/**
		Destructor. */
		~IndexedArray()
		{
		}

		/**
		Resets the container. All elements are reinitialised and internal version counters are reset. */
		void reset()
		{
			for (IndexType iCurrentIndex = 0; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].data = ElementType();
				m_data[iCurrentIndex].version = 1;
				m_data[iCurrentIndex].bActive = false;
			}

			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;
			m_iNumElements = 0;
		}

		/**
		Pushes an element on to the back of the array and assigns it an id. If the container was
		full the element will not be appended and an invalid id will be returned.
		@param element The element to append to the array 
		@return The elements id, or an invalid id */
		Id<IndexType> push(const ElementType& element)
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (!m_data[iCurrentIndex].bActive)
				{
					m_data[iCurrentIndex].bActive = true;
					m_data[iCurrentIndex].data = element;
					++m_data[iCurrentIndex].version;
					++m_iNumElements;

					if (m_iNumElements == 1)
					{
						m_iHighestElementPlusOne = 1;
					}
					else
					{
						if (iCurrentIndex > m_iHighestElement)
						{
							m_iHighestElement = iCurrentIndex;
							m_iHighestElementPlusOne = m_iHighestElement + 1;
						}
						if (iCurrentIndex < m_iLowestElement)
						{
							m_iLowestElement = iCurrentIndex;
						}
					}

					return Id<IndexType>(iCurrentIndex, m_data[iCurrentIndex].version);
				}
			}

			return Id<IndexType>::createInvalid();
		}

		/**
		Pops the last element off of the back of the array. If no such element existed, no action is 
		taken. Note that the element is only flagged as removed and is not actually removed. Use 
		popAndReset to actually remove the element. 
		@return True if an element was removed, false otherwise */
		bool pop()
		{
			if (m_iNumElements > 0)
			{
				m_data[m_iHighestElement].bActive = false;
				++m_data[m_iHighestElement].version;
				--m_iNumElements;

				if (m_iNumElements == 0)
				{
					m_iLowestElement = 0;
					m_iHighestElement = 0;
					m_iHighestElementPlusOne = 0;
				}
				else
				{
					while (!m_data[m_iHighestElement].bActive)
					{
						--m_iHighestElement;
					}
				}

				return true;
			}

			return false;
		}

		/**
		Pops the last element off of the back of the array. If no such element existed, no action is
		taken.
		@return True if an element was removed, false otherwise */
		bool popAndReset()
		{
			if (m_iNumElements > 0)
			{
				m_data[m_iHighestElement].bActive = false;
				m_data[m_iHighestElement].data = ElementType();
				++m_data[m_iHighestElement].version;
				--m_iNumElements;

				if (m_iNumElements == 0)
				{
					m_iLowestElement = 0;
					m_iHighestElement = 0;
					m_iHighestElementPlusOne = 0;
				}
				else
				{
					while (!m_data[m_iHighestElement].bActive)
					{
						--m_iHighestElement;
					}
				}

				return true;
			}

			return false;
		}

		/**
		Inserts an element at the given index.
		@param element The element to insert
		@param iIndex The index to insert at
		@return The id of the inserted element */
		Id<IndexType> insert(const ElementType& element, IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (iIndex < m_iMaxElements)
			{
				if (!m_data[iIndex].bActive)
				{
					m_data[iIndex].bActive = true;
					++m_iNumElements;

					if (m_iNumElements == 1)
					{
						m_iLowestElement = iIndex;
						m_iHighestElement = iIndex;
						m_iHighestElementPlusOne = m_iHighestElement + 1;
					}
					else
					{
						if (iIndex > m_iHighestElement)
						{
							m_iHighestElement = iIndex;
							m_iHighestElementPlusOne = m_iHighestElement + 1;
						}
						if (iIndex < m_iLowestElement)
						{
							m_iLowestElement = iIndex;
						}
					}
				}

				m_data[iIndex].data = element;
				++m_data[iIndex].version;
				return Id<IndexType>(iIndex, m_data[iIndex].version);
			}

			return Id<IndexType>::createInvalid();
		}

		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param id The id of the element to get
		@return A reference to an element
		@see tryToGet() */
		ElementType& operator[](const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(id.index));
			}
			#endif

			if (m_data[id.index].version == id.version)
			{
				return m_data[id.index].data;
			}
			else
			{
				fatalexit("Attempting to access non-existent indexed array element. Current version: " +
					std::to_string(m_data[id.index].version) + ". Attempted version: " +
					std::to_string(id.version));
			}
		}

		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param id The id of the element to get 
		@return A reference to an element 
		@see tryToGet() */
		ElementType& get(const Id<IndexType>& id) 
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(id.index));
			}
			#endif

			if (m_data[id.index].version == id.version)
			{
				return m_data[id.index].data;
			}
			else
			{
				fatalexit("Attempting to access non-existent indexed array element. Current version: " + 
					std::to_string(m_data[id.index].version) + ". Attempted version: " + 
					std::to_string(id.version));
			}
		}

		/**
		Retrieves a pointer to an element. If the element did not exist, a nullptr is returned instead.
		@param id The id of the element to get
		@return A pointer to an element, or a nullptr 
		@see get() */
		ElementType* tryToGet(const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(id.index));
			}
			#endif

			if (m_data[id.index].version == id.version)
			{
				return &m_data[id.index].data;
			}
			else
			{
				return nullptr;
			}
		}

		/**	
		Removes an element with the given id. Note that the element is only flagged as removed and
		is not actually removed. Use removeAndReset to actually remove the element.
		@param id The id of the element to remove 
		@return The number of elements removed, which is at most 1 */
		IndexType remove(const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(id.index));
			}
			#endif

			if (m_data[id.index].version == id.version)
			{
				++m_data[id.index].version;
				m_data[id.index].bActive = false;
				--m_iNumElements;

				resolveLowHighOnRemoval(id.index);
				return (IndexType)1;
			}

			return (IndexType)0;
		}

		/**
		Removes and resets an element with the given id.
		@param id The id of the element to remove 
		@return The number of elements removed, which is at most 1 */
		IndexType removeAndReset(const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(id.index));
			}
			#endif

			if (m_data[id.index].version == id.version)
			{
				m_data[id.index].data = ElementType();
				++m_data[id.index].version;
				m_data[id.index].bActive = false;
				--m_iNumElements;

				resolveLowHighOnRemoval(id.index);
				return (IndexType)1;
			}

			return (IndexType)0;
		}

		/**
		Removes the first element that compares equal to the given element. Note that the element
		is only flagged as removed and is not actually removed. Use removeAndReset to actually 
		remove the element.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType remove(const ElementType& element)
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].bActive = false;
						++m_data[iCurrentIndex].version;
						--m_iNumElements;

						resolveLowHighOnRemoval(iCurrentIndex);
						return (IndexType)1;
					}
				}
			}

			return (IndexType)0;
		}

		/**
		Removes and resets the first element that compares equal to the given element.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType removeAndReset(const ElementType& element)
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].data = ElementType();
						m_data[iCurrentIndex].bActive = false;
						++m_data[iCurrentIndex].version;
						--m_iNumElements;

						resolveLowHighOnRemoval(iCurrentIndex);
						return (IndexType)1;
					}
				}
			}

			return (IndexType)0;
		}

		/**
		Removes all elements that compare equal to the given element. Note that the element is only
		flagged as removed and is not actually removed. Use removeAllAndReset to actually remove 
		the element.
		@param element The element to remove
		@return The number of elements removed */
		IndexType removeAll(const ElementType& element)
		{
			IndexType iRemovalCount = 0;

			IndexType iCurrentIndex = m_iLowestElement;
			while (iCurrentIndex < m_iHighestElementPlusOne)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].bActive = false;
						++m_data[iCurrentIndex].version;
						--m_iNumElements;
						++iRemovalCount;

						resolveLowHighOnRemoval(iCurrentIndex);
					}
					else
					{
						++iCurrentIndex;
					}
				}
				else
				{
					++iCurrentIndex;
				}
			}

			return iRemovalCount;
		}

		/**
		Removes and resets all elements that compare equal to the given element.
		@param element The element to remove
		@return The number of elements removed */
		IndexType removeAllAndReset(const ElementType& element)
		{
			IndexType iRemovalCount = 0;

			IndexType iCurrentIndex = m_iLowestElement;
			while (iCurrentIndex < m_iHighestElementPlusOne)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].bActive = false;
						m_data[iCurrentIndex].data = ElementType();
						++m_data[iCurrentIndex].version;
						--m_iNumElements;
						++iRemovalCount;

						resolveLowHighOnRemoval(iCurrentIndex);
					}
					else
					{
						++iCurrentIndex;
					}
				}
				else
				{
					++iCurrentIndex;
				}
			}

			return iRemovalCount;
		}

		/**
		Clears the container. All elements are removed. Note that unlike reset, internal version 
		counters are not reset. */
		void clear()
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				m_data[iCurrentIndex].data = ElementType();
				++m_data[iCurrentIndex].version;
				m_data[iCurrentIndex].bActive = false;
			}

			m_iNumElements = 0; 
			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;
		}

		/**
		Queries the existence of an element with the given id.
		@param id The id of the element to search for
		@return True if an element existed, false if it did not */
		bool exists(const Id<IndexType>& id) const
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(id.index));
			}
			#endif

			if (m_data[id.index].version == id.version)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		/** 
		Queries the existence of an element that compares equal with the given element.
		@param element The element to search for
		@return True if the element existed, false if it did not */
		bool exists(const ElementType& element) const
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						return true;
					}
				}
			}

			return false;
		}

		/**
		Queries the existence of an element.
		@param pElement A pointer to the element to search for
		@return True if at least 1 equal element existed, false otherwise */
		bool exists(typename std::conditional<std::is_pointer<ElementType>::value, FakeParam,
			const ElementType*>::type pElement) const
		{
			// Function disabled for pointer types. Allows the user to search for a 
			// specific element instead of any element that compares equal.

			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (&m_data[iCurrentIndex].data == pElement)
					{
						return true;
					}
				}
			}

			return false;
		}

		/**
		Counts the number of times an element that compares equal with the given element exists 
		within the container.
		@param element The element to search for
		@return The number of occurences */
		IndexType count(const ElementType& element) const
		{
			IndexType iCount = 0;
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						++iCount;
					}
				}
			}

			return iCount;
		}

		/**
		Retrieves the current number of elements.
		@return The current number of elements */
		IndexType numElements() const
		{
			return m_iNumElements;
		}

		/**
		Retrieves the maximum number of elements.
		@return The maximum number of elements */
		IndexType maxElements() const
		{
			return m_iMaxElements;
		}

		/**
		Queries whether the container is empty.
		@return True if the container is empty, false if it is not */
		bool isEmpty() const
		{
			return m_iNumElements == 0;
		}

		/**
		Queries whether the container is not empty.
		@return True if the container is not empty, false if it is not */
		bool isNotEmpty() const
		{
			return m_iNumElements != 0;
		}

		/**
		Queries whether the container is full.
		@return True if the container is full, false if it is not */
		bool isFull() const
		{
			return m_iNumElements == m_iMaxElements;
		}

		/**
		Queries whether the container is not full.
		@return True if the container is not full, false if it is */
		bool isNotFull() const
		{
			return m_iNumElements != m_iMaxElements;
		}

		/**
		Creates an iterator targetting the first element in the array.
		@return An iterator targetting the first element in the array */
		Iterator begin()
		{
			return Iterator(&m_data[m_iLowestElement],
				&m_data[m_iHighestElementPlusOne],
				&m_data[m_iLowestElement]);
		}

		/**
		Creates an iterator targetting the theoretical element one past the last element in the
		array.
		@return An iterator targetting the theoretical element one past the last element */
		Iterator end()
		{
			return Iterator(&m_data[m_iLowestElement],
				&m_data[m_iHighestElementPlusOne],
				&m_data[m_iHighestElementPlusOne]);
		}

		/**
		Creates a const iterator targetting the first element in the array.
		@return A const iterator targetting the first element in the array */
		ConstIterator cbegin() const
		{
			return ConstIterator(&m_data[m_iLowestElement],
				&m_data[m_iHighestElementPlusOne],
				&m_data[m_iLowestElement]);
		}

		/**
		Creates a const iterator targetting the theoretical element one past the last element in
		the array.
		@return A const iterator targetting the theoretical element one past the last element */
		ConstIterator cend() const
		{
			return ConstIterator(&m_data[m_iLowestElement],
				&m_data[m_iHighestElementPlusOne],
				&m_data[m_iHighestElementPlusOne]);
		}
};

#endif