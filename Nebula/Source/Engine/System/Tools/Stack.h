/**
A standard stack container.

Concurrent versions are available.

@see ConcurrentStack

@date edited 13/09/2016
@date authored 13/09/2016

@author Nathan Sainsbury */

#ifndef STACK_H
#define STACK_H

#include "Engine/System/Tools/StackNode.h"
#include "Engine/System/Tools/LanguageExtensions.h"

template <class ElementType>
class Stack
{
	private:
		unsigned int m_uiNumElements;
		StackNode<ElementType>* m_top;

	protected:

	public:
		/**
		Constructs an empty stack. */
		Stack()
		{
			m_uiNumElements = 0;

			reset();
		}

		/**
		Destructs the stack. All elements are deleted. */
		~Stack()
		{
			while (isNotEmpty())
			{
				pop();
			}
		}

		/**
		Resets the stack. All elements are deleted. */
		void reset()
		{
			while (isNotEmpty())
			{
				pop();
			}

			m_top = nullptr;
		}

		/**
		Pushes an element on to the top of the stack. 
		@param element The element to push */
		void push(const ElementType& element)
		{
			StackNode<ElementType>* currentTop = m_top;
			StackNode<ElementType>* newTop = new StackNode<ElementType>(element);
			m_top = newTop;
			newTop->next = currentTop;

			++m_uiNumElements;
		}

		/**
		Removes the top element in the stack. If the stack is empty, this function does nothing. */
		void pop()
		{
			if (m_top)
			{
				StackNode<ElementType>* second = m_top->next;
				delete m_top;
				m_top = second;

				--m_uiNumElements;
			}
		}

		/**
		Returns a reference to the top element in the stack. If the stack was empty, this function 
		invokes undefined behaviour.
		@return A reference to the element at the top of the stack */
		ElementType& first()
		{
			if (m_top == nullptr)
			{
				fatalexit("Attempting to access non-existent stack node");
			}

			return m_top->data;
		}

		/**
		Retrieves the number of elements in the stack.
		@return The number of elements in the stack */
		unsigned int numElements() const
		{
			return m_uiNumElements;
		}

		/**
		Checks whether the stack is empty.
		@see isNotEmpty
		@return True if the stack is empty, false if it is not */
		bool isEmpty() const
		{
			return m_uiNumElements == 0;
		}

		/**
		Checks whether the stack is not empty.
		@see isEmpty
		@return True if the stack contains at least 1 element, false if it does not */
		bool isNotEmpty() const
		{
			return m_uiNumElements != 0;
		}
};

#endif