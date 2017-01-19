/**
A red-black tree.

The tree possesses a single element cache that is populated by the last accessed/modified element
so that consecutive uses of a single element are quicker.

@date edited 19/01/2017
@date authored 19/10/2016

@author Nathan Sainsbury */

#ifndef RB_TREE_H
#define RB_TREE_H

#include <limits.h>
#include <type_traits>

#include "Engine/System/Tools/FakeParam.h"
#include "Engine/System/Tools/RBTreeNode.h"
#include "Engine/System/Tools/RBTreeElementBuilder.h"
#include "Engine/System/Tools/LanguageExtensions.h"
#include "Engine/System/Tools/RBTreeIterator.h"
#include "Engine/System/Tools/RBTreeConstIterator.h"

template <class ElementType,
	class KeyType = ElementType, 
	class Comparator = std::less<KeyType>,
	class ElementBuilder = RBTreeElementBuilder<ElementType, KeyType>>
class RBTree
{
	private:
		RBTreeNode<ElementType>* m_pRootNode;
		RBTreeNode<ElementType>* m_pHeadNode;
		RBTreeNode<ElementType>* m_pCachedElement;
		RBTreeNode<ElementType>* m_pLeftMost;
		unsigned int m_uiNumElements;
		unsigned int m_uiMaxElements;
		Comparator m_comparator;
		ElementBuilder m_elementBuilder;

		/**
		Constructs the head node if it does not exist and ensures it is correctly linked to the
		root node if one exists. */
		void buildAndOrBindHead()
		{
			if (isnullptr(m_pHeadNode))
			{
				// Parent of head node does not matter
				// Both children must point to root node (used by iterators)
				// Must be flagged as a leaf
				// Must always be black
				m_pHeadNode = new RBTreeNode<ElementType>();
				m_pHeadNode->pLeftChild = m_pRootNode;
				m_pHeadNode->pRightChild = m_pRootNode;
				m_pHeadNode->pParent = nullptr;
				m_pHeadNode->colour = RBTreeNodeColours::BLACK;
				m_pHeadNode->bIsLeaf = true;
			}

			if (notnullptr(m_pRootNode))
			{
				m_pRootNode->pParent = m_pHeadNode;
			}
		}

		/**
		Constructs the root node. 
		@param element The element to insert */
		void constructRootNode(const ElementType& element)
		{
			m_pRootNode = new RBTreeNode<ElementType>(element);
			RBTreeNode<ElementType>* pLeafLeft = new RBTreeNode<ElementType>();
			RBTreeNode<ElementType>* pLeafRight = new RBTreeNode<ElementType>();

			buildAndOrBindHead();

			m_pRootNode->pLeftChild = pLeafLeft;
			m_pRootNode->pRightChild = pLeafRight;
			m_pRootNode->colour = RBTreeNodeColours::BLACK;
			m_pRootNode->bIsLeaf = false;

			pLeafLeft->pLeftChild = nullptr;
			pLeafLeft->pRightChild = nullptr;
			pLeafLeft->pParent = m_pRootNode;
			pLeafLeft->colour = RBTreeNodeColours::BLACK;
			pLeafLeft->bIsLeaf = true;

			pLeafRight->pLeftChild = nullptr;
			pLeafRight->pRightChild = nullptr;
			pLeafRight->pParent = m_pRootNode;
			pLeafRight->colour = RBTreeNodeColours::BLACK;
			pLeafRight->bIsLeaf = true;
		}

		/**
		Constructs and configures a new child node to the left of the given parent node.
		@param pParentNode The parent node to attach to
		@param element The element to assign to the node */
		void addChildNodeLeft(RBTreeNode<ElementType>* pParentNode, const ElementType& element)
		{
			// Const elements cannot be modified so the new node is actually going to be a freshly
			// initialised node. The existing left leaf is just moved down.
			RBTreeNode<ElementType>* pNewLeftChild = new RBTreeNode<ElementType>(element);
			RBTreeNode<ElementType>* pLeafLeft = pParentNode->pLeftChild;
			RBTreeNode<ElementType>* pLeafRight = new RBTreeNode<ElementType>();

			pParentNode->pLeftChild = pNewLeftChild;

			// Config new element node
			pNewLeftChild->pLeftChild = pLeafLeft;
			pNewLeftChild->pRightChild = pLeafRight;
			pNewLeftChild->pParent = pParentNode;
			pNewLeftChild->colour = RBTreeNodeColours::RED;
			pNewLeftChild->bIsLeaf = false;

			// Config new leaves
			pLeafLeft->pLeftChild = nullptr;
			pLeafLeft->pRightChild = nullptr;
			pLeafLeft->pParent = pNewLeftChild;
			pLeafLeft->colour = RBTreeNodeColours::BLACK;
			pLeafLeft->bIsLeaf = true;

			pLeafRight->pLeftChild = nullptr;
			pLeafRight->pRightChild = nullptr;
			pLeafRight->pParent = pNewLeftChild;
			pLeafRight->colour = RBTreeNodeColours::BLACK;
			pLeafRight->bIsLeaf = true;
		}

