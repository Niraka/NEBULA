/**
An indexed vector is a varying size container that specialises in quick insertion, deletion and
iteration of versioned elements.

To summarise, an indexed vector uses Id objects that track both the version and index number of
an inserted element. The index number is used to move directly to the target index rather than
searching for an element. The version number allows the container to reuse indices while still
being able to determine which elements exist.

Ranged based operations are not implemented for this container as their usage generally indicates
an incorrect choice of container. Users should prefer id-based functionality.

@see Vector
@see CyclicVector

@date edited 05/01/2017
@date authored 15/09/2016

@author Nathan Sainsbury */

#ifndef INDEXED_VECTOR_H
#define INDEXED_VECTOR_H

#include "Engine/EngineBuildConfig.h"
#include "Engine/System/Tools/Id.h"
#include "Engine/System/Tools/IndexedVectorEntry.h"
#include "Engine/System/Tools/IndexedVectorIterator.h"
#include "Engine/System/Tools/IndexedVectorConstIterator.h"

template <class ElementType, class IdType = unsigned int>
class IndexedVector
{
	private:
		IndexedVectorEntry<ElementType, IdType>* m_pData;
		IdType m_iNumElements;
		IdType m_iMaxElements;
		IdType m_iGrowth;

	protected:

	public:
		typedef IndexedVectorIterator<ElementType, IdType> Iterator;
		typedef IndexedVectorConstIterator<ElementType, IdType> ConstIterator;

		/**
		Constructs an indexed vector with a size and growth of 5 . */
		IndexedVector()
		{
			m_pData = nullptr;
			m_iMaxElements = 5;
			m_iGrowth = 5;

			reset();
		}

		/**
		Constructs an indexed vector of the given size.
		@param iCapacity The desired capacity */
		IndexedVector(IdType iCapacity)
		{
			m_pData = nullptr;
			m_iMaxElements = iCapacity;
			m_iGrowth = 5;

			reset();
		}

		/**
		Constructs an indexed vector with the given size and growth.
		@param iCapacity The desired capacity
		@param iGrowth The growth value */
		IndexedVector(IdType iCapacity, IdType iGrowth)
		{
			m_pData = nullptr;
			m_iMaxElements = iCapacity;
			m_iGrowth = iGrowth;

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

			m_pData = new IndexedVectorEntry<ElementType, IdType>[m_iMaxElements];
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
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

			m_pData = new IndexedVectorEntry<ElementType, IdType>[m_iMaxElements];
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
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
		Resets the container. All elements are removed and internal version counters are reset. The
		capacity/maximum elements remains the same. */
		void reset()
		{
			delete[] m_pData;
			m_pData = new IndexedVectorEntry<ElementType, IdType>[m_iMaxElements];

			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex].data = ElementType();
				m_pData[iCurrentIndex].version = 1;
				m_pData[iCurrentIndex].bActive = false;
			}

