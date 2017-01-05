/**
A forward iterator for an indexed array.

@date edited 05/01/2017
@date authored 10/12/2016

@author Nathan Sainsbury */

#ifndef INDEXED_ARRAY_ITERATOR_H
#define INDEXED_ARRAY_ITERATOR_H

#include "Engine/System/Tools/IndexedArrayEntry.h"

template <class ElementType, class IdType>
class IndexedArrayIterator
{
	private:
		typedef IndexedArrayIterator<ElementType, IdType> Iter;
		IndexedArrayEntry<ElementType, IdType>* m_pElement;

	protected:

	public:
		/**
		Constructs an iterator targeting the given element.
		@param pElement The address of the element to target */
		IndexedArrayIterator(IndexedArrayEntry<ElementType, IdType>* pElement)
		{
			m_pElement = pElement;
		}

		/**
		Copy constructs an iterator.
		@param other The iterator to copy */
		IndexedArrayIterator(const Iter& other)
		{
			m_pElement = other.m_pElement;
		}

		/**
		Assigns an iterator.
		@param other The iterator to copy
		@return A reference to this iterator */
		Iter& operator=(const Iter& other)
		{
			m_pElement = other.m_pElement;
			return *this;
		}

		/**
		Destructs the iterator. */
		~IndexedArrayIterator()
		{
			m_pElement = nullptr;
		}

		/**
		Moves the iterator to the next element. Integer parameter is a dummy parameter that
		indicates post-increment.
		@return A copy of the iterator */
		Iter operator++(int)
		{
			Iter temp = *this;
			++m_pElement;
			return temp;
		}

		/**
		Moves the iterator to the next element.
		@return A reference to this iterator */
		Iter& operator++()
		{
			++m_pElement;
			return *this;
		}

		/**
		Moves the iterator to the previous element. Integer parameter is a dummy parameter that
		indicates post-decrement.
		@return A copy of the iterator */
		Iter operator--(int)
		{
			Iter temp = *this;
			--m_pElement;
			return temp;
		}

		/**
		Moves the iterator to the previous element.
		@return A reference to this iterator */
		Iter& operator--()
		{
			--m_pElement;
			return *this;
		}

		/**
		Compares two iterators for equality.
		@param other The iterator to compare with
		@return True if the iterators are considered equal, false if they are not */
		bool operator==(const Iter& other)
		{
			return m_pElement == other.m_pElement;
		}

		/**
		Compares two iterators for inequality.
		@param other The iterator to compare with
		@return True if the iterators are not considered equal, false if they are */
		bool operator!=(const Iter& other)
		{
			return m_pElement != other.m_pElement;
		}

		/**
		Dereferences the currently addressed element. If the iterator is not targetting a valid
		element undefined behaviour will be invoked.
		@return A reference to an element */
		ElementType& operator*()
		{
			return (*m_pElement).data;
		}

		/**
		Dereferences the currently addressed element. If the iterator is not targetting a valid
		element undefined behaviour will be invoked.
		@return A reference to an element */
		ElementType* operator->()
		{
			return &(m_pElement->data);
		}

		/**
		Moves the iterator to the next element. */
		Iter& next()
		{
			++m_pElement;
			return *this;
		}

		/**
		Moves the iterator to the previous element. */
		Iter& prev()
		{
			--m_pElement;
			return *this;
		}
};

#endif