		/**
		Constructs and configures a new child node to the right of the given parent node.
		@param pParentNode The parent node to attach to
		@param element The element to assign to the node */
		void addChildNodeRight(RBTreeNode<ElementType>* pParentNode, const ElementType& element)
		{
			// Const elements cannot be modified so the new node is actually going to be a freshly
			// initialised node. The existing right leaf is just moved down.
			RBTreeNode<ElementType>* pNewRightChild = new RBTreeNode<ElementType>(element);
			RBTreeNode<ElementType>* pLeafLeft = new RBTreeNode<ElementType>();
			RBTreeNode<ElementType>* pLeafRight = pParentNode->pRightChild;

			pParentNode->pRightChild = pNewRightChild;

			// Config new element node
			pNewRightChild->pLeftChild = pLeafLeft;
			pNewRightChild->pRightChild = pLeafRight;
			pNewRightChild->pParent = pParentNode;
			pNewRightChild->colour = RBTreeNodeColours::RED;
			pNewRightChild->bIsLeaf = false;

			// Config new leaves
			pLeafLeft->pLeftChild = nullptr;
			pLeafLeft->pRightChild = nullptr;
			pLeafLeft->pParent = pNewRightChild;
			pLeafLeft->colour = RBTreeNodeColours::BLACK;
			pLeafLeft->bIsLeaf = true;

			pLeafRight->pLeftChild = nullptr;
			pLeafRight->pRightChild = nullptr;
			pLeafRight->pParent = pNewRightChild;
			pLeafRight->colour = RBTreeNodeColours::BLACK;
			pLeafRight->bIsLeaf = true;
		}

