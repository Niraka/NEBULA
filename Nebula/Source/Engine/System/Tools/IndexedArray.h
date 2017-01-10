/**
An indexed array is a fixed size container that specialises in quick insertion and deletion of
versioned elements.

To summarise, an indexed array uses Id objects that track both the version and index number of
an inserted element. The index number is used to move directly to the target index rather than
searching for an element. The version number allows the container to reuse indices whilst still
being able to determine which elements still exist.

@see Id
@see Array
@see TrackedArray
@see CyclicArray

@date edited 09/01/2017
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef INDEXED_ARRAY_H
#define INDEXED_ARRAY_H

#include <type_traits>

#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/Id.h"
#include "Engine/System/Tools/IndexedArrayEntry.h"
#include "Engine/System/Tools/IndexedArrayIterator.h"
#include "Engine/System/Tools/IndexedArrayConstIterator.h"

template <class ElementType, class IdType, IdType m_iMaxElements>
class IndexedArray
{
	private:
		IdType m_iNumElements;
		IndexedArrayEntry<ElementType, IdType> m_data[m_iMaxElements];

	protected:

	public:
		typedef IndexedArrayIterator<ElementType, IdType> Iterator;
		typedef IndexedArrayConstIterator<ElementType, IdType> ConstIterator;

		/**
		Constructor. */
		IndexedArray()
		{
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
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].data = ElementType();
				m_data[iCurrentIndex].version = 1;
				m_data[iCurrentIndex].bActive = false;
			}

			m_iNumElements = 0;
		}

		/**
		Pushes an element on to the back of the array and assigns it an id. If the container was
		full the element will not be appended and an invalid id will be returned.
		@param element The element to append to the array 
		@return The elements id, or an invalid id */
		Id<IdType> push(const ElementType& element)
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (!m_data[iCurrentIndex].bActive)
				{
					m_data[iCurrentIndex].bActive = true;
					m_data[iCurrentIndex].data = element;
					++m_data[iCurrentIndex].version;
					++m_iNumElements;

					return Id<IdType>(iCurrentIndex, m_data[iCurrentIndex].version);
				}
			}

			return Id<IdType>::createInvalid();
		}

		/**
		Pops the last element off of the back of the array. If no such element existed, no action is 
		taken. Note that the element is only flagged as removed and is not actually removed. Use 
		popAndReset to actually remove the element. 
		@return True if an element was removed, false otherwise */
		bool pop()
		{
			IdType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_data[iCurrentIndex].bActive)
				{
					m_data[iCurrentIndex].bActive = false;
					++m_data[iCurrentIndex].version;
					--m_iNumElements;

					return true;
				}
			} 
			while (iCurrentIndex != 0);

			return false;
		}

		/**
		Pops the last element off of the back of the array. If no such element existed, no action is
		taken.
		@return True if an element was removed, false otherwise */
		bool popAndReset()
		{
			IdType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_data[iCurrentIndex].bActive)
				{
					m_data[iCurrentIndex].bActive = false;
					m_data[iCurrentIndex].data = ElementType();
					++m_data[iCurrentIndex].version;
					--m_iNumElements;

					return true;
				}
			}
			while (iCurrentIndex != 0);

			return false;
		}

		/**
		Inserts an element at the given index and assigns it an id.
		@param element The element to insert
		@param iIndex The index to insert at
		@return The elements id */
		Id<IdType> insert(const ElementType& element, IdType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (!m_data[iIndex].bActive)
			{
				m_data[iIndex].bActive = true;
				++m_iNumElements;		
			}

			m_data[iIndex].data = element;
			++m_data[iIndex].version;
			return Id<IdType>(iIndex, m_data[iIndex].version);
		}

		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param id The id of the element to get
		@return A reference to an element
		@see tryToGet */
		ElementType& operator[](const Id<IdType>& id)
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
		@see tryToGet */
		ElementType& get(const Id<IdType>& id) 
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
		@see get */
		ElementType* tryToGet(const Id<IdType>& id)
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
		Removes all elements in the given range. Note that the element is only flagged as removed
		and is not actually removed. Use removeRangeAndReset to actually remove the element. End
		index must be greater than the start index.
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IdType removeRange(IdType iStartIndex, IdType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IdType iNumRemoved = 0;
			IdType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					++m_data[iCurrentIndex].version;
					m_data[iCurrentIndex].bActive = false;
					++iNumRemoved;
					--m_iNumElements;
				}
			}

			return iNumRemoved;
		}

		/**
		Removes and resets all elements in the given range. End index must be greater than the start
		index.
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IdType removeRangeAndReset(IdType iStartIndex, IdType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IdType iNumRemoved = 0;
			IdType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					m_data[iCurrentIndex].data = ElementType();
					++m_data[iCurrentIndex].version;
					m_data[iCurrentIndex].bActive = false;
					++iNumRemoved;
					--m_iNumElements;
				}
			}

			return iNumRemoved;
		}

		/**	
		Removes an element with the given id. Note that the element is only flagged as removed and
		is not actually removed. Use removeAndReset to actually remove the element.
		@param id The id of the element to remove 
		@return The number of elements removed, which is at most 1 */
		IdType remove(const Id<IdType>& id)
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
				return (IdType)1;
			}

			return (IdType)0;
		}

		/**
		Removes and resets an element with the given id.
		@param id The id of the element to remove 
		@return The number of elements removed, which is at most 1 */
		IdType removeAndReset(const Id<IdType>& id)
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
				return (IdType)1;
			}

			return (IdType)0;
		}

		/**
		Removes the first element that compares equal to the given element. Note that the element
		is only flagged as removed and is not actually removed. Use removeAndReset to actually 
		remove the element.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IdType remove(const ElementType& element)
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].bActive = false;
						++m_data[iCurrentIndex].version;
						--m_iNumElements;
						return (IdType)1;
					}
				}
			}

			return (IdType)0;
		}

		/**
		Removes and resets the first element that compares equal to the given element.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IdType removeAndReset(const ElementType& element)
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].data = ElementType();
						m_data[iCurrentIndex].bActive = false;
						++m_data[iCurrentIndex].version;
						--m_iNumElements;
						return (IdType)1;
					}
				}
			}

			return (IdType)0;
		}

		/**
		Removes all elements that compare equal to the given element. Note that the element is only
		flagged as removed and is not actually removed. Use removeAllAndReset to actually remove 
		the element.
		@param element The element to remove
		@return The number of elements removed */
		IdType removeAll(const ElementType& element)
		{
			IdType iNumRemoved = 0;
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						++m_data[iCurrentIndex].version;
						m_data[iCurrentIndex].bActive = false;
						++iNumRemoved;
						--m_iNumElements;
					}
				}
			}

			return iNumRemoved;
		}

		/**
		Removes and resets all elements that compare equal to the given element.
		@param element The element to remove
		@return The number of elements removed */
		IdType removeAllAndReset(const ElementType& element)
		{
			IdType iNumRemoved = 0;
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == element)
					{
						m_data[iCurrentIndex].data = ElementType();
						++m_data[iCurrentIndex].version;
						m_data[iCurrentIndex].bActive = false;
						++iNumRemoved;
						--m_iNumElements;
					}
				}
			}

			return iNumRemoved;
		}

		/**
		Clears the container. All elements are removed. Note that unlike reset, internal version 
		counters are not reset. */
		void clear()
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].data = ElementType();
				++m_data[iCurrentIndex].version;
				m_data[iCurrentIndex].bActive = false;
			}

			m_iNumElements = 0;
		}

		/**
		Replaces each instance of the first given element with a copy of the second given element.
		@param first The element to replace
		@param second The element to replace with
		@return The number of elements that were replaced */
		IdType replace(const ElementType& first, const ElementType& second)
		{
			IdType iNumReplaced = 0;
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == first)
					{
						m_data[iCurrentIndex].data = second;
						++iNumReplaced;
					}
				}
			}

			return iNumReplaced;
		}

		/**
		Replaces each instance of the first given element with a copy of the second given element
		within the specified range. End index must be greater than the start index.
		@param first The element to replace
		@param second The element to replace with
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements that were replaced */
		IdType replaceRange(const ElementType& first, const ElementType& second, 
			IdType iStartIndex, IdType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IdType iNumReplaced = 0;
			IdType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].bActive)
				{
					if (m_data[iCurrentIndex].data == first)
					{
						m_data[iCurrentIndex].data = second;
						++iNumReplaced;
					}
				}
			}

			return iNumReplaced;
		}

		/**
		Queries the existence of an element with the given id.
		@param id The id of the element to search for
		@return True if an element existed, false if it did not */
		bool exists(const Id<IdType>& id) const
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
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
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
		template <class ElementType2 = std::enable_if<!std::is_pointer<ElementType>::value, ElementType>>
		bool exists(const ElementType2* pElement) const
		{
			// Function enabled if the element type is not a pointer. Allows the user
			// to search for a specific element instead of any element that compares
			// equal.

			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
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
		IdType count(const ElementType& element) const
		{
			IdType iCount = 0;
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
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
		IdType numElements() const
		{
			return m_iNumElements;
		}

		/**
		Retrieves the maximum number of elements.
		@return The maximum number of elements */
		IdType maxElements() const
		{
			return m_iMaxElements;
		}

		/**
		Checks whether the container is empty.
		@return True if it is empty, false otherwise */
		bool isEmpty() const
		{
			return m_iNumElements == 0;
		}

		/**
		Checks whether the container is not empty.
		@return True if it is not empty, false otherwise */
		bool isNotEmpty() const
		{
			return m_iNumElements != 0;
		}

		/**
		Checks whether the container is full.
		@return True if it is full, false otherwise */
		bool isFull() const
		{
			return m_iNumElements == m_iMaxElements;
		}

		/**
		Creates an iterator targetting the first element in the array.
		@return An iterator targetting the first element in the array */
		Iterator begin() const
		{
			return Iterator(&m_data[0]);
		}

		/**
		Creates an iterator targetting the theoretical element one past the last element in the
		array.
		@return An iterator targetting the theoretical element one past the last element */
		Iterator end() const
		{
			return Iterator(&m_data[m_iMaxElements]);
		}

		/**
		Creates a const iterator targetting the first element in the array.
		@return A const iterator targetting the first element in the array */
		ConstIterator cbegin() const
		{
			return ConstIterator(&m_data[0]);
		}

		/**
		Creates a const iterator targetting the theoretical element one past the last element in
		the array.
		@return A const iterator targetting the theoretical element one past the last element */
		ConstIterator cend() const
		{
			return ConstIterator(&m_data[m_iMaxElements]);
		}
};

#endif