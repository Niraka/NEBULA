/**
A queue node for use in a queue container.

@date edited 12/10/2016
@date authored 13/09/2016

@author Nathan Sainsbury */

#ifndef QUEUE_NODE_H
#define QUEUE_NODE_H

template <class ElementType>
struct QueueNode
{
	/**
	The element. */
	ElementType data;

	/**
	A pointer to the next node in the queue. */
	QueueNode<ElementType>* next;

	/**
	Constructs a queue node with the given element.
	@param element The element */
	QueueNode(const ElementType& element)
	{
		data = element;
		next = nullptr;
	}
};

#endif