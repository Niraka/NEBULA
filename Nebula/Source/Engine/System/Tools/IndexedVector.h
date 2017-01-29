/**
An indexed vector is a varying size container that specialises in quick insertion, deletion and
iteration of versioned elements.

To summarise, an indexed vector uses Id objects that track both the version and index number of
an inserted element. The index number is used to move directly to the target index rather than
searching for an element. The version number allows the container to reuse indices while still
being able to determine which elements exist.

Ranged based operations are not implemented for this container as their usage generally indicates
an incorrect choice of container. Users should prefer id-based functionality.

Attempting to retrieve an element with a version number that does not match the current indices' 
version number is equivalent to out of bounds access.

@see Vector
@see CyclicVector

@date edited 29/01/2017
@date authored 15/09/2016

@author Nathan Sainsbury */

#ifndef INDEXED_VECTOR_H
#define INDEXED_VECTOR_H

#include <cstdint>
#include <type_traits>

#include "Engine/System/Tools/FakeParam.h"
#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/Id.h"
#include "Engine/System/Tools/IndexedVectorEntry.h"
#include "Engine/System/Tools/IndexedVectorIterator.h"
#include "Engine/System/Tools/IndexedVectorConstIterator.h"

template <class ElementType, class IndexType = std::uint32_t>
class IndexedVector
{
	private:
		IndexedVectorEntry<ElementType, IndexType>* m_pData;
		IndexType m_iNumElements;
		IndexType m_iMaxElements;
		IndexType m_iGrowth;
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
					while (!m_pData[iModifiedIndex].bActive)
					{
						--iModifiedIndex;
					}