		/**
		Inserts an element in to the tree. Insertion will fail if the tree would grow too large or
		the element already existed.
		@param element The element to insert 
		@return True if the element was successfully inserted, false if it was not */
		bool performInsertion(const ElementType& element)
		{
			if (m_uiNumElements + 1 > m_uiMaxElements)
			{
				// Tree would grow too large, fail
				return false;
			}
			else if (m_uiNumElements == 0)
			{
				// Empty tree. Construct the root
				constructRootNode(element);
				
				++m_uiNumElements;
				m_pCachedElement = m_pRootNode;
				m_pLeftMost = m_pRootNode;
				return true;
			}
			else if (notnullptr(m_pCachedElement) &&
				!m_comparator(element, m_pCachedElement->element) &&
				!m_comparator(m_pCachedElement->element, element))
			{
				// Already exists, fail
				return false;
			}
			else
			{
				// Traverse tree to target insertion point
				RBTreeNode<ElementType>* pParentNode = m_pRootNode;
				RBTreeNode<ElementType>* pChildNode = nullptr;
				while (!pParentNode->bIsLeaf)
				{
					if (!m_comparator(element, pParentNode->element) &&
						!m_comparator(pParentNode->element, element))
					{
						// Already exists, fail
						m_pCachedElement = pParentNode;
						return false;
					}
					else if (m_comparator(element, pParentNode->element))
					{
						// Traverse left
						if (!pParentNode->pLeftChild->bIsLeaf)
						{
							pParentNode = pParentNode->pLeftChild;
						}
						else
						{
							addChildNodeLeft(pParentNode, element);
							pChildNode = pParentNode->pLeftChild;

							if (pParentNode == m_pLeftMost)
							{
								m_pLeftMost = pParentNode->pLeftChild;
							}

							++m_uiNumElements;
							break;
						}
					}
					else
					{
						// Traverse right
						if (!pParentNode->pRightChild->bIsLeaf)
						{
							pParentNode = pParentNode->pRightChild;
						}
						else
						{
							addChildNodeRight(pParentNode, element);
							pChildNode = pParentNode->pRightChild;

							++m_uiNumElements;
							break;
						}
					}
				}

				RBTreeNode<ElementType>* pUncleNode = nullptr;

				// Resolve tree starting from the newly inserted node. Warning for dereferencing a 
				// potential nullptr here. Avoided as long as tree structure remains correct
				#pragma warning(suppress: 6011)
				for (RBTreeNode<ElementType>* pCurrentNode = pChildNode;
					pCurrentNode->pParent->colour == RBTreeNodeColours::RED; )
				{
					// Reverse walking the tree so find out which direction we are coming from
					if (pCurrentNode->pParent->pParent->pLeftChild == pCurrentNode->pParent)
					{
						pUncleNode = pCurrentNode->pParent->pParent->pRightChild;
						if (pUncleNode->colour == RBTreeNodeColours::RED)
						{
							// Correct colours and advance up tree
							pCurrentNode->pParent->colour = RBTreeNodeColours::BLACK;
							pUncleNode->colour = RBTreeNodeColours::BLACK;
							pCurrentNode->pParent->pParent->colour = RBTreeNodeColours::RED;

							pCurrentNode = pCurrentNode->pParent->pParent;
						}
						else
						{
							// Resolve
							if (pCurrentNode == pCurrentNode->pParent->pRightChild)
							{
								pCurrentNode = pCurrentNode->pParent;
								rotateLeft(pCurrentNode);
							}

							pCurrentNode->pParent->colour = RBTreeNodeColours::BLACK;
							pCurrentNode->pParent->pParent->colour = RBTreeNodeColours::RED;
							rotateRight(pCurrentNode->pParent->pParent);
						}
					}
					else
					{
						pUncleNode = pCurrentNode->pParent->pParent->pLeftChild;
						if (pUncleNode->colour == RBTreeNodeColours::RED)
						{
							// Correct colours and advance up tree
							pCurrentNode->pParent->colour = RBTreeNodeColours::BLACK;
							pUncleNode->colour = RBTreeNodeColours::BLACK;
							pCurrentNode->pParent->pParent->colour = RBTreeNodeColours::RED;

							pCurrentNode = pCurrentNode->pParent->pParent;
						}
						else
						{
							// Resolve
							if (pCurrentNode == pCurrentNode->pParent->pLeftChild)
							{
								pCurrentNode = pCurrentNode->pParent;
								rotateRight(pCurrentNode);
							}

							pCurrentNode->pParent->colour = RBTreeNodeColours::BLACK;
							pCurrentNode->pParent->pParent->colour = RBTreeNodeColours::RED;
							rotateLeft(pCurrentNode->pParent->pParent);
						}
					}
				}

				// Ensure root is black. This line raises a null dereference warning but will not occur
				// as long as m_uiNumElements is accurate.
				#pragma warning(suppress: 28182)
				m_pRootNode->colour = RBTreeNodeColours::BLACK;

				// Cache the element
				m_pCachedElement = pChildNode;

				return true;		
			}

			return false;
		}

		/**
		Rotates a node left.
		@param pRotatingNode A pointer to the node to rotate  */
		void rotateLeft(RBTreeNode<ElementType>* pRotatingNode)
		{
			RBTreeNode<ElementType>* pNewParentNode = pRotatingNode->pRightChild;
			pRotatingNode->pRightChild = pNewParentNode->pLeftChild;
			pRotatingNode->pRightChild->pParent = pRotatingNode;

			if (!pNewParentNode->pLeftChild->bIsLeaf)
			{
				pNewParentNode->pLeftChild->pParent = pRotatingNode;
			}
			pNewParentNode->pParent = pRotatingNode->pParent;

			if (pRotatingNode == m_pRootNode)
			{
				m_pRootNode = pNewParentNode;
				pNewParentNode->pParent = m_pHeadNode;
			}
			else if (pRotatingNode == pRotatingNode->pParent->pLeftChild)
			{
				pRotatingNode->pParent->pLeftChild = pNewParentNode;
			}
			else
			{
				pRotatingNode->pParent->pRightChild = pNewParentNode;
			}

			pNewParentNode->pLeftChild = pRotatingNode;
			pRotatingNode->pParent = pNewParentNode;
		}

