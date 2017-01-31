/**
A const forward iterator for an indexed array.

@date edited 30/01/2017
@date authored 18/12/2016

@author Nathan Sainsbury */

#ifndef INDEXED_ARRAY_CONST_ITERATOR_H
#define INDEXED_ARRAY_CONST_ITERATOR_H

#include "Engine/System/Tools/IndexedArrayEntry.h"

template <class ElementType, class IndexType>
class IndexedArrayConstIterator
{
	private:
		typedef IndexedArrayConstIterator<ElementType, IndexType> Iter;
		const IndexedArrayEntry<ElementType, IndexType>* m_pCurrent;
		const IndexedArrayEntry<ElementType, IndexType>* m_pLast;
		const IndexedArrayEntry<ElementType, IndexType>* m_pFirst;

	protected:

	public:
		/**
		Constructs an iterator targetting the given current element.
		@param pFirst A pointer to the first element
		@param pLast A pointer to the last element
		@param pCurrent A pointer to the current element */
		IndexedArrayConstIterator(
			const IndexedArrayEntry<ElementType, IndexType>* pFirst,
			const IndexedArrayEntry<ElementType, IndexType>* pLast,
			const IndexedArrayEntry<ElementType, IndexType>* pCurrent)
		{
			m_pFirst = pFirst;
			m_pLast = pLast;
			m_pCurrent = pCurrent;
		}

		/**
		Copy constructs an iterator.
		@param other The iterator to copy */
		IndexedArrayConstIterator(const Iter& other)
		{
			m_pFirst = other.m_pFirst;
			m_pLast = other.m_pLast;
			m_pCurrent = other.m_pCurrent;
		}

		/**
		Assigns an iterator.
		@param other The iterator to copy
		@return A reference to this iterator */
		Iter& operator=(const Iter& other)
		{
			m_pFirst = other.m_pFirst;
			m_pLast = other.m_pLast;
			m_pCurrent = other.m_pCurrent;

			return *this;
		}

		/**
		Destructs the iterator. */
		~IndexedArrayConstIterator()
		{
			m_pFirst = nullptr;
			m_pLast = nullptr;
			m_pCurrent = nullptr;
		}

		/**
		Moves the iterator to the next element. Integer parameter is a dummy parameter that
		indicates post-increment.
		@return A copy of the iterator */
		Iter operator++(int)
		{
			if (m_pCurrent != m_pLast)
			{
				Iter temp = *this;
				++m_pCurrent;
				while (m_pCurrent != m_pLast && !m_pCurrent->bActive)
				{
					++m_pCurrent;
				}
				return temp;
			}

			return *this;
		}

		/**
		Moves the iterator to the next element.
		@return A reference to this iterator */
		Iter& operator++()
		{
			if (m_pCurrent != m_pLast)
			{
				++m_pCurrent;
				while (m_pCurrent != m_pLast && !m_pCurrent->bActive)
				{
					++m_pCurrent;
				}
			}

			return *this;
		}

		/**
		Moves the iterator to the previous element. Integer parameter is a dummy parameter that
		indicates post-decrement.
		@return A copy of the iterator */
		Iter operator--(int)
		{
			if (m_pCurrent != m_pFirst)
			{
				Iter temp = *this;
				--m_pCurrent;
				while (m_pCurrent != m_pFirst && !m_pCurrent->bActive)
				{
					--m_pCurrent;
				}
				return temp;
			}

			return *this;
		}

		/**
		Moves the iterator to the previous element.
		@return A reference to this iterator */
		Iter& operator--()
		{
			if (m_pCurrent != m_pFirst)
			{
				--m_pCurrent;
				while (m_pCurrent != m_pFirst && !m_pCurrent->bActive)
				{
					--m_pCurrent;
				}
			}

			return *this;
		}

		/**
		Compares two iterators for equality.
		@param other The iterator to compare with
		@return True if the iterators are considered equal, false if they are not */
		bool operator==(const Iter& other)
		{
			return m_pCurrent == other.m_pCurrent;
		}

		/**
		Compares two iterators for inequality.
		@param other The iterator to compare with
		@return True if the iterators are not considered equal, false if they are */
		bool operator!=(const Iter& other)
		{
			return m_pCurrent != other.m_pCurrent;
		}

		/**
		Dereferences the currently addressed element. If the iterator is not targetting a valid
		element undefined behaviour will be invoked.
		@return A reference to an element */
		const ElementType& operator*()
		{
			return m_pCurrent->data;
		}

		/**
		Dereferences the currently addressed element. If the iterator is not targetting a valid
		element undefined behaviour will be invoked.
		@return A reference to an element */
		const ElementType* operator->()
		{
			return &(m_pCurrent->data);
		}

		/**
		Moves the iterator to the next element.  
		@return A reference to the iterator */
		Iter& next()
		{
			++m_pCurrent;
			return *this;
		}

		/**
		Moves the iterator to the previous element.  
		@return A reference to the iterator */
		Iter& prev()
		{
			--m_pCurrent;
			return *this;
		}
};

#endif