					m_iHighestElement = iModifiedIndex;
					m_iHighestElementPlusOne = m_iHighestElement + 1;
				}
				else if (iModifiedIndex == m_iLowestElement)
				{
					while (!m_pData[iModifiedIndex].bActive)
					{
						++iModifiedIndex;
					}

					m_iLowestElement = iModifiedIndex;
				}
			}
		}

	protected:

	public:
		typedef IndexedVectorIterator<ElementType, IndexType> Iterator;
		typedef IndexedVectorConstIterator<ElementType, IndexType> ConstIterator;

		/**
		Constructs an indexed vector with a size and growth of 5 . */
		IndexedVector()
		{
			m_pData = nullptr;
			m_iMaxElements = 5;
			m_iGrowth = 5;
			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;

			reset();
		}

		/**
		Constructs an indexed vector of the given size.
		@param iCapacity The desired capacity */
		IndexedVector(IndexType iCapacity)
		{
			m_pData = nullptr;
			m_iMaxElements = iCapacity;
			m_iGrowth = 5;
			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;

			reset();
		}

		/**
		Constructs an indexed vector with the given size and growth.
		@param iCapacity The desired capacity
		@param iGrowth The growth value */
		IndexedVector(IndexType iCapacity, IndexType iGrowth)
		{
			m_pData = nullptr;
			m_iMaxElements = iCapacity;
			m_iGrowth = iGrowth;
			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;

			reset();
		}

		/**
		Copy constructs an indexed vector.
		@param vec The indexed vector to copy */
		IndexedVector(const IndexedVector& vec)
		{
			m_iGrowth = vec.m_iGrowth;
			m_iNumElements = vec.m_iNumElements;
			m_iMaxElements = vec.m_iMaxElements;
			m_iLowestElement = vec.m_iLowestElement;
			m_iHighestElement = vec.m_iHighestElement;
			m_iHighestElementPlusOne = vec.m_iHighestElementPlusOne;

			m_pData = new IndexedVectorEntry<ElementType, IndexType>[m_iMaxElements];
			for (IndexType iCurrentIndex; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = vec.m_pData[iCurrentIndex];
			}
		}

		/**
		Assignment constructs an indexed vector.
		@param vec The indexed vector to copy 
		@return An indexed vector */
		IndexedVector operator=(const IndexedVector& vec)
		{
			if (m_pData)
			{
				delete[] m_pData;
			}

			m_iGrowth = vec.m_iGrowth;
			m_iNumElements = vec.m_iNumElements;
			m_iMaxElements = vec.m_iMaxElements;
			m_iLowestElement = vec.m_iLowestElement;
			m_iHighestElement = vec.m_iHighestElement;
			m_iHighestElementPlusOne = vec.m_iHighestElementPlusOne;

			m_pData = new IndexedVectorEntry<ElementType, IndexType>[m_iMaxElements];
			for (IndexType iCurrentIndex = 0; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex] = vec.m_pData[iCurrentIndex];
			}

			return *this;
		}

		/**
		Destructs the indexed vector. All elements are deleted. */
		~IndexedVector()
		{
			if (m_pData)
			{
				delete[] m_pData;
			}
		}

		/**
		Resets the container and its entire contents. */
		void reset()
		{
			delete[] m_pData;
			m_pData = new IndexedVectorEntry<ElementType, IndexType>[m_iMaxElements];
	
			for (IndexType iCurrentIndex = 0; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex].data = ElementType();
				m_pData[iCurrentIndex].version = 1;
				m_pData[iCurrentIndex].bActive = false;
			}

			m_iNumElements = 0;
			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;
		}

		/**
		Appends an element to the vector. If the vector was full, it will expand to accomodate the
		new element. If this function fails an invalid id is returned.
		@param element The element to append
		@return The id of inserted element or an invalid id
		@see pop()
		@see setGrowth() */
		Id<IndexType> push(const ElementType& element)
		{
			IndexType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (!m_pData[iCurrentIndex].bActive)
				{
					m_pData[iCurrentIndex].bActive = true;
					m_pData[iCurrentIndex].data = element;
					++m_pData[iCurrentIndex].version;
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

					return Id<IndexType>(iCurrentIndex, m_pData[iCurrentIndex].version);
				}
			}

			reserve(m_iMaxElements + m_iGrowth);

			m_pData[iCurrentIndex].bActive = true;
			m_pData[iCurrentIndex].data = element;
			++m_pData[iCurrentIndex].version;
			++m_iNumElements;

			return Id<IndexType>(iCurrentIndex, m_pData[iCurrentIndex].version);
		}

		/**
		Pops the last element off of the vector. If the vector was empty, no action is taken. Note
		that this function only flags the element as free. Use popAndReset to actually delete the
		element. */
		void pop()
		{
			if (m_iNumElements > 0)
			{
				m_pData[m_iHighestElement].bActive = false;
				++m_pData[m_iHighestElement].version;
				--m_iNumElements;

				if (m_iNumElements == 0)
				{
					m_iLowestElement = 0;
					m_iHighestElement = 0;
					m_iHighestElementPlusOne = 0;
				}
				else
				{
					while (!m_pData[m_iHighestElement].bActive)
					{
						--m_iHighestElement;
					}
				}
			}
		}

		/**
		Pops the last element off of the vector. If the vector was empty, no action is taken. */
		void popAndReset()
		{
			if (m_iNumElements > 0)
			{
				m_pData[m_iHighestElement].bActive = false;
				m_pData[m_iHighestElement].data = ElementType();
				++m_pData[m_iHighestElement].version;
				--m_iNumElements;

				if (m_iNumElements == 0)
				{
					m_iLowestElement = 0;
					m_iHighestElement = 0;
					m_iHighestElementPlusOne = 0;
				}
				else
				{
					while (!m_pData[m_iHighestElement].bActive)
					{
						--m_iHighestElement;
					}
				}
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
				IndexedVectorEntry<ElementType, IndexType>* pDataNew = new IndexedVectorEntry<ElementType, IndexType>[iCapacity];
				IndexType iCurrentIndex = 0;
				for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
				{
					pDataNew[iCurrentIndex] = m_pData[iCurrentIndex];
				}

				delete[] m_pData;
				m_pData = pDataNew;
				m_iMaxElements = iCapacity;
				
				for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
				{
					m_pData[iCurrentIndex].data = ElementType();
					m_pData[iCurrentIndex].version = 1;
					m_pData[iCurrentIndex].bActive = false;
				}
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
		Inserts an element at the given index. 
		@param element The element to insert
		@param iIndex The index to insert at 
		@return The id of the inserted element */
		Id<IndexType> insert(const ElementType& element, IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: " 
					+ std::to_string(iIndex));
			}
			#endif

			if (iIndex < m_iMaxElements)
			{
				if (!m_pData[iIndex].bActive)
				{
					m_pData[iIndex].bActive = true;
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

				m_pData[iIndex].data = element;
				++m_pData[iIndex].version;


				return Id<IndexType>(iIndex, m_pData[iIndex].version);
			}

			return Id<IndexType>::createInvalid();
		}

		/**
		Retrieves a reference to an element with the given id. If the element did not exist, this
		function invokes undefined behaviour.
		@param id The id of the element to get
		@return A reference to an element
		@see tryToGet() */
		ElementType& get(const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(id.index));
			}
			#endif

			if (m_pData[id.index].version == id.version)
			{
				return m_pData[id.index].data;
			}
			else
			{
				fatalexit("Indexed vector element did not exist. Current version: " +
					std::to_string(m_pData[id.index].version) + ". Given version: "
					+ std::to_string(id.version));
			}
		}

		/**
		Retrieves a reference to an element with the given id. If the element did not exist, this
		function invokes undefined behaviour.
		@param id The id of the element to get
		@return A reference to an element
		@see tryToGet() */
		ElementType& operator[](const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(id.index));
			}
			#endif

			if (m_pData[id.index].version == id.version)
			{
				return m_pData[id.index].data;
			}
			else
			{
				fatalexit("Indexed vector element did not exist. Current version: " +
					std::to_string(m_pData[id.index].version) + ". Given version: "
					+ std::to_string(id.version));
			}
		}

		/**
		Retrieves a pointer to an element with the given id. If the element did not exist a nullptr
		is returned instead.
		@param id The id of the element to get
		@return A pointer to an element, or a nullptr
		@see get() */
		ElementType* tryToGet(const Id<IndexType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(id.index));
			}
			#endif

			if (m_pData[id.index].version == id.version)
			{
				return &m_pData[id.index].data;
			}
			else
			{
				return nullptr;
			}
		}

		/**
		Removes the element at the given index. If no such element existed, no action is taken. 
		Note that the elements are not actually removed and are instead just flagged as 
		available space.
		@param iIndex The index of the element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType remove(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iIndex));
			}
			#endif

			if (m_pData[iIndex].bActive)
			{
				m_pData[iIndex].bActive = false;
				++m_pData[iIndex].version;
				--m_iNumElements;

				resolveLowHighOnRemoval(iIndex);

				return (IndexType)1;
			}

			return (IndexType)0;
		}

		/**
		Removes the first instance of the given element. Note that the elements are not actually 
		removed and are instead just flagged as available space.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType remove(const ElementType& element)
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						++m_pData[iCurrentIndex].version;
						--m_iNumElements;
						
						resolveLowHighOnRemoval(iCurrentIndex);

						return (IndexType)1;
					}
				}
			}

			return (IndexType)0;
		}

		/**
		Removes all elements that compare equal to the given element. Note that the elements are 
		not actually removed and are instead just flagged as removed.
		@param element The element to remove
		@return The number of elements removed 
		@see removeAllAndReset() */
		IndexType removeAll(const ElementType& element)
		{
			IndexType iRemovalCount = 0;

			IndexType iCurrentIndex = m_iLowestElement;
			while (iCurrentIndex < m_iHighestElementPlusOne)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						++m_pData[iCurrentIndex].version;
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
		Removes and resets the element at the given index. If no such element existed, no action is
		taken.
		@param iIndex The index of the element to remove
		@return The number of elements removed, which is at most 1 */
		IndexType removeAndReset(IndexType iIndex)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (iIndex >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(iIndex));
			}
			#endif

			if (m_pData[iIndex].bActive)
			{
				m_pData[iIndex].bActive = false;
				m_pData[iIndex].data = ElementType();
				++m_pData[iIndex].version;
				--m_iNumElements;

				resolveLowHighOnRemoval(iIndex);

				return (IndexType)1;
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
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						m_pData[iCurrentIndex].data = ElementType();
						++m_pData[iCurrentIndex].version;
						--m_iNumElements;

						resolveLowHighOnRemoval(iCurrentIndex);

						return (IndexType)1;
					}
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
			IndexType iRemovalCount = 0;

			IndexType iCurrentIndex = m_iLowestElement;
			while (iCurrentIndex < m_iHighestElementPlusOne)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						m_pData[iCurrentIndex].data = ElementType();
						++m_pData[iCurrentIndex].version;
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
		Clears the container. Version counters are not reset.
		@see reset() */
		void clear()
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					m_pData[iCurrentIndex].bActive = false;
					m_pData[iCurrentIndex].data = ElementType();
				}
			}

			m_iNumElements = 0;
			m_iLowestElement = 0;
			m_iHighestElement = 0;
			m_iHighestElementPlusOne = 0;
		}

		/**
		Replaces each instance of the first given element with a copy of the second given element.
		@param first The element to replace
		@param second The element to replace with
		@return The number of elements that were replaced */
		IndexType replace(const ElementType& first, const ElementType& second)
		{
			IndexType iReplaceCount = 0;

			for (IndexType iCurrentIndex = m_iLowestElement; 
				iCurrentIndex < m_iHighestElementPlusOne; 
				++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == first)
					{
						m_pData[iCurrentIndex].data = second;
						++iReplaceCount;
					}
				}		
			}

			return iReplaceCount;
		}

		/**
		Queries the existence of an element with the given id.
		@param id The id of the element to search for
		@return True if an element with the given id existed, false otherwise */
		bool exists(const Id<IndexType>& id) const
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Out of bounds indexed vector access. Max index: " +
					std::to_string(m_iMaxElements - 1) + ". Attempted access: "
					+ std::to_string(id.index));
			}
			#endif

			if (m_pData[id.index].bActive)
			{
				if (m_pData[id.index].version == id.version)
				{
					return true;
				}
			}

			return false;
		}

		/**
		Queries the existence of an element that compares equal to the given element.
		@param element The element to search for
		@return True if at least 1 equivalent element existed, false otherwise */
		bool exists(const ElementType& element) const
		{
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
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
				if (m_pData[iCurrentIndex].bActive)
				{
					if (&m_pData[iCurrentIndex].data == pElement)
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
			for (IndexType iCurrentIndex = m_iLowestElement;
				iCurrentIndex < m_iHighestElementPlusOne;
				++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						++iCount;
					}
				}
			}

			return iCount;
		}

		/**
		Queries whether the container is empty or not.
		@return True if the container was empty, false if it was not */
		bool isEmpty() const
		{
			return m_iNumElements == 0;
		}

		/**
		Queries whether the container holds at least 1 element.
		@return True if the container held at least 1 element, false if it did not */
		bool isNotEmpty() const
		{
			return m_iNumElements != 0;
		}

		/**
		Queries whether the container is full or not.
		@return True if the container was full, false if it was not */
		bool isFull() const
		{
			return m_iNumElements == m_iMaxElements;
		}

		/**
		Queries whether the container holds less than its maximum number of elements.
		@return True if the container was not full, false if it was */
		bool isNotFull() const
		{
			return m_iNumElements != m_iMaxElements;
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
		Creates an iterator targetting the first element. When the vector is empty this iterator is
		equal to the iterator created via a call to end().
		@return An iterator targetting the first element in the vector */
		Iterator begin() const
		{
			return Iterator(&m_pData[m_iLowestElement], &m_pData[m_iHighestElementPlusOne], &m_pData[m_iLowestElement]);
		}

		/**
		Creates an iterator targetting the theoretical element one past the last element in the
		vector.
		@return An iterator targetting the theoretical element one past the last element */
		Iterator end() const
		{
			return Iterator(&m_pData[m_iLowestElement], &m_pData[m_iHighestElementPlusOne], &m_pData[m_iHighestElementPlusOne]);
		}

		/**
		Creates a const iterator targetting the first element. When the vector is empty this
		iterator is equal to the iterator created via a call to cend().
		@return A const iterator targetting the first element in the vector */
		ConstIterator cbegin() const
		{
			return ConstIterator(&m_pData[m_iLowestElement], &m_pData[m_iHighestElementPlusOne], &m_pData[m_iLowestElement]);
		}

		/**
		Creates a const iterator targetting the theoretical element one past the last element in
		the vector.
		@return A const iterator targetting the theoretical element one past the last element */
		ConstIterator cend() const
		{
			return ConstIterator(&m_pData[m_iLowestElement], &m_pData[m_iHighestElementPlusOne], &m_pData[m_iHighestElementPlusOne]);
		}
};

#endif