		/**
		Rotates a node right.
		@param pRotatingNode A pointer to the node to rotate */
		void rotateRight(RBTreeNode<ElementType>* pRotatingNode)
		{
			RBTreeNode<ElementType>* pNewParentNode = pRotatingNode->pLeftChild;
			pRotatingNode->pLeftChild = pNewParentNode->pRightChild;
			pRotatingNode->pLeftChild->pParent = pRotatingNode;

			if (!pNewParentNode->pRightChild->bIsLeaf)
			{
				pNewParentNode->pRightChild->pParent = pRotatingNode;
			}
			pNewParentNode->pParent = pRotatingNode->pParent;

			if (pRotatingNode == m_pRootNode)
			{
				m_pRootNode = pNewParentNode;
				pNewParentNode->pParent = m_pHeadNode;
			}
			else if (pRotatingNode == pRotatingNode->pParent->pRightChild)
			{
				pRotatingNode->pParent->pRightChild = pNewParentNode;
			}
			else
			{
				pRotatingNode->pParent->pLeftChild = pNewParentNode;
			}

			pNewParentNode->pRightChild = pRotatingNode;
			pRotatingNode->pParent = pNewParentNode;
		}

		/**
		Removes an element from the tree and resets the cache. If the element did not exist, the 
		tree and cache are not modified.
		@param element The element to remove
		@return True if the element was successfully removed, false if it was not */
		bool performRemoval(const KeyType& key)
		{
			// Find the node to delete. If the node is found it will now be available via the cache
			if (!exists(key))
			{
				return false;
			}
			RBTreeNode<ElementType>* pErasedNode = m_pCachedElement;
			RBTreeNodeColours erasedColour = pErasedNode->colour;

			RBTreeNode<ElementType>* pNode = pErasedNode;
			RBTreeNode<ElementType>* pFixNode = nullptr;
			RBTreeNode<ElementType>* pFixNodeParent = nullptr;

			// Identify which subtrees are active (and thus need resolving)
			if (pNode->pLeftChild->bIsLeaf)
			{
				pFixNode = pNode->pRightChild;
			}
			else if (pNode->pRightChild->bIsLeaf)
			{
				pFixNode = pNode->pLeftChild;
			}
			else
			{
				pNode = successor(pErasedNode);
				pFixNode = pNode->pRightChild;
			}

			// Check the number of subtrees that need to be fixed
			if (pNode == pErasedNode)
			{
				// One subtree
				pFixNodeParent = pErasedNode->pParent;
				pFixNode->pParent = pFixNodeParent;

				if (m_pRootNode == pErasedNode)
				{
					m_pRootNode = pFixNode;
				}
				else if (pFixNodeParent->pLeftChild == pErasedNode)
				{
					pFixNodeParent->pLeftChild = pFixNode;
				}
				else
				{
					pFixNodeParent->pRightChild = pFixNode;
				}

				if (m_pLeftMost == pErasedNode)
				{
					if (pFixNode->bIsLeaf)
					{
						m_pLeftMost = pFixNodeParent;
					}
					else
					{
						m_pLeftMost = min(pFixNode);
					}
				}

				// Delete the dead leaf node
				if (pFixNode == pErasedNode->pLeftChild)
				{
					delete pErasedNode->pRightChild;
				}
				else
				{
					delete pErasedNode->pLeftChild;
				}

				// Delete the removed node
				pErasedNode->pLeftChild = nullptr;
				pErasedNode->pRightChild = nullptr;
				delete pErasedNode;
			}
			else
			{
				// Two subtrees. pNode is the successor.
				pErasedNode->pLeftChild->pParent = pNode;
				delete pNode->pLeftChild;
				pNode->pLeftChild = pErasedNode->pLeftChild;

				if (pNode == pErasedNode->pRightChild)
				{
					// Successor is right child
					pFixNodeParent = pNode;
				}
				else
				{
					// Successor is further down
					pFixNodeParent = pNode->pParent;
					pFixNode->pParent = pFixNodeParent;
					pFixNodeParent->pLeftChild = pFixNode;
					pNode->pRightChild = pErasedNode->pRightChild;
					pErasedNode->pRightChild->pParent = pNode;
				}

				if (m_pRootNode == pErasedNode)
				{
					m_pRootNode = pNode;
				}
				else if (pErasedNode->pParent->pLeftChild == pErasedNode)
				{
					pErasedNode->pParent->pLeftChild = pNode;
				}
				else
				{
					pErasedNode->pParent->pRightChild = pNode;
				}

				// Swap colours
				pNode->pParent = pErasedNode->pParent;
				RBTreeNodeColours c = pNode->colour;
				pNode->colour = erasedColour;
				erasedColour = c;

				// Delete the node
				pErasedNode->pLeftChild = nullptr;
				pErasedNode->pRightChild = nullptr;
				delete pErasedNode;
			}

			// Recursively rebalance
			if (erasedColour == RBTreeNodeColours::BLACK)
			{
				for (; pFixNode != m_pRootNode &&
					pFixNode->colour == RBTreeNodeColours::BLACK;
					pFixNodeParent = pFixNode->pParent)
				{
					if (pFixNode == pFixNodeParent->pLeftChild)
					{
						// Fixing a left subtree
						pNode = pFixNodeParent->pRightChild;
						if (pNode->colour == RBTreeNodeColours::RED)
						{
							pNode->colour = RBTreeNodeColours::BLACK;
							pFixNodeParent->colour = RBTreeNodeColours::RED;
							rotateLeft(pFixNodeParent);
							pNode = pFixNodeParent->pRightChild;
						}

						if (pNode->bIsLeaf)
						{
							pFixNode = pFixNodeParent;
						}
						else if (pNode->pLeftChild->colour == RBTreeNodeColours::BLACK &&
							pNode->pRightChild->colour == RBTreeNodeColours::BLACK)
						{
							pNode->colour = RBTreeNodeColours::RED;
							pFixNode = pFixNodeParent;
						}
						else
						{
							if (pNode->pRightChild->colour == RBTreeNodeColours::BLACK)
							{
								pNode->pLeftChild->colour = RBTreeNodeColours::BLACK;
								pNode->colour = RBTreeNodeColours::RED;
								rotateRight(pNode);
								pNode = pFixNodeParent->pRightChild;
							}

							pNode->colour = pFixNodeParent->colour;
							pFixNodeParent->colour = RBTreeNodeColours::BLACK;
							pNode->pRightChild->colour = RBTreeNodeColours::BLACK;
							rotateLeft(pFixNodeParent);
							break;
						}
					}
					else
					{
						// Fixing a right subtree
						pNode = pFixNodeParent->pLeftChild;
						if (pNode->colour == RBTreeNodeColours::RED)
						{
							pNode->colour = RBTreeNodeColours::BLACK;
							pFixNodeParent->colour = RBTreeNodeColours::RED;
							rotateRight(pFixNodeParent);
							pNode = pFixNodeParent->pLeftChild;
						}

						if (pNode->bIsLeaf)
						{
							pFixNode = pFixNodeParent;
						}
						else if (pNode->pRightChild->colour == RBTreeNodeColours::BLACK &&
							pNode->pLeftChild->colour == RBTreeNodeColours::BLACK)
						{
							pNode->colour = RBTreeNodeColours::RED;
							pFixNode = pFixNodeParent;
						}
						else
						{
							if (pNode->pLeftChild->colour == RBTreeNodeColours::BLACK)
							{
								pNode->pRightChild->colour = RBTreeNodeColours::BLACK;
								pNode->colour = RBTreeNodeColours::RED;
								rotateLeft(pNode);
								pNode = pFixNodeParent->pLeftChild;
							}

							pNode->colour = pFixNodeParent->colour;
							pFixNodeParent->colour = RBTreeNodeColours::BLACK;
							pNode->pLeftChild->colour = RBTreeNodeColours::BLACK;
							rotateRight(pFixNodeParent);
							break;
						}
					}
				}
			}

			--m_uiNumElements;

			m_pCachedElement = nullptr;
			return true;
		}

