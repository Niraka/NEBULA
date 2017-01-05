/**
A red black tree forward iterator.

@date edited 05/01/2017
@date authored 18/12/2016

@author Nathan Sainsbury */

#ifndef RB_TREE_ITERATOR_H
#define RB_TREE_ITERATOR_H

#include "Engine/System/Tools/RBTreeNode.h"

template <class ElementType>
class RBTreeIterator
{
	private:
		typedef RBTreeIterator<ElementType> Iter;
		RBTreeNode<ElementType>* m_pNode;

		/**
		Finds and returns the maximum node in the given tree.
		@param pSubtreeRoot The root of the subtree to search
		@return A pointer to the maximum node */
		RBTreeNode<ElementType>* max(RBTreeNode<ElementType>* pSubtreeRoot)
		{
			while (!pSubtreeRoot->pRightChild->bIsLeaf)
			{
				pSubtreeRoot = pSubtreeRoot->pRightChild;
			}

			return pSubtreeRoot;
		}

		/**
		Finds and returns the minimum node in the given tree.
		@param pSubtreeRoot The root of the subtree to search
		@return A pointer to the minimum node */
		RBTreeNode<ElementType>* min(RBTreeNode<ElementType>* pSubtreeRoot)
		{
			while (!pSubtreeRoot->pLeftChild->bIsLeaf)
			{
				pSubtreeRoot = pSubtreeRoot->pLeftChild;
			}

			return pSubtreeRoot;
		}

	protected:

	public:
		/**
		Constructs an iterator targetting the given node.
		@param pNode The address of the node to target */
		RBTreeIterator(RBTreeNode<ElementType>* pNode)
		{
			m_pNode = pNode;
		}

		/**
		Copy constructs an iterator.
		@param other The iterator to copy */
		RBTreeIterator(const Iter& other)
		{
			m_pNode = other.m_pNode;
		}

		/**
		Assigns an iterator.
		@param other The iterator to copy
		@return A reference to this iterator */
		Iter& operator=(const Iter& other)
		{
			m_pNode = other.m_pNode;
			return *this;
		}

		/**
		Destructs the iterator. */
		~RBTreeIterator()
		{
			m_pNode = nullptr;
		}

		/**
		Moves the iterator to the next element. Integer parameter is a dummy parameter that
		indicates post-increment.
		@return A copy of the iterator */
		Iter operator++(int)
		{
			Iter temp = *this;
			++*this;
			return temp;
		}

		/**
		Moves the iterator to the next element.
		@return A reference to this iterator */
		Iter& operator++()
		{
			if (m_pNode->bIsLeaf)
			{
				// Do nothing, iterator is at the end
			}
			else if (!m_pNode->pRightChild->bIsLeaf)
			{
				m_pNode = min(m_pNode->pRightChild);
			}
			else
			{
				RBTreeNode<ElementType>* pNode = m_pNode->pParent;
				while (!pNode->bIsLeaf && m_pNode == pNode->pRightChild)
				{
					m_pNode = pNode;
					pNode = m_pNode->pParent;
				}

				m_pNode = pNode;
			}
			return *this;
		}

		/**
		Moves the iterator to the previous element. Integer parameter is a dummy parameter that
		indicates post-decrement.
		@return A copy of the iterator */
		Iter operator--(int)
		{
			Iter temp = *this;
			--*this;
			return temp;
		}

		/**
		Moves the iterator to the previous element.
		@return A reference to this iterator */
		Iter& operator--()
		{
			if (m_pNode->bIsLeaf)
			{
				// Go to last element
				m_pNode = max(m_pNode);
			}
			else if (!m_pNode->pLeftChild->bIsLeaf)
			{
				m_pNode = max(m_pNode->pLeftChild);
			}
			else
			{
				RBTreeNode<ElementType>* pNode = m_pNode->pParent;
				while (!pNode->bIsLeaf && m_pNode == pNode->pLeftChild)
				{
					m_pNode = pNode;
					pNode = m_pNode->pParent;
				}

				if (!m_pNode->bIsLeaf)
				{
					m_pNode = pNode;
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
			return m_pNode == other.m_pNode;
		}

		/**
		Compares two iterators for inequality.
		@param other The iterator to compare with
		@return True if the iterators are not considered equal, false if they are */
		bool operator!=(const Iter& other)
		{
			return m_pNode != other.m_pNode;
		}

		/**
		Dereferences the currently addressed element. If the iterator is not targetting a valid
		element undefined behaviour will be invoked.
		@return A reference to an element */
		ElementType& operator*()
		{
			return m_pNode->element;
		}

		/**
		Dereferences the currently addressed element. If the iterator is not targetting a valid
		element undefined behaviour will be invoked.
		@return A reference to an element */
		ElementType* operator->()
		{
			return &(m_pNode->element);
		}

		/**
		Moves the iterator to the next element. */
		Iter& next()
		{
			++m_pNode;
			return *this;
		}

		/**
		Moves the iterator to the previous element. */
		Iter& prev()
		{
			--m_pNode;
			return *this;
		}
};

#endif