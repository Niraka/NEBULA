/**
An array of fixed size.

@see TrackedArray
@see IndexedArray
@see CyclicArray

@date edited 09/01/2017
@date authored 05/10/2016

@author Nathan Sainsbury */

#ifndef ARRAY_H
#define ARRAY_H

#include <type_traits>

#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/ArrayIterator.h"
#include "Engine/System/Tools/ArrayConstIterator.h"

template <class ElementType, class IndexType, IndexType m_iMaxElements>
class Array
{
	private:
		ElementType m_data[m_iMaxElements];

	protected:

	public:
		typedef ArrayIterator<ElementType> Iterator;
		typedef ArrayConstIterator<ElementType> ConstIterator;

		/**
		Constructor. */
		Array()
		{
			reset();
		}

		/**
		Destructor. */
		~Array()
		{
		}

		/**
		Resets the container. All elements are reinitialised. */
		void reset()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex] = ElementType();
			}
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
				fatalexit("Out of bounds array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			m_data[iIndex] = element;
		}

		/**
		Retrieves a reference to an element.
		@param iIndex The index to access
		@return A reference to an element */
		ElementType& operator[](IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			return m_data[iIndex];
		}

		/**
		Retrieves a reference to an element.
		@param iIndex The index to access
		@return A reference to an element */
		ElementType& get(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			return m_data[iIndex];
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
				fatalexit("Out of bounds array access. Max index: " +
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
				m_data[iCurrentIndex] = ElementType();
				++iNumRemoved;
			}
			return iNumRemoved;
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
				fatalexit("Out of bounds array access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			m_data[iIndex] = ElementType();
			return (IndexType)1;
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
				if (m_data[iCurrentIndex] == element)
				{
					m_data[iCurrentIndex] = ElementType();
					return (IndexType)1;
				}
			}

			return (IndexType)0;
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
				if (m_data[iCurrentIndex] == element)
				{
					m_data[iCurrentIndex] = ElementType();
					++iNumRemoved;
				}
			}

			return iNumRemoved;
		}

		/**
		Clears the container. All elements are reset. This is functionally equivalent to calling 
		reset. */
		void clear()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_data[iCurrentIndex] = ElementType();
			}
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
				if (m_data[iCurrentIndex] == first)
				{
					m_data[iCurrentIndex] = second;
					++iNumReplaced;
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
				fatalexit("Out of bounds array access. Max index: " +
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
				if (m_data[iCurrentIndex] == first)
				{
					m_data[iCurrentIndex] = second;
					++iNumReplaced;
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
				m_data[iCurrentIndex] = element;
			}
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
				fatalexit("Out of bounds array access. Max index: " +
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
				m_data[iCurrentIndex] = element;
			}
		}

		/**
		Queries the existence of an element that compares equal with the given element.
		@param element The element to search for
		@return True if at least 1 equal element existed, false otherwise */
		bool exists(const ElementType& element) const
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex] == element)
				{
					return true;
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
				if (&m_data[iCurrentIndex] == pElement)
				{
					return true;
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
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_data[iCurrentIndex] == element)
				{
					++iCount;
				}
			}

			return iCount;
		}

		/**
		Retrieves the maximum number of elements.
		@return The maximum number of elements */
		IndexType maxElements() const
		{
			return m_iMaxElements;
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