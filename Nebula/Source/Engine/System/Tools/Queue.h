/**
A standard queue container.

Concurrent versions are available.

@see ConcurrentQueue

@date edited 13/09/2016
@date authored 13/09/2016

@author Nathan Sainsbury */

#ifndef QUEUE_H
#define QUEUE_H

#include "Engine/System/Tools/QueueNode.h"
#include "Engine/System/Tools/LanguageExtensions.h"

template <class ElementType>
class Queue
{
	private:
		unsigned int m_uiNumElements;
		QueueNode<ElementType>* m_front;
		QueueNode<ElementType>* m_back;

	protected:

	public:
		/**
		Constructs an empty queue. */
		Queue()
		{
			reset();
		}

		/**
		Destructs the queue. All elements are deleted. */
		~Queue()
		{
			QueueNode<ElementType>* next = nullptr;
			QueueNode<ElementType>* current = m_front;
			while (current != nullptr)
			{
				next = current->next;
				delete current;
				current = next;
			}
		}

		/**
		Resets the queue. All elements are deleted. */
		void reset()
		{
			QueueNode<ElementType>* next = nullptr;
			QueueNode<ElementType>* current = m_front;
			while (current != nullptr)
			{
				next = current->next;
				delete current;
				current = next;
			}

			m_uiNumElements = 0;
			m_front = nullptr;
			m_back = nullptr;
		}

		/**
		Pushes an element on to the back of the queue.
		@param element The element to push */
		void push(const ElementType& element)
		{
			QueueNode<ElementType>* node = new QueueNode<ElementType>(element);
			++m_uiNumElements;
			
			if (m_uiNumElements == 1)
			{
				m_front = node;
				m_back = node;
			}
			else
			{
				m_back->next = node;
				m_back = node;
			}	
		}

		/**
		Removes the first element in the queue. If the queue is empty, this function does
		nothing. */
		void pop()
		{
			if (isNotEmpty())
			{
				QueueNode<ElementType>* next = m_front->next;
				delete m_front;
				--m_uiNumElements;

				if (m_uiNumElements == 0)
				{
					m_front = nullptr;
					m_back = nullptr;
				}
				else
				{
					m_front = next;
				}
			}
		}

		/**
		Returns a reference to the first element in the queue. If the queue was empty, this function
		invokes undefined behaviour.
		@return The first element in the queue */
		ElementType& first()
		{
			if (m_front == nullptr)
			{
				fatalexit("Attempting to access non-existent queue node");
			}

			return m_front->data;
		}

		/**
		Gets the number of elements in the queue.
		@return The number of elements in the queue */
		unsigned int numElements() const
		{
			return m_uiNumElements;
		}

		/**
		Returns true if the queue is empty. 
		@return True if the queue is empty, false otherwise */
		bool isEmpty() const
		{
			return m_uiNumElements == 0;
		}

		/**
		Returns true if the queue is not empty.
		@return True if the queue is not empty, false otherwise */
		bool isNotEmpty() const
		{
			return m_uiNumElements != 0;
		}
};

#endif