		/**
		Finds and returns the successor node for the given node.
		@param pSubtreeRoot A pointer to the root of the subtree whose successor to find 
		@return A pointer to the successor, or a pointer to the root node */
		RBTreeNode<ElementType>* successor(RBTreeNode<ElementType>* pSubtreeRoot)
		{
			// Lowest value in right subtree
			if (!pSubtreeRoot->pRightChild->bIsLeaf)
			{
				return min(pSubtreeRoot->pRightChild);
			}

			// Or traverse upwards until RBTree root
			RBTreeNode<ElementType>* pParent = pSubtreeRoot;
			while (pParent != m_pHeadNode && pParent->pRightChild == pSubtreeRoot)
			{
				pSubtreeRoot = pParent;
				pParent = pParent->pParent;
			}

			// No successor
			return pParent;
		}

		/**
		Finds and returns the predecessor node for the given node.
		@param pSubtreeRoot A pointer to the root of the subtree whose predecessor to find
		@return A pointer to the predecessor, or a pointer to the root node */
		RBTreeNode<ElementType>* predecessor(RBTreeNode<ElementType>* pSubtreeRoot)
		{
			// Highest value in left subtree
			if (!pSubtreeRoot->pLeftChild->bIsLeaf)
			{
				return max(pSubtreeRoot->pLeftChild);
			}

			// Or traverse upwards until RBTree root
			RBTreeNode<ElementType>* pParent = pSubtreeRoot;
			while (pParent != m_pHeadNode && pParent->pLeftChild == pSubtreeRoot)
			{
				pSubtreeRoot = pParent;
				pParent = pParent->pParent;
			}

			// No predecessor
			return pParent;
		}

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

