/**
An indexed vector is a container class that extends upon a standard vector to add tracking
functionality. Each index is assigned a version number that tracks the number of times the element
at that index has been swapped out. By comparing Id objects to the current version number at an
index a user can quickly identify whether an element exists or not.

Element access occurs through the use of Id objects and Iterators. Iterators only iterate over
active elements even though each element exists in a valid state.

Ids are composed of the index that the element currently resides at and a unique* version number
that increments each time the stored object is replaced.

*Note that if the version number overflows, Ids are no longer unique. Users should consider this
when selecting their container of choice.

@date authored 26/03/2017
@date edited 14/03/2017

@author Nathan Sainsbury */

#ifndef INDEXED_VECTOR_H
#define INDEXED_VECTOR_H

struct IndexedVectorId
{
	/**
	The index. */
	size_t uiIndex;

	/**
	The version number. */
	size_t uiVersion;

	IndexedVectorId() :
		uiIndex(0),
		uiVersion(0)
	{
	}

	IndexedVectorId(size_t uiIndex, size_t uiVersion) :
		uiIndex(uiIndex),
		uiVersion(uiVersion)
	{
	}
};

template <typename ElementType>
struct IndexedVectorEntry
{
	/**
	Whether the entry currently contains a live element or not. */
	bool bIsActive;

	/**
	The version number. */
	size_t uiVersionNumber;

	/**
	The stored element. */
	ElementType element;

	IndexedVectorEntry() :
		bIsActive(false),
		uiVersionNumber(0),
		element(ElementType())
	{
	}

	IndexedVectorEntry(bool bIsActive, size_t uiVersionNumber, const ElementType& element) :
		bIsActive(bIsActive),
		uiVersionNumber(uiVersionNumber),
		element(element)
	{
	}
};

template <typename ElementType>
class IndexedVectorIterator
{
	private:
		typedef IndexedVectorEntry<ElementType> Entry;
		typedef IndexedVectorIterator<ElementType> Iterator;
		typedef IndexedVectorId Id;

	public:
		IndexedVectorIterator() :
			m_pElement(nullptr),
			m_uiElementIndex(0),
			m_uiMaxIndex(0)
		{
		}

		IndexedVectorIterator(Entry* pElement, 
			size_t uiElementIndex, 
			size_t uiMaxIndex) :
			m_pElement(pElement),
			m_uiElementIndex(uiElementIndex),
			m_uiMaxIndex(uiMaxIndex)
		{
		}

		~IndexedVectorIterator()
		{
			m_pElement = nullptr;
		}

		bool operator==(const Iterator& other) const
		{
			return m_pElement == other.m_pElement;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_pElement != other.m_pElement;
		}

		Iterator& operator++()
		{
			if (m_uiElementIndex == m_uiMaxIndex)
			{
				return *this;
			}

			do 
			{
				++m_pElement;
				++m_uiElementIndex;
			} 
			while (m_uiElementIndex < m_uiMaxIndex && !m_pElement->bIsActive);

			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		Iterator& operator--()
		{
			if (m_uiElementIndex == 0)
			{
				return *this;
			}

			do
			{
				--m_pElement;
				--m_uiElementIndex;
			} 
			while (m_uiElementIndex > 0 && !m_pElement->bIsActive);

			return *this;
		}

		Iterator operator--(int)
		{
			Iterator temp = *this;
			--(*this);
			return temp;
		}

		ElementType* operator->() const
		{
			return &(m_pElement->element);
		}

		ElementType& operator*() const
		{
			return m_pElement->element;
		}

		Id elementId() const
		{
			return Id(m_uiElementIndex, m_pElement->uiVersionNumber);
		}

	protected:
	private:
		Entry* m_pElement;
		size_t m_uiElementIndex;
		size_t m_uiMaxIndex;
};

template <typename ElementType>
class IndexedVectorConstIterator
{
	private:
		typedef IndexedVectorEntry<ElementType> Entry;
		typedef IndexedVectorConstIterator<ElementType> Iterator;
		typedef IndexedVectorId Id;

	public:
		IndexedVectorConstIterator() :
			m_pElement(nullptr),
			m_uiElementIndex(0),
			m_uiMaxIndex(0)
		{
		}


		IndexedVectorConstIterator(Entry* pElement,
			size_t uiElementIndex,
			size_t uiMaxIndex) :
			m_pElement(pElement),
			m_uiElementIndex(uiElementIndex),
			m_uiMaxIndex(uiMaxIndex)
		{
		}

		~IndexedVectorConstIterator()
		{
			m_pElement = nullptr;
		}

		bool operator==(const Iterator& other) const
		{
			return m_pElement == other.m_pElement;
		}

