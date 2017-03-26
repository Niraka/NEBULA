/**
An indexed array is a container class that extends upon a standard array to add tracking 
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

#ifndef INDEXED_ARRAY_H
#define INDEXED_ARRAY_H

struct IndexedArrayId
{
	/**
	The index. */
	size_t uiIndex;

	/**
	The version number. */
	size_t uiVersion;

	IndexedArrayId() : 
		uiIndex(0), 
		uiVersion(0) 
	{
	}

	IndexedArrayId(size_t uiIndex, size_t uiVersion) : 
		uiIndex(uiIndex), 
		uiVersion(uiVersion) 
	{
	}
};

template <typename ElementType>
struct IndexedArrayEntry
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

	IndexedArrayEntry() : 
		bIsActive(false), 
		uiVersionNumber(0), 
		element(ElementType()) 
	{
	}

	IndexedArrayEntry(bool bIsActive, size_t uiVersionNumber, const ElementType& element) :
		bIsActive(bIsActive), 
		uiVersionNumber(uiVersionNumber), 
		element(element)
	{
	}
};

template <typename ElementType>
class IndexedArrayIterator
{
	private:
		typedef IndexedArrayEntry<ElementType> Entry;
		typedef IndexedArrayIterator<ElementType> Iterator;
		typedef IndexedArrayId Id;

	public:
		IndexedArrayIterator() :
			m_pElement(nullptr),
			m_uiElementIndex(0),
			m_uiMaxIndex(0)
		{
		}

		IndexedArrayIterator(Entry* pElement,
			size_t uiElementIndex,
			size_t uiMaxIndex) :
			m_pElement(pElement),
			m_uiElementIndex(uiElementIndex),
			m_uiMaxIndex(uiMaxIndex)		
		{
		}

		~IndexedArrayIterator()
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
class IndexedArrayConstIterator
{
	private:
		typedef IndexedArrayEntry<ElementType> Entry;
		typedef IndexedArrayConstIterator<ElementType> Iterator;
		typedef IndexedArrayId Id;

	public:
		IndexedArrayConstIterator() :
			m_pElement(nullptr),
			m_uiElementIndex(0)
		{
		}

		IndexedArrayConstIterator(Entry* pElement, 
			size_t uiElementIndex,
			size_t uiMaxIndex) :
			m_pElement(pElement),
			m_uiElementIndex(uiElementIndex),
			m_uiMaxIndex(uiMaxIndex)
		{
		}

		~IndexedArrayConstIterator()
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

template <typename ElementType, size_t m_uiMaxElements>
class IndexedArray
{
	private:
		typedef IndexedArray<ElementType, m_uiMaxElements> Array;
		typedef IndexedArrayEntry<ElementType> Entry;

	public:
		typedef IndexedArrayConstIterator<ElementType> ConstIterator;
		typedef IndexedArrayIterator<ElementType> Iterator;
		typedef IndexedArrayId Id;

		static_assert((m_uiMaxElements > 0), "IndexedArray template parameter 'm_uiMaxElements' "
			"must be >= 1.");
		static_assert(!std::is_const<ElementType>::value, "IndexedArray does not support const "
			"element types.");
		static_assert(std::is_default_constructible<ElementType>::value, "IndexedArray template "
			"parameter 'ElementType' must be default constructible.");
		static_assert(std::is_copy_assignable<ElementType>::value, "IndexedArray requires elements "
			"to be copy assignable");
		static_assert(std::is_destructible<ElementType>::value, "IndexedArray requires elements "
			"to be destructible");		

		/**
		Constructor. */
		IndexedArray() :
			m_uiNumElements(0)
		{
			m_pElements = new Entry[m_uiMaxElements];
		}

		/**
		Destructor. */
		~IndexedArray()
		{
			delete[] m_pElements;
		}

		/**
		Copy constructor.
		@param other The indexed array to copy */
		IndexedArray(const Array& other) :
			m_uiNumElements(other.m_uiNumElements)
		{
			m_pElements = new Entry[m_uiMaxElements];
			for (size_t i = 0; i < m_uiMaxElements; ++i)
			{
				m_pElements[i] = other.m_pElements[i];
			}
		}

		/**
		Move-copy constructor.
		@param other The indexed array to copy */
		IndexedArray(Array&& other) :
			m_uiNumElements(other.m_uiNumElements),
			m_pElements(other.m_pElements)
		{
			other.m_pElements = new Entry[m_uiMaxElements];
			other.m_uiNumElements = 0;
		}

		/**
		Assignment operator.
		@param other The indexed array to assign from
		@return A reference to this indexed array */
		Array& operator=(const Array& other)
		{
			if (this != &other)
			{
				m_uiNumElements = other.m_uiNumElements;

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
		@param other The indexed array to assign from
		@return A reference to this indexed array */
		Array& operator=(Array&& other)
		{
			if (this != &other)
			{
				delete[] m_pElements;

				m_pElements = other.m_pElements;
				m_uiNumElements = other.m_uiNumElements;

				other.m_pElements = new Entry[m_uiMaxElements];
				other.m_uiNumElements = 0;
			}

			return *this;
		}

		/**
		Swaps the contents of the indexed array with another indexed array of the same type and
		size.
		@param other The array to swap contents with */
		void swap(Array& other)
		{
			std::swap(m_uiNumElements, other.m_uiNumElements);
			std::swap(m_pElements, other.m_pElements);
		}

		/**
		Pushes an element on to the indexed array. If the container was full, the element is not 
		appended and a default id is returned instead.
		@param element The element to insert
		@return The elements id, or a default id */
		Id push(const ElementType& element)
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

			return Id(0, 0);
		}

		/**
		Pushes an element on to the indexed array. If the container was full, the element is not 
		appended and a default id is returned instead.
		@param element The element to insert
		@return The elements id, or a default id */
		Id push(ElementType&& element)
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
		Queries whether the indexed array is empty.
		@return True if the indexed array is empty, false if it is */
		bool isEmpty() const
		{
			return m_uiNumElements == 0;
		}

		/**
		Queries whether the indexed array is not empty.
		@return True if the indexed array is not empty, false if it is */
		bool isNotEmpty() const
		{
			return m_uiNumElements != 0;
		}

		/**
		Queries whether the indexed array is full.
		@return True if the indexed array is full, false if it is not */
		bool isFull() const
		{
			return m_uiNumElements == m_uiMaxElements;
		}

		/**
		Queries whether the indexed array is not full.
		@return True if the indexed array is not full, false if it is */
		bool isNotFull() const
		{
			return m_uiNumElements != m_uiMaxElements;
		}

	protected:

	private:
		Entry* m_pElements;
		size_t m_uiNumElements;
};

#endif