		/**
		Copies a subtree and returns a pointer to the newly constructed root node. If the subtree
		root was a nullptr, the returned value will also be a nullptr.
		@param pSubtreeRoot A pointer to the subtree root
		@return A pointer to the newly constructed root node, or a nullptr */
		RBTreeNode<ElementType>* copySubtree(RBTreeNode<ElementType>* pSubtreeRoot)
		{
			// Subtree is null so root is nullptr
			if (isnullptr(pSubtreeRoot))
			{
				return nullptr;
			}

			RBTreeNode<ElementType>* pNewRoot = new RBTreeNode<ElementType>(*pSubtreeRoot);

			return pNewRoot;
		}

	protected:

	public:
		typedef RBTreeIterator<ElementType> Iterator;
		typedef RBTreeConstIterator<ElementType> ConstIterator;

		/**
		Constructs a red-black tree. */
		RBTree()
		{
			m_uiNumElements = 0;
			m_uiMaxElements = std::numeric_limits<unsigned int>::max();

			m_pRootNode = nullptr;
			m_pHeadNode = nullptr;
			m_pCachedElement = nullptr;
			m_pLeftMost = nullptr;
		}

		/**
		Copy constructor. 
		@param other The tree to copy */
		RBTree(const RBTree<ElementType, KeyType, Comparator, ElementBuilder>& other)
		{
			m_uiNumElements = other.m_uiNumElements;
			m_uiMaxElements = other.m_uiMaxElements;

			m_comparator = other.m_comparator;
			m_elementBuilder = other.m_elementBuilder;

			if (m_uiNumElements == 0)
			{
				m_pRootNode = nullptr;
				m_pHeadNode = nullptr;
				m_pCachedElement = nullptr;
				m_pLeftMost = nullptr;
			}
			else
			{
				m_pRootNode = copySubtree(other.m_pRootNode);
				buildAndOrBindHead();
				m_pCachedElement = m_pRootNode;
				m_pLeftMost = min(m_pRootNode);
			}
		}

		/**
		Assignment operator.
		@param other The tree to copy
		@return The copied tree */
		RBTree<ElementType, KeyType, Comparator, ElementBuilder>& 
			operator=(const RBTree<ElementType, KeyType, Comparator, ElementBuilder>& other)
		{
			m_uiNumElements = other.m_uiNumElements;
			m_uiMaxElements = other.m_uiMaxElements;

			m_comparator = other.m_comparator;
			m_elementBuilder = other.m_elementBuilder;

			if (m_uiNumElements == 0)
			{
				m_pRootNode = nullptr;
				m_pHeadNode = nullptr;
				m_pCachedElement = nullptr;
				m_pLeftMost = nullptr;
			}
			else
			{
				m_pRootNode = copySubtree(other.m_pRootNode);
				buildAndOrBindHead();
				m_pCachedElement = m_pRootNode;
				m_pLeftMost = min(m_pRootNode);
			}

			return *this;
		}