		bool operator!=(const Iterator& other) const
		{
			return m_pElement != other.m_pElement;
		}

		Iterator& operator++()
		{
			if (m_uiElementIndex == m_uiMaxIndex)
			{
				return *this;
			}

			do
			{
				++m_pElement;
				++m_uiElementIndex;
			} 
			while (m_uiElementIndex < m_uiMaxIndex && !m_pElement->bIsActive);

			return *this;
		}

		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		Iterator& operator--()
		{
			if (m_uiElementIndex == 0)
			{
				return *this;
			}

			do
			{
				--m_pElement;
				--m_uiElementIndex;
			} 
			while (m_uiElementIndex > 0 && !m_pElement->bIsActive);

			return *this;
		}

		Iterator operator--(int)
		{
			Iterator temp = *this;
			--(*this);
			return temp;
		}

		const ElementType* operator->() const
		{
			return &(m_pElement->element);
		}

		const ElementType& operator*() const
		{
			return m_pElement->element;
		}

		Id elementId() const
		{
			return Id(m_uiElementIndex, m_pElement->uiVersionNumber);
		}

	protected:

	private:
		Entry* m_pElement;
		size_t m_uiElementIndex;
		size_t m_uiMaxIndex;
};

template <typename ElementType>
class IndexedVector
{
	private:
		typedef IndexedVector<ElementType> Array;
		typedef IndexedVectorEntry<ElementType> Entry;

	public:
		typedef IndexedVectorConstIterator<ElementType> ConstIterator;
		typedef IndexedVectorIterator<ElementType> Iterator;
		typedef IndexedVectorId Id;

		static_assert(!std::is_const<ElementType>::value, "IndexedVector does not support const "
			"element types.");
		static_assert(std::is_default_constructible<ElementType>::value, "IndexedVector template "
			"parameter 'ElementType' must be default constructible.");
		static_assert(std::is_copy_assignable<ElementType>::value, "IndexedVector requires elements "
			"to be copy assignable");
		static_assert(std::is_destructible<ElementType>::value, "IndexedVector requires elements "
			"to be destructible");

		/**
		Constructor. */
		IndexedVector() :
			m_uiNumElements(0),
			m_uiMaxElements(1)
		{
			m_pElements = new Entry[m_uiMaxElements];
		}

		/**
		Destructor. */
		~IndexedVector()
		{
			delete[] m_pElements;
		}

		/**
		Copy constructor.
		@param other The indexed vector to copy */
		IndexedVector(const Array& other) :
			m_uiNumElements(other.m_uiNumElements),
			m_uiMaxElements(other.m_uiMaxElements)
		{
			m_pElements = new Entry[m_uiMaxElements];
			for (size_t i = 0; i < m_uiMaxElements; ++i)
			{
				m_pElements[i] = other.m_pElements[i];
			}
		}

		/**
		Move-copy constructor.
		@param other The indexed vector to copy */
		IndexedVector(Array&& other) :
			m_uiNumElements(other.m_uiNumElements),
			m_uiMaxElements(other.m_uiMaxElements),
			m_pElements(other.m_pElements)
		{
			other.m_pElements = new Entry[m_uiMaxElements];
			other.m_uiNumElements = 0;
		}

		/**
		Assignment operator.
		@param other The indexed vector to assign from
		@return A reference to this indexed vector */
		Array& operator=(const Array& other)
		{
			if (this != &other)
			{
				m_uiNumElements = other.m_uiNumElements;
				m_uiMaxElements = other.m_uiMaxElements;

				delete[] m_pElements;

				m_pElements = new Entry[m_uiMaxElements];
				for (size_t i = 0; i < m_uiMaxElements; ++i)
				{
					m_pElements[i] = other.m_pElements[i];
				}
			}

			return *this;
		}

		/**
		Move-assignment operator.
		@param other The indexed vector to assign from
		@return A reference to this indexed vector */
		Array& operator=(Array&& other)
		{
			if (this != &other)
			{
				delete[] m_pElements;

				m_uiNumElements = other.m_uiNumElements;
				m_uiMaxElements = other.m_uiMaxElements;
				m_pElements = other.m_pElements;

				other.m_uiNumElements = 0;
				other.m_uiMaxElements = 1;
				other.m_pElements = new Entry[other.m_uiMaxElements];
			}

			return *this;
		}

		/**
		Swaps the contents of the indexed vector with another indexed vector of the same type and
		size.
		@param other The array to swap contents with */
		void swap(Array& other)
		{
			std::swap(m_uiNumElements, other.m_uiNumElements);
			std::swap(m_uiMaxElements, other.m_uiMaxElements);
			std::swap(m_pElements, other.m_pElements);
		}

