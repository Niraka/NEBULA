/**
A cyclic array is an extension upon a tracked array that tracks an active index.

The active index can be modified with calls to next(), previous() and reset() and can be used to
retrieve a particular element via a parameter-less call to get().

@see Array
@see TrackedArray
@see IndexedArray

@date edited 09/01/2017
@date authored 12/10/2016

@author Nathan Sainsbury */

#ifndef CYCLIC_ARRAY_H
#define CYCLIC_ARRAY_H

#include <type_traits>

#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/Pair.h"
#include "Engine/System/Tools/CyclicArrayIterator.h"
#include "Engine/System/Tools/CyclicArrayConstIterator.h"

template <class ElementType, class IndexType, IndexType m_iMaxElements>
class CyclicArray
{
	private:
		IndexType m_iNumElements;
		IndexType m_iActiveIndex;
		Pair<ElementType, bool> m_data[m_iMaxElements];

		/**
		Validates and resolves if necessary the current active index. */
		void validateActiveIndex()
		{
			if (m_data[m_iActiveIndex].second == false)
			{
				if (m_iNumElements == 0)
				{
					m_iActiveIndex = 0;
					return;
				}

				IndexType iCurrentIterations = 0;
				for (; iCurrentIterations < m_iMaxElements; ++iCurrentIterations)
				{
					// Cycle index
					if (m_iActiveIndex == 0)
					{
						m_iActiveIndex = (m_iMaxElements - 1);
					}
					else
					{
						--m_iActiveIndex;
					}

					// Check validity
					if (m_data[m_iActiveIndex].second)
					{
						return;
					}
				}
			}
		}

	protected:

	public:
		typedef CyclicArrayIterator<ElementType> Iterator;
		typedef CyclicArrayConstIterator<ElementType> ConstIterator;

		/**
		Constructor. */
		CyclicArray()
		{
			reset();
		}

		/**
		Destructor. */
		~CyclicArray()
		{
		}

		/**
		Moves the active index to the next element. If the container contained either 0 or 1
		elements, this function does nothing. */
		void next()
		{
			if (m_iNumElements < 2)
			{
				return;
			}

			IndexType iCurrentIterations = 0;
			for (; iCurrentIterations < m_iMaxElements; ++iCurrentIterations)
			{
				// Advance index
				if (m_iActiveIndex == (m_iMaxElements - 1))
				{
					m_iActiveIndex = 0;
				}
				else
				{
					++m_iActiveIndex;
				}

				// Check validity
				if (m_data[m_iActiveIndex].second)
				{
					return;
				}
			}
		}

		/**
		Moves the active index to the previous element. If the container contained either 0 or
		1 elements, this function does nothing. */
		void previous()
		{
			if (m_iNumElements < 2)
			{
				return;
			}

			IndexType iCurrentIterations = 0;
			for (; iCurrentIterations < m_iMaxElements; ++iCurrentIterations)
			{
				// Advance index
				if (m_iActiveIndex == 0)
				{
					m_iActiveIndex = (m_iMaxElements - 1);
				}
				else
				{
					--m_iActiveIndex;
				}

				// Check validity
				if (m_data[m_iActiveIndex].second)
				{
					return;
				}
			}
		}

		/**
		Retrieves the element at the current index. If the container was empty this function will
		invoke undefined behaviour. 
		@return A reference to the element at the active index */
		ElementType& get()
		{
			if (m_data[m_iActiveIndex].second)
			{
				return m_data[m_iActiveIndex].first;
			}
			else
			{
				fatalexit("Attempting to access non-existent cyclic array element");
			}
		}

		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param iIndex The index of the element to access
		@return A reference to an element
		@see tryToGet */
		ElementType& get(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (m_data[iIndex].second)
			{
				return m_data[iIndex].first;
			}
			else
			{
				fatalexit("Attempting to access non-existent cyclic array element");
			}
		}
		
		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param iIndex The index of the element to access
		@return A reference to an element
		@see tryToGet */
		ElementType& operator[](IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (m_data[iIndex].second)
			{
				return m_data[iIndex].first;
			}
			else
			{
				fatalexit("Attempting to access non-existent cyclic array element");
			}
		}