		/**
		Destructs the tree and all elements within. */
		~RBTree()
		{
			delete m_pRootNode;
			delete m_pHeadNode;
		}

		/**
		Destructs all elements. Functionally equivalent to clear. */
		void reset()
		{
			delete m_pRootNode;
			m_pRootNode = nullptr;

			m_uiNumElements = 0;
			m_pCachedElement = nullptr;
			m_pLeftMost = nullptr;
		}

		/**
		Destructs all elements. Functionally equivalent to reset. */
		void clear()
		{
			delete m_pRootNode;
			m_pRootNode = nullptr;

			m_uiNumElements = 0;
			m_pCachedElement = nullptr;
			m_pLeftMost = nullptr;
		}

		/**
		Sets the maximum number of elements that the tree may contain. This function will not
		remove elements if the maximum is set to below the current number of elements.
		@param uiMax The desired maximum */
		void setMaxElements(unsigned int uiMax)
		{
			m_uiMaxElements = uiMax;
		}

		/**
		Returns the maximum number of elements that the tree may contain.
		@return The maximum number of elements */
		unsigned int maxElements() const
		{
			return m_uiMaxElements;
		}

		/**
		Returns the current number of elements that the tree contains.
		@return The current number of elements */
		unsigned int numElements() const
		{
			return m_uiNumElements;
		}

		/**
		Queries the existence of an element within the tree.
		@param key The key of the element to find
		@return True if the element existed, false if it did not */
		bool exists(const KeyType& key)
		{
			if (m_uiNumElements == 0)
			{
				return false;
			}
			else if (notnullptr(m_pCachedElement) &&
					!m_comparator(key, m_pCachedElement->element) &&
					!m_comparator(m_pCachedElement->element, key))
			{
				// Check the cached element
				return true;
			}
			else
			{
				// Search tree for element
				RBTreeNode<ElementType>* pCurrentNode = m_pRootNode;
				while (!pCurrentNode->bIsLeaf)
				{
					if (!m_comparator(key, pCurrentNode->element) &&
						!m_comparator(pCurrentNode->element, key))
					{
						m_pCachedElement = pCurrentNode;
						return true;
					}
					else
					{
						if (m_comparator(key, pCurrentNode->element))
						{
							pCurrentNode = pCurrentNode->pLeftChild;
						}
						else
						{
							pCurrentNode = pCurrentNode->pRightChild;
						}
					}
				}

				return false;
			}
		}

		/**
		Queries the existence of an element within the tree.
		@param pElement A pointer to the element to search for
		@return True if the element existed, false if it did not */
		bool exists(typename std::conditional<std::is_pointer<KeyType>::value, FakeParam,
			const KeyType*>::type pElement) 
		{
			if (m_uiNumElements == 0)
			{
				return false;
			}
			else if (notnullptr(m_pCachedElement) &&
				!m_comparator(*pElement, m_pCachedElement->element) &&
				!m_comparator(m_pCachedElement->element, *pElement))
			{
				// Check the cached element
				if (m_comparator(pElement, &(m_pCachedElement->element)))
				{
					return true;
				}
				return false;
			}
			else
			{
				// Search tree for element
				RBTreeNode<ElementType>* pCurrentNode = m_pRootNode;
				while (!pCurrentNode->bIsLeaf)
				{
					if (!m_comparator(*pElement, pCurrentNode->element) &&
						!m_comparator(pCurrentNode->element, *pElement))
					{
						if (m_comparator(pElement, &(pCurrentNode->element)))
						{
							m_pCachedElement = pCurrentNode;
							return true;
						}
						return false;
					}
					else
					{
						if (m_comparator(*pElement, pCurrentNode->element))
						{
							pCurrentNode = pCurrentNode->pLeftChild;
						}
						else
						{
							pCurrentNode = pCurrentNode->pRightChild;
						}
					}
				}

				return false;
			}
		}