		/**
		Pushes an element on to the indexed vector. If the container was full, the element is not
		appended and a default id is returned instead.
		@param element The element to insert
		@return The elements id, or a default id */
		Id push(const ElementType& element)
		{
			if (m_uiNumElements < m_uiMaxElements)
			{
				for (size_t i = 0; i < m_uiMaxElements; ++i)
				{
					if (!m_pElements[i].bIsActive)
					{
						m_pElements[i].bIsActive = true;
						++m_pElements[i].uiVersionNumber;
						m_pElements[i].element = element;
						++m_uiNumElements;

						return Id(i, m_pElements[i].uiVersionNumber);
					}
				}
			}
			else
			{
				size_t uiOldMax = m_uiMaxElements;

				// Expand if possible
				if (m_uiMaxElements < std::numeric_limits<size_t>::max())
				{
					if (m_uiMaxElements * 2 < std::numeric_limits<size_t>::max())
					{
						m_uiMaxElements *= 2;
					}
					else
					{
						m_uiMaxElements = std::numeric_limits<size_t>::max();
					}

					Entry* pNewElements = new Entry[m_uiMaxElements];
					for (size_t ui = 0; ui < uiOldMax; ++ui)
					{
						pNewElements[ui] = m_pElements[ui];
					}

					delete[] m_pElements;
					m_pElements = pNewElements;

					m_pElements[uiOldMax].bIsActive = true;
					++m_pElements[uiOldMax].uiVersionNumber;
					m_pElements[uiOldMax].element = std::move(element);
					++m_uiNumElements;

					return Id(uiOldMax, 1);
				}
			}

			return Id(0, 0);
		}

		/**
		Pushes an element on to the indexed vector. If the container was full, the element is not
		appended and a default id is returned instead.
		@param element The element to insert
		@return The elements id, or a default id */
		Id push(ElementType&& element)
		{
			if (m_uiNumElements < m_uiMaxElements)
			{
				for (size_t i = 0; i < m_uiMaxElements; ++i)
				{
					if (!m_pElements[i].bIsActive)
					{
						m_pElements[i].bIsActive = true;
						++m_pElements[i].uiVersionNumber;
						m_pElements[i].element = std::move(element);
						++m_uiNumElements;

						return Id(i, m_pElements[i].uiVersionNumber);
					}
				}
			}
			else
			{
				size_t uiOldMax = m_uiMaxElements;
				
				// Expand if possible
				if (m_uiMaxElements < std::numeric_limits<size_t>::max())
				{
					if (m_uiMaxElements * 2 < std::numeric_limits<size_t>::max())
					{
						m_uiMaxElements *= 2;
					}
					else
					{
						m_uiMaxElements = std::numeric_limits<size_t>::max();
					}
					
					Entry* pNewElements = new Entry[m_uiMaxElements];
					for (size_t ui = 0; ui < uiOldMax; ++ui)
					{
						pNewElements[ui] = m_pElements[ui];
					}

					delete[] m_pElements;
					m_pElements = pNewElements;

					m_pElements[uiOldMax].bIsActive = true;
					++m_pElements[uiOldMax].uiVersionNumber;
					m_pElements[uiOldMax].element = std::move(element);
					++m_uiNumElements;

					return Id(uiOldMax, 1);
				}			
			}

			return Id(0, 0);
		}

		/**
		Inserts an element at the given index.
		@param element The element to insert
		@param uiIndex The index to insert at
		@return The elements id, or a default id */
		Id insert(const ElementType& element, size_t uiIndex)
		{
			if (uiIndex < m_uiMaxElements)
			{
				if (!m_pElements[uiIndex].bIsActive)
				{
					m_pElements[uiIndex].bIsActive = true;
					++m_uiNumElements;
				}

				++m_pElements[uiIndex].uiVersionNumber;
				m_pElements[uiIndex].element = element;

				return Id(uiIndex, m_pElements[uiIndex].uiVersionNumber);
			}
			else
			{
				return Id(0, 0);
			}
		}

		/**
		Inserts an element at the given index.
		@param element The element to insert
		@param uiIndex The index to insert at
		@return The elements id, or a default id */
		Id insert(ElementType&& element, size_t uiIndex)
		{
			if (uiIndex < m_uiMaxElements)
			{
				if (!m_pElements[uiIndex].bIsActive)
				{
					m_pElements[uiIndex].bIsActive = true;
					++m_uiNumElements;
				}

				++m_pElements[uiIndex].uiVersionNumber;
				m_pElements[uiIndex].element = std::move(element);

				return Id(uiIndex, m_pElements[uiIndex].uiVersionNumber);
			}
			else
			{
				return Id(0, 0);
			}
		}

