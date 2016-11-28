/**
A node for use within a red-black tree.

@date edited 24/11/2016
@date authored 21/10/2016

@author Nathan Sainsbury */

#ifndef RB_TREE_NODE_H
#define RB_TREE_NODE_H

#include "Engine/System/Tools/RBTreeNodeColours.h"
#include "Engine/System/Tools/LanguageExtensions.h"

template <class ElementType>
struct RBTreeNode
{
	private:
		/**
		Disabled because it is currently not intended for use anywhere. Compiler error will be useful
		in catching unintended usages. */
		RBTreeNode<ElementType>& operator=(const RBTreeNode<ElementType>& other)
		{
			return *this;
		}

	protected:

	public:
		/**
		The element. */
		ElementType element;

		/**
		The parent node. */
		RBTreeNode<ElementType>* pParent;

		/**
		The left child node (Evaluates less than). */
		RBTreeNode<ElementType>* pLeftChild;

		/**
		The right child node (Evaluates greater than) */
		RBTreeNode<ElementType>* pRightChild;

		/**
		The colour of the red-black tree node. */
		RBTreeNodeColours colour;

		/**
		Is the node a leaf node or not. */
		bool bIsLeaf;

		/**
		Constructs a tree node with uninitialised values. */
		RBTreeNode()
		{
		}

		/**
		Constructs a tree node with the given element. All other values remain uninitialised.
		@param element The element */
		RBTreeNode(const ElementType& element) :
			element(element)
		{
		}

		/**
		Copy-constructor. Child nodes are also copied. Parent of the root of the subtree copy
		must be set manually.
		@param other The node to copy */
		RBTreeNode(const RBTreeNode<ElementType>& other) :
			element(other.element)
		{
			bIsLeaf = other.bIsLeaf;
			colour = other.colour;
			pParent = nullptr;
			if (notnullptr(other.pLeftChild))
			{
				pLeftChild = new RBTreeNode<ElementType>(*other.pLeftChild);
				pLeftChild->pParent = this;
			}
			else
			{
				pLeftChild = nullptr;
			}
			if (notnullptr(other.pRightChild))
			{
				pRightChild = new RBTreeNode<ElementType>(*other.pRightChild);
				pRightChild->pParent = this;
			}
			else
			{
				pRightChild = nullptr;
			}
		}

		/**
		Destructor. Destroys any attached child nodes. */
		~RBTreeNode()
		{
			delete pLeftChild;
			delete pRightChild;
		}
};

#endif