/**
A tracked array is a fixed size container that tracks whether elements are in use or not.

This is done by assigning a boolean to each element to determine whether the index is in use. The
current number of elements is also tracked for convenience.

@see Array
@see IndexedArray

@date edited 11/09/2016
@date authored 11/09/2016

@author Nathan Sainsbury */

#ifndef TRACKED_ARRAY_H
#define TRACKED_ARRAY_H

#include "Engine/System/Tools/Pair.h"
#include "Engine/System/Tools/LanguageExtensions.h"

template <class ElementType, class IndexType, IndexType m_iMaxElements>
class TrackedArray
{
	private:
		IndexType m_iNumElements;
		Pair<ElementType, bool> m_data[m_iMaxElements];

	protected:

	public:
		/**
		Constructor. */
		TrackedArray()
		{
			reset();
		}

		/**
		Destructor. */
		~TrackedArray()
		{
		}

		/**
		Resets the container. All elements are removed. */
		void reset()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].first = ElementType();
				m_data[iCurrentIndex].second = false;
			}

			m_iNumElements = 0;
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
			IndexType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_data[iCurrentIndex].second)
				{
					m_data[iCurrentIndex].second = false;
					m_data[iCurrentIndex].first = ElementType();
					--m_iNumElements;

					return true;
				}
			} 
			while (iCurrentIndex != 0);

			return false;
		}

		/**
		Inserts an element at the given index.
		@param element The element to insert
		@param iIndex The index to insert at */
		void insert(const ElementType& element, IndexType iIndex)
		{
			if (!m_data[iIndex].second)
			{
				m_data[iIndex].second = true;
				++m_iNumElements;
			}

			m_data[iIndex].first = element;
		}

		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param iIndex The index of the element to access
		@return A reference to an element
		@see tryToGet */
		ElementType& get(IndexType iIndex)
		{
			if (m_data[iIndex].second)
			{
				return m_data[iIndex].first;
			}
			else
			{
				fatalexit("Attempting to access non-existent tracked array element");
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
		Removes all elements in the given range. Note that the element is only flagged as removed
		and is not actually removed. Use removeRangeAndReset to actually remove the element. End 
		index must be greater than the start index.
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IndexType removeRange(IndexType iStartIndex, IndexType iEndIndex)
		{
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
			if (m_data[iIndex].second)
			{
				m_data[iIndex].second = false;
				--m_iNumElements;
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
			if (m_data[iIndex].second)
			{
				m_data[iIndex].first = ElementType();
				m_data[iIndex].second = false;
				--m_iNumElements;
				return (IndexType)1;
			}

			return (IndexType)0;
		}

		/**
		Removes the first instance of the given element. Note that the element is only flagged as
		removed and is not actually removed. Use removeAndReset to actually remove the element.
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
						return (IndexType)1;
					}
				}
			}

			return (IndexType)0;
		}
		
		/**
		Removes and resets the first instance of the given element.
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
						return (IndexType)1;
					}
				}
			}

			return (IndexType)0;
		}

		/**
		Removes all copies of the given element. Note that the element is only flagged as removed
		and is not actually removed. Use removeAllAndReset to actually remove the element.
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

			return iNumRemoved;
		}

		/**
		Removes and resets all copies of the given element. 
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

			return iNumRemoved;
		}
		
		/**
		Clears the container. All elements are removed. This is functionally equivalent to calling
		reset. */
		void clear()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex].first = ElementType();
				m_data[iCurrentIndex].second = false;
			}

			m_iNumElements = 0;
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
		}

		/**
		Queries the existence of any element at the given index.
		@param iIndex The index to query
		@return True if an element existed at the index, false otherwise */
		bool exists(IndexType iIndex) const
		{
			return m_data[iIndex].second;
		}

		/**
		Queries the existence of the given element.
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
		Counts the number of times the given element exists within the container.
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
};

#endif