		/**
		Reserves space for at least the given number of elements. If the given number is less
		than the current maximum elements, no action is taken.
		@param uiCapacity The desired capacity */
		void reserve(size_t uiCapacity)
		{
			if (uiCapacity > m_uiMaxElements)
			{
				Entry* pNewElements = new Entry[uiCapacity];
				for (size_t ui = 0; ui < m_uiMaxElements; ++ui)
				{
					pNewElements[ui] = m_pElements[ui];
				}

				delete[] m_pElements;
				m_pElements = pNewElements;
				m_uiMaxElements = uiCapacity;
			}
		}

		/**
		Returns an iterator to an element with the given id. If no such element existed, the
		iterator will address the end iterator.
		@param id An id
		@return An iterator to the element, or an iterator to the end */
		Iterator find(const Id& id) const
		{
			if (id.uiIndex < m_uiMaxElements)
			{
				if (m_pElements[id.uiIndex].uiVersionNumber == id.uiVersion)
				{
					return Iterator(&m_pElements[id.uiIndex], id.uiIndex, m_uiMaxElements);
				}
				else
				{
					return end();
				}
			}

			return end();
		}

		/**
		Removes an element with the given id. If no such element existed, the container is not
		modified.
		@param id The id of the element to remove */
		void remove(const Id& id)
		{
			if (id.uiIndex < m_uiMaxElements)
			{
				if (m_pElements[id.uiIndex].uiVersionNumber == id.uiVersion)
				{
					m_pElements[id.uiIndex].bIsActive = false;
					++m_pElements[id.uiIndex].uiVersionNumber;
					m_pElements[id.uiIndex].element = ElementType();
					--m_uiNumElements;
				}
			}
		}

		/**
		Clears all elements. Version counters are incremented. */
		void clear()
		{
			for (size_t i = 0; i < m_uiMaxElements; ++i)
			{
				m_pElements[i].bIsActive = false;
				++m_pElements[i].uiVersionNumber;
				m_pElements[i].element = ElementType();
			}

			m_uiNumElements = 0;
		}

		/**
		Clears all elements. Version counters are reset to 0. */
		void reset()
		{
			for (size_t i = 0; i < m_uiMaxElements; ++i)
			{
				m_pElements[i].bIsActive = false;
				m_pElements[i].uiVersionNumber = 0;
				m_pElements[i].element = ElementType();
			}

			m_uiNumElements = 0;
		}

		/**
		Constructs and returns an iterator addressing the first element.
		@return An iterator addressing the first element */
		Iterator begin() const
		{
			return Iterator(&m_pElements[0], 0, m_uiMaxElements);
		}

		/**
		Constructs and returns an iterator addressing the end element.
		@return An iterator addressing the end element */
		Iterator end() const
		{
			return Iterator(&m_pElements[m_uiMaxElements], m_uiMaxElements, m_uiMaxElements);
		}

		/**
		Constructs and returns a const iterator addressing the first element.
		@return A const iterator addressing the first element */
		ConstIterator cbegin() const
		{
			return ConstIterator(&m_pElements[0], 0, m_uiMaxElements);
		}

		/**
		Constructs and returns a const iterator addressing the end element.
		@return A const iterator addressing the end element */
		ConstIterator cend() const
		{
			return ConstIterator(&m_pElements[m_uiMaxElements], m_uiMaxElements, m_uiMaxElements);
		}

		/**
		Retrieves the current number of elements in the container.
		@return The current number of elements */
		size_t size() const
		{
			return m_uiNumElements;
		}

		/**
		Retrieves the maximum number of elements in the container.
		@return The maximum number of elements */
		size_t capacity() const
		{
			return m_uiMaxElements;
		}

		/**
		Queries whether the indexed vector is empty.
		@return True if the indexed vector is empty, false if it is */
		bool isEmpty() const
		{
			return m_uiNumElements == 0;
		}

		/**
		Queries whether the indexed vector is not empty.
		@return True if the indexed vector is not empty, false if it is */
		bool isNotEmpty() const
		{
			return m_uiNumElements != 0;
		}

		/**
		Queries whether the indexed vector is full.
		@return True if the indexed vector is full, false if it is not */
		bool isFull() const
		{
			return m_uiNumElements == m_uiMaxElements;
		}

		/**
		Queries whether the indexed vector is not full.
		@return True if the indexed vector is not full, false if it is */
		bool isNotFull() const
		{
			return m_uiNumElements != m_uiMaxElements;
		}

	protected:

	private:
		Entry* m_pElements;
		size_t m_uiNumElements;
		size_t m_uiMaxElements;
};

#endif