		/**
		Retrieves a pointer to an element. If the element did not exist, a nullptr is returned
		instead.
		@param iIndex The index of the element to access
		@return A pointer to an element, or a nullptr
		@see get */
		ElementType* tryToGet(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (m_data[iIndex].second)
			{
				return &m_data[iIndex].first;
			}
			else
			{
				return nullptr;
			}
		}

		/**
		Retrieves the active index. For empty containers this will be equal to 0.
		@return The active index */
		IndexType getActiveIndex() const
		{
			return m_iActiveIndex;
		}

		/**
		Resets the container. All elements are reinitialised. The active index becomes 0. */
		void reset()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].first = ElementType();
				m_data[iCurrentIndex].second = false;
			}

			m_iNumElements = 0;
			m_iActiveIndex = 0;
		}

		/**
		Pushes an element on to the back of the array. If the container was	full the element will
		not be appended.
		@param element The element to append to the array
		@return True if an element was appended, false otherwise */
		bool push(const ElementType& element)
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (!m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].second = true;
					m_data[iCurrentIndex].first = element;
					++m_iNumElements;

					return true;
				}
			}

			return false;
		}

		/**
		Pops the last element off of the back of the array. If no such element existed, no action
		is taken. Note that the element is only flagged as removed and is not actually removed. Use
		popAndReset to actually remove the element.
		@return True if an element was removed, false otherwise */
		bool pop()
		{
			IndexType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].second = false;
					--m_iNumElements;

					validateActiveIndex();
					return true;
				}
			} while (iCurrentIndex != 0);

			return false;
		}

		/**
		Pops the last element off of the back of the array. If no such element existed, no action is
		taken.
		@return True if an element was removed, false otherwise */
		bool popAndReset()
		{
			IndexType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].second = false;
					m_data[iCurrentIndex].first = ElementType();
					--m_iNumElements;

					validateActiveIndex();
					return true;
				}
			} while (iCurrentIndex != 0);

			return false;
		}

		/**
		Inserts an element at the given index.
		@param element The element to insert
		@param iIndex The index to insert at */
		void insert(const ElementType& element, IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (!m_data[iIndex].second)
			{
				m_data[iIndex].second = true;
				++m_iNumElements;
			}

			m_data[iIndex].first = element;
			validateActiveIndex();
		}

		/**
		Removes all elements in the given range. Note that the element is only flagged as removed
		and is not actually removed. Use removeRangeAndReset to actually remove the elements. End
		index must be greater than the start index.
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IndexType removeRange(IndexType iStartIndex, IndexType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IndexType iNumRemoved = 0;
			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].second = false;
					++iNumRemoved;
					--m_iNumElements;
				}
			}

			validateActiveIndex();
			return iNumRemoved;
		}

		/**
		Removes and resets all elements in the given range. End index must be greater than the start
		index.
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IndexType removeRangeAndReset(IndexType iStartIndex, IndexType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IndexType iNumRemoved = 0;
			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].first = ElementType();
					m_data[iCurrentIndex].second = false;
					++iNumRemoved;
					--m_iNumElements;
				}
			}

			validateActiveIndex();
			return iNumRemoved;
		}

		/**
		Removes and resets the element at the given index. If no such element existed, no action is
		taken. Note that the element is only flagged as removed and is not actually removed. Use
		removeAndReset to actually remove the element.
		@param iIndex The index of the element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType remove(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (m_data[iIndex].second)
			{
				m_data[iIndex].second = false;
				--m_iNumElements;
				validateActiveIndex();
				return (IndexType)1;
			}

			return (IndexType)0;
		}

		/**
		Removes and resets the element at the given index. If no such element existed, no action is
		taken.
		@param iIndex The index of the element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType removeAndReset(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (m_data[iIndex].second)
			{
				m_data[iIndex].first = ElementType();
				m_data[iIndex].second = false;
				--m_iNumElements;
				validateActiveIndex();
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
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == element)
					{
						m_data[iCurrentIndex].second = false;
						--m_iNumElements;
						validateActiveIndex();
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
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == element)
					{
						m_data[iCurrentIndex].first = ElementType();
						m_data[iCurrentIndex].second = false;
						--m_iNumElements;
						validateActiveIndex();
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
			IndexType iNumRemoved = 0;
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == element)
					{
						m_data[iCurrentIndex].second = false;
						++iNumRemoved;
						--m_iNumElements;
					}
				}
			}

			validateActiveIndex();
			return iNumRemoved;
		}

		/**
		Removes and resets all elements that compare equal to the given element.
		@param element The element to remove
		@return The number of elements removed */
		IndexType removeAllAndReset(const ElementType& element)
		{
			IndexType iNumRemoved = 0;
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == element)
					{
						m_data[iCurrentIndex].first = ElementType();
						m_data[iCurrentIndex].second = false;
						++iNumRemoved;
						--m_iNumElements;
					}
				}
			}

			validateActiveIndex();
			return iNumRemoved;
		}

		/**
		Clears the container. All elements are reinitialised. This is functionally equivalent to 
		calling reset. */
		void clear()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].first = ElementType();
				m_data[iCurrentIndex].second = false;
			}

			m_iNumElements = 0;
			m_iActiveIndex = 0;
		}

		/**
		Replaces each instance of the first given element with a copy of the second given element.
		@param first The element to replace
		@param second The element to replace with
		@return The number of elements that were replaced */
		IndexType replace(const ElementType& first, const ElementType& second)
		{
			IndexType iNumReplaced = 0;
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == first)
					{
						m_data[iCurrentIndex].first = second;
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
		IndexType replaceRange(const ElementType& first, const ElementType& second,
			IndexType iStartIndex, IndexType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IndexType iNumReplaced = 0;
			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == first)
					{
						m_data[iCurrentIndex].first = second;
						++iNumReplaced;
					}
				}
			}

			return iNumReplaced;
		}

		/**
		Fills the container with the given element.
		@param element The element to fill with */
		void fill(const ElementType& element)
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].first = element;
				m_data[iCurrentIndex].second = true;
			}

			m_iNumElements = m_iMaxElements;
		}

		/**
		Fills a range of indices with the given element. End index must be greater than the start
		index.
		@param element The element to fill with
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive) */
		void fillRange(const ElementType& element, IndexType iStartIndex, IndexType iEndIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iEndIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iEndIndex));
			}
			if (iStartIndex >= iEndIndex)
			{
				fatalexit("Invalid range parameters. End index must be greater than start index. " 
					"Start: " + std::to_string(iStartIndex) + ". End: " + std::to_string(iEndIndex));
			}
			#endif

			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].first = element;
				if (!m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].second = true;
					++m_iNumElements;
				}
			}

			validateActiveIndex();
		}

		/**
		Queries the existence of an element at the given index.
		@param iIndex The index to query
		@return True if an element existed at the index, false otherwise */
		bool exists(IndexType iIndex) const
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds cyclic array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			return m_data[iIndex].second;
		}

		/**
		Queries the existence of an element that compares equal to the given element.
		@param element The element to search for
		@return True if at least 1 of the element existed, false otherwise */
		bool exists(const ElementType& element) const
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == element)
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

			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (&m_data[iCurrentIndex].first == pElement)
					{
						return true;
					}
				}
			}

			return false;
		}

		/**
		Counts the number of times an element that compares equal to the given element exists
		within the container.
		@param element The element to search for
		@return The number of occurences */
		IndexType count(const ElementType& element) const
		{
			IndexType iCount = 0;
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex].second)
				{
					if (m_data[iCurrentIndex].first == element)
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