		/**
		Retrieves a pointer to an element in the tree that compares equal to the given key.
		If no such element existed, a nullptr is returned instead.
		@param key The key of the element to get
		@return A pointer to an element, or a nullptr
		@see get */
		ElementType* tryToGet(const KeyType& key)
		{
			if (m_uiNumElements == 0)
			{
				return nullptr;
			}
			else if (notnullptr(m_pCachedElement) &&
				!m_comparator(key, m_pCachedElement->element) &&
				!m_comparator(m_pCachedElement->element, key))
			{
				// Check the cached element
				return &m_pCachedElement->element;
			}
			else
			{
				// Search tree
				RBTreeNode<ElementType>* pCurrentNode = m_pRootNode;
				while (!pCurrentNode->bIsLeaf)
				{
					if (!m_comparator(key, pCurrentNode->element) &&
						!m_comparator(pCurrentNode->element, key))
					{
						m_pCachedElement = pCurrentNode;
						return &pCurrentNode->element;
					}
					else
					{
						if (m_comparator(key, pCurrentNode->element))
						{
							pCurrentNode = pCurrentNode->pLeftChild;
						}
						else
						{
							pCurrentNode = pCurrentNode->pRightChild;
						}
					}
				}

				return nullptr;
			}
		}

		/**
		Retrieves a reference to an element in the tree that compares equal to the given key.
		If no such element existed, it is inserted with a default value. If the insertion of the
		default element fails then undefined behaviour is invoked.
		@param key The key of the element to get
		@return A reference to an element
		@see tryToGet
		@see exists */
		ElementType& get(const KeyType& key)
		{
			if (m_uiNumElements == 0)
			{
				if (performInsertion(m_elementBuilder.build(key)))
				{
					return m_pCachedElement->element;
				}
				else
				{
					fatalexit("Attempting to retrieve non-existent element in tree. Insertion of default failed.");
					return m_pCachedElement->element;
				}
			}
			else if (notnullptr(m_pCachedElement) &&
				!m_comparator(key, m_pCachedElement->element) &&
				!m_comparator(m_pCachedElement->element, key))
			{
				// Check the cached element
				return m_pCachedElement->element;
			}
			else
			{
				// Search tree
				RBTreeNode<ElementType>* pCurrentNode = m_pRootNode;
				while (!pCurrentNode->bIsLeaf)
				{
					if (!m_comparator(key, pCurrentNode->element) &&
						!m_comparator(pCurrentNode->element, key))
					{
						m_pCachedElement = pCurrentNode;
						return pCurrentNode->element;
					}
					else
					{
						if (m_comparator(key, pCurrentNode->element))
						{
							pCurrentNode = pCurrentNode->pLeftChild;
						}
						else
						{
							pCurrentNode = pCurrentNode->pRightChild;
						}
					}
				}

				// Couldnt find. Try insert default
				if (performInsertion(m_elementBuilder.build(key)))
				{
					return m_pCachedElement->element;
				}
				else
				{
					fatalexit("Attempting to retrieve non-existent element in tree. Insertion of default failed.");
					return m_pCachedElement->element;
				}
			}
		}

		/**
		Inserts an element in to the tree. This function will fail if inserting the element would
		cause the tree to exceed its maximum capacity or the element already existed.
		@param element The element to insert
		@return True if the element was inserted, false otherwise */
		bool insert(const ElementType& element)
		{
			return performInsertion(element);
		}

		/**
		Searches for and removes an element identified by the given key.
		@param key The key of the element to remove
		@return True if the element was removed, false if it did not exist */
		bool remove(const KeyType& key)
		{
			if (m_uiNumElements == 0)
			{
				return false;
			}
			else
			{
				return performRemoval(key);
			}
		}

		/**
		Creates an iterator targetting the first element. When the vector is empty this iterator is
		equal to the iterator created via a call to end().
		@return An iterator targetting the first element in the vector
		@see end */
		Iterator begin() const
		{
			return Iterator(m_pLeftMost);
		}

		/**
		Creates an iterator targetting the theoretical element one past the last element in the
		vector.
		@return An iterator targetting the theoretical element one past the last element */
		Iterator end() const
		{
			return Iterator(m_pHeadNode);
		}

		/**
		Creates a const iterator targetting the first element. When the vector is empty this
		iterator is equal to the iterator created via a call to cend().
		@return A const iterator targetting the first element in the vector */
		ConstIterator cbegin() const
		{
			return ConstIterator(m_pLeftMost);
		}

		/**
		Creates a const iterator targetting the theoretical element one past the last element in
		the vector.
		@return A const iterator targetting the theoretical element one past the last element */
		ConstIterator cend() const
		{
			return ConstIterator(m_pHeadNode);
		}
};

#endif