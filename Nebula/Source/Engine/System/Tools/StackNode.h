/**
A stack node for use in a stack container.

@date edited 12/10/2016
@date authored 13/09/2016

@author Nathan Sainsbury */

#ifndef STACK_NODE_H
#define STACK_NODE_H

template <class ElementType>
struct StackNode
{
	/**
	The element. */
	ElementType data;

	/**
	A pointer to the next node in the stack. */
	StackNode<ElementType>* next;

	/**
	Constructs a stack node with the given element. 
	@param element The element */
	StackNode(const ElementType& element)
	{
		data = element;
		next = nullptr;
	}
};

#endif