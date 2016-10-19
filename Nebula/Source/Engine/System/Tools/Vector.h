/**
A standard vector.

@see IndexedVector
@see CyclicVector

@date edited 25/09/2016
@date authored 15/09/2016

@author Nathan Sainsbury */

#ifndef VECTOR_H
#define VECTOR_H

#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/LanguageExtensions.h"

template <class ElementType, class IndexType>
class Vector
{
	private:
		ElementType* m_pData;
		IndexType m_iNumElements;
		IndexType m_iMaxElements;
		IndexType m_iGrowth;

	protected:

	public:
		/**
		Constructs a vector of size 5. */
		Vector()
		{
			m_pData = nullptr;
			m_iMaxElements = 5;
			m_iGrowth = 5;

			reset();
		}
		
		/**
		Constructs a vector of the given size. 
		@param iCapacity The desired capacity */
		Vector(IndexType iCapacity)
		{
			m_pData = nullptr;
			m_iMaxElements = iCapacity;
			m_iGrowth = 5;

			reset();
		}

		/**
		Copy constructs a vector. 
		@param vec The vector to copy */
		Vector(const Vector& vec)
		{
			m_iGrowth = vec.m_iGrowth;
			m_iNumElements = vec.m_iNumElements;
			m_iMaxElements = vec.m_iMaxElements;

			m_pData = new ElementType[m_iMaxElements];
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = vec.m_pData[iCurrentIndex];
			}
		}

		/**
		Assignment constructs a vector. 
		@param vec The vector to copy */
		Vector operator=(const Vector& vec)
		{
			if (m_pData)
			{
				delete[] m_pData;
			}

			m_iGrowth = vec.m_iGrowth;
			m_iNumElements = vec.m_iNumElements;
			m_iMaxElements = vec.m_iMaxElements;

			m_pData = new ElementType[m_iMaxElements];
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = vec.m_pData[iCurrentIndex];
			}

			return *this;
		}

		/**
		Destructs the vector. */
		~Vector()
		{
			if (m_pData)
			{
				delete[] m_pData;
			}
		}

		/**
		Resets the container. All elements are removed. The capacity/maximum elements remains the
		same. */
		void reset()
		{
			m_iNumElements = 0;

			if (m_pData)
			{
				delete[] m_pData;
			}
			
			m_pData = new ElementType[m_iMaxElements];
		}

		/**
		Appends an element to the vector. If the vector was full, it will expand to accomodate the
		new element.
		@param element The element to append 
		@see pop
		@see setGrowth */
		void push(const ElementType& element)
		{
			if (m_iNumElements < m_iMaxElements)
			{
				m_pData[m_iNumElements] = element;
				++m_iNumElements;
			}
			else
			{
				reserve(m_iMaxElements + m_iGrowth);
				m_pData[m_iNumElements] = element;
				++m_iNumElements;
			}
		}

		/**
		Pops the last element off of the vector. If the vector was empty, no action is taken. Note
		that this function only flags the element as free. Use popAndReset to actually delete the
		element. */
		void pop()
		{
			if (m_iNumElements > 0)
			{
				--m_iNumElements;
			}
		}	
		
		/**
		Pops the last element off of the vector. If the vector was empty, no action is taken. */
		void popAndReset()
		{
			if (m_iNumElements > 0)
			{
				m_pData[m_iNumElements - 1] = ElementType();
				--m_iNumElements;
			}
		}

		/**
		Reserves memory for at least the given number of elements. If the target capacity is less
		than the current maximum elements, no action is taken.
		@param iCapacity The desired capacity */
		void reserve(IndexType iCapacity)
		{
			if (m_iMaxElements < iCapacity)
			{
				ElementType* pDataNew = new ElementType[iCapacity];
				IndexType iCurrentIndex = 0;
				for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
				{
					pDataNew[iCurrentIndex] = m_pData[iCurrentIndex];
				}

				delete[] m_pData;
				m_pData = pDataNew;
				m_iMaxElements = iCapacity;
			}
		}

		/**
		Sets the growth value. Defaults to 5. Must be at least 1.
		@param iGrowth The growth */
		void setGrowth(IndexType iGrowth)
		{
			if (iGrowth < 1)
			{
				m_iGrowth = 1;
			}
			else
			{
				m_iGrowth = iGrowth;
			}
		}

		/**
		Gets the growth value.
		@return The growth value */
		IndexType getGrowth() const
		{
			return m_iGrowth;
		}

		/**
		Inserts an element at the given index. If the index is greater than the current number of
		elements, this function does nothing.
		@param element The element to insert
		@param iIndex The index to insert at */
		void insert(const ElementType& element, IndexType iIndex)
		{
			if (iIndex < m_iNumElements)
			{
				m_pData[iIndex] = element;
			}
		}

		/**
		Retrieves a reference to an element. If the element did not exist, this function invokes
		undefined behaviour.
		@param iIndex The index to access
		@return A reference to an element 
		@see tryToGet */
		ElementType& get(IndexType iIndex)
		{
			//#ifndef NEB_NO_CONTAINER_CHECKS
			//if (iIndex >= m_iNumElements)
			//{
			//	fatalexit("Attempting to access non-existant index in vector");
			//}
			//#endif

			return m_pData[iIndex];
		}

		/**
		Removes and resets all elements in the given range. End index must be greater than the start
		index.
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IndexType removeRange(IndexType iStartIndex, IndexType iEndIndex)
		{
			#ifndef NEB_NO_CONTAINER_CHECKS
			if (iStartIndex > iEndIndex)
			{
				return 0;
			}
			if (iEndIndex > m_iNumElements)
			{
				return 0;
			}
			#endif

			IndexType iRemovalCount = iEndIndex - iStartIndex;
			m_iNumElements -= iRemovalCount;

			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = m_pData[iCurrentIndex + iRemovalCount];
			}

			return iRemovalCount;
		}

		/**
		Removes and resets all elements in the given range. End index must be greater than the start
		index. 
		@param iStartIndex The start index (inclusive)
		@param iEndIndex The end index (exclusive)
		@return The number of elements removed */
		IndexType removeRangeAndReset(IndexType iStartIndex, IndexType iEndIndex)
		{
			#ifndef NEB_NO_CONTAINER_CHECKS
			if (iStartIndex > iEndIndex)
			{
				return 0;
			}
			if (iEndIndex > m_iNumElements)
			{
				return 0;
			}
			#endif

			IndexType iRemovalCount = iEndIndex - iStartIndex;
			m_iNumElements -= iRemovalCount;

			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = m_pData[iCurrentIndex + iRemovalCount];
			}
			iCurrentIndex = 0;
			for (; iCurrentIndex < iRemovalCount; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex + m_iNumElements] = ElementType();
			}

			return iRemovalCount;
		}

		/**
		Removes and resets the element at the given index. If no such element existed, no action is
		taken.
		@param iIndex The index of the element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType removeAndReset(IndexType iIndex)
		{	
			if ((iIndex + 1) == m_iNumElements)
			{
				--m_iNumElements;
				m_pData[iIndex] = ElementType();
				return (IndexType)1;
			}
			else if (iIndex < m_iNumElements)
			{
				--m_iNumElements;

				IndexType iCurrentIndex = iIndex;
				for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
				{
					m_pData[iCurrentIndex] = m_pData[iCurrentIndex + 1];
				}

				m_pData[m_iNumElements] = ElementType();

				return (IndexType)1;
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
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex] == element)
				{
					--m_iNumElements;

					for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
					{
						m_pData[iCurrentIndex] = m_pData[iCurrentIndex + 1];
					}

					m_pData[m_iNumElements] = ElementType();

					return (IndexType)1;
				}
			}

			return (IndexType)0;
		}

		/**
		Removes and resets all copies of the given element.
		@param element The element to remove
		@return The number of elements removed */
		IndexType removeAllAndReset(const ElementType& element)
		{
			IndexType iRemovalCount = 0;
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex] == element)
				{
					--m_iNumElements;
					++iRemovalCount;

					if (iRemovalCount > 0)
					{
						IndexType iInternalCurrentIndex = iCurrentIndex;
						for (; iInternalCurrentIndex < m_iNumElements; ++iInternalCurrentIndex)
						{
							m_pData[iInternalCurrentIndex] = m_pData[iInternalCurrentIndex + 1];
						}

						--iCurrentIndex;
					}
				}
			}

			// Reset the elements that were moved down
			IndexType iIterationCount = 0;
			for (; iIterationCount < iRemovalCount; ++iIterationCount)
			{
				m_pData[iCurrentIndex + iIterationCount] = ElementType();
			}

			return iRemovalCount;
		}

		/**
		Clears the container. All elements are removed. */
		void clear()
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = ElementType();
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
			IndexType iReplaceCount = 0;
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex] == first)
				{
					m_pData[iCurrentIndex] = second;
					++iReplaceCount;
				}
			}

			return iReplaceCount;
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
			#ifndef NEB_NO_CONTAINER_CHECKS
			if (iStartIndex > iEndIndex)
			{
				return 0;
			}
			if (iEndIndex > m_iMaxElements)
			{
				return 0;
			}
			#endif

			IndexType iReplaceCount = 0;
			IndexType iCurrentIndex = iStartIndex;
			if (iEndIndex > m_iNumElements)
			{
				m_iNumElements += iEndIndex - m_iNumElements;
			}
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex] == first)
				{
					m_pData[iCurrentIndex] = second;
					++iReplaceCount;
				}
			}

			return iReplaceCount;
		}

		/**
		Fills the container to its maximum capacity with the given element.
		@param element The element to fill with */
		void fill(const ElementType& element)
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = element;
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
			#ifndef NEB_NO_CONTAINER_CHECKS
			if (iStartIndex > iEndIndex)
			{
				return;
			}
			if (iEndIndex > m_iMaxElements)
			{
				return;
			}
			#endif

			if (iEndIndex > m_iNumElements)
			{
				m_iNumElements += iEndIndex - m_iNumElements;
			}
			IndexType iCurrentIndex = iStartIndex;
			for (; iCurrentIndex < iEndIndex; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = element;
			}
		}

		/**
		Queries the existence of the given element.
		@param element The element to search for
		@return True if at least 1 of the element existed, false otherwise */
		bool exists(const ElementType& element) const
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex] == element)
				{
					return true;
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
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex] == element)
				{
					++iCount;
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
};

#endif