			m_iNumElements = 0;
		}

		/**
		Appends an element to the vector. If the vector was full, it will expand to accomodate the
		new element. If this function fails an invalid id is returned.
		@param element The element to append
		@return The id of inserted element or an invalid id
		@see pop
		@see setGrowth */
		Id<IdType> push(const ElementType& element)
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (!m_pData[iCurrentIndex].bActive)
				{
					m_pData[iCurrentIndex].bActive = true;
					m_pData[iCurrentIndex].data = element;
					++m_pData[iCurrentIndex].version;
					++m_iNumElements;

					return Id<IdType>(iCurrentIndex, m_pData[iCurrentIndex].version);
				}
			}

			reserve(m_iMaxElements + m_iGrowth);

			m_pData[iCurrentIndex].bActive = true;
			m_pData[iCurrentIndex].data = element;
			++m_pData[iCurrentIndex].version;
			++m_iNumElements;

			return Id<IdType>(iCurrentIndex, m_pData[iCurrentIndex].version);
		}

		/**
		Pops the last element off of the vector. If the vector was empty, no action is taken. Note
		that this function only flags the element as free. Use popAndReset to actually delete the
		element. */
		void pop()
		{
			IdType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_pData[iCurrentIndex].bActive)
				{
					m_pData[iCurrentIndex].bActive = false;
					++m_pData[iCurrentIndex].version;
					--m_iNumElements;

					return true;
				}
			} 
			while (iCurrentIndex != 0);

			return false;
		}

		/**
		Pops the last element off of the vector. If the vector was empty, no action is taken. */
		void popAndReset()
		{
			IdType iCurrentIndex = m_iMaxElements;
			do
			{
				--iCurrentIndex;

				if (m_pData[iCurrentIndex].bActive)
				{
					m_pData[iCurrentIndex].bActive = false;
					m_pData[iCurrentIndex].data = ElementType();
					++m_pData[iCurrentIndex].version;
					--m_iNumElements;

					return true;
				}
			} 
			while (iCurrentIndex != 0);

			return false;
		}

		/**
		Reserves memory for at least the given number of elements. If the target capacity is less
		than the current maximum elements, no action is taken.
		@param iCapacity The desired capacity */
		void reserve(IdType iCapacity)
		{
			if (m_iMaxElements < iCapacity)
			{
				IndexedVectorEntry<ElementType, IdType>* pDataNew = new IndexedVectorEntry<ElementType, IdType>[iCapacity];
				IdType iCurrentIndex = 0;
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
		void setGrowth(IdType iGrowth)
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
		IdType getGrowth() const
		{
			return m_iGrowth;
		}

		/**
		Inserts an element at the given index. If the index is greater than the maximum number of
		elements, this function does nothing.
		@param element The element to insert
		@param iIndex The index to insert at 
		@return The id of the inserted element */
		Id<IdType> insert(const ElementType& element, IdType iIndex)
		{
			if (iIndex < m_iMaxElements)
			{
				if (!m_pData[iIndex].bActive)
				{
					m_pData[iIndex].bActive = true;
					++m_iNumElements;
				}

				m_pData[iIndex].data = element;
				++m_pData[iIndex].version;

				return Id<IdType>(iIndex, m_pData[iIndex].version);
			}

			return Id<IdType>::createInvalid();
		}

		/**
		Retrieves a reference to an element with the given id. If the element did not exist, this
		function invokes undefined behaviour. Note that this function ignores the version component
		of the id.
		@param id The id of the element to get
		@return A reference to an element
		@see tryToGet */
		ElementType& get(const Id<IdType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Attempting to access non-existant index in indexed vector");
			}
			#endif

			return m_pData[id.index];
		}

		/**
		Retrieves a pointer to an element with the given id. If the element did not exist a nullptr
		is returned instead.
		@param id The id of the element to get
		@return A pointer to an element, or a nullptr
		@see get */
		ElementType* tryToGet(const Id<IdType>& id)
		{
			#ifdef NEB_USE_CONTAINER_CHECKS
			if (id.index >= m_iMaxElements)
			{
				fatalexit("Attempting to access non-existant index in indexed vector");
			}
			#endif

			if (m_pData[id.index].version == id.version)
			{
				return &m_pData[id.index];
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
		IdType remove(IdType iIndex)
		{
			if (m_pData[iIndex].bActive)
			{
				m_pData[iIndex].bActive = false;
				++m_pData[iIndex].version;
				--m_iNumElements;

				return (IdType)1;
			}

			return (IdType)0;
		}

		/**
		Removes the first instance of the given element. Note that the elements are not actually 
		removed and are instead just flagged as available space.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IdType remove(const ElementType& element)
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						++m_pData[iCurrentIndex].version;

						return (IdType)1;
					}
				}
			}

			return (IdType)0;
		}

		/**
		Removes all copies of the given element. Note that the elements are not actually removed
		and are instead just flagged as available space.
		@param element The element to remove
		@return The number of elements removed 
		@see removeAllAndReset */
		IdType removeAll(const ElementType& element)
		{
			IdType iRemovalCount = 0;

			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						++m_pData[iCurrentIndex].version;

						+iRemovalCount;
					}
				}
			}

			return iRemovalCount;
		}

		/**
		Removes and resets the element at the given index. If no such element existed, no action is
		taken.
		@param iIndex The index of the element to remove
		@return The number of elements removed, which is at most 1 */
		IdType removeAndReset(IdType iIndex)
		{
			if (m_pData[iIndex].bActive)
			{
				m_pData[iIndex].bActive = false;
				m_pData[iIndex].data = ElementType();
				++m_pData[iIndex].version;
				--m_iNumElements;

				return (IdType)1;
			}

			return (IdType)0;
		}

		/**
		Removes and resets the first instance of the given element.
		@param element The element to remove
		@return The number of elements removed, which is at most 1 */
		IdType removeAndReset(const ElementType& element)
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						m_pData[iCurrentIndex].data = ElementType();
						++m_pData[iCurrentIndex].version;

						return (IdType)1;
					}
				}
			}

			return (IdType)0;
		}

		/**
		Removes and resets all copies of the given element.
		@param element The element to remove
		@return The number of elements removed */
		IdType removeAllAndReset(const ElementType& element)
		{
			IdType iRemovalCount = 0;

			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex].data == element)
					{
						m_pData[iCurrentIndex].bActive = false;
						m_pData[iCurrentIndex].data = ElementType();
						++m_pData[iCurrentIndex].version;

						+iRemovalCount;
					}
				}
			}

			return iRemovalCount;
		}

		/**
		Clears the container. All elements are removed. Version counters are NOT reset.
		@see reset */
		void clear()
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iNumElements; ++iCurrentIndex)
			{
				m_pData[iCurrentIndex].bActive = false;
				m_pData[iCurrentIndex].data = ElementType();
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
			IdType iReplaceCount = 0;
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
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
		bool exists(const Id<IdType>& id) const
		{
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
		Queries the existence of the given element.
		@param element The element to search for
		@return True if at least 1 of the element existed, false otherwise */
		bool exists(const ElementType& element) const
		{
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
			{
				if (m_pData[iCurrentIndex].bActive)
				{
					if (m_pData[iCurrentIndex] == element)
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
		IdType count(const ElementType& element) const
		{
			IdType iCount = 0;
			IdType iCurrentIndex = 0;
			for (; iCurrentIndex < m_iMaxElements; ++iCurrentIndex)
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
		Creates an iterator targetting the first element. When the vector is empty this iterator is
		equal to the iterator created via a call to end().
		@return An iterator targetting the first element in the vector
		@see end */
		Iterator begin()
		{
			return Iterator(&m_pData[0]);
		}

		/**
		Creates an iterator targetting the theoretical element one past the last element in the
		vector.
		@return An iterator targetting the theoretical element one past the last element */
		Iterator end()
		{
			return Iterator(&m_pData[m_iNumElements]);
		}

		/**
		Creates a const iterator targetting the first element. When the vector is empty this
		iterator is equal to the iterator created via a call to cend().
		@return A const iterator targetting the first element in the vector */
		ConstIterator cbegin()
		{
			return ConstIterator(&m_pData[0]);
		}

		/**
		Creates a const iterator targetting the theoretical element one past the last element in
		the vector.
		@return A const iterator targetting the theoretical element one past the last element */
		ConstIterator cend()
		{
			return ConstIterator(&m_pData[m_iNumElements]);
		}
};

#endif