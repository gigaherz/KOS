#include "Kernel.h"

void AVLTree_insertNode(AVLTree* pthis, AVLTreeNode *newNode);
void AVLTree_restoreAVL(AVLTree* pthis, AVLTreeNode *ancestor, AVLTreeNode *newNode);
void AVLTree_adjustBalanceFactors(AVLTree* pthis, AVLTreeNode *end, AVLTreeNode *start);
void AVLTree_rotateLeft(AVLTree* pthis, AVLTreeNode *n);
void AVLTree_rotateRight(AVLTree* pthis, AVLTreeNode *n);
void AVLTree_adjustLeftRight(AVLTree* pthis, AVLTreeNode *end, AVLTreeNode *start);
void AVLTree_adjustRightLeft(AVLTree* pthis, AVLTreeNode *end, AVLTreeNode *start);

//------------------------------------------------------------------
// Default constructor
//------------------------------------------------------------------
AVLTree * AVLTreeInit(UIntPtr addr)
{
	AVLTree * tree = (AVLTree*) addr;
	tree->root = NULL;   // Initialize root to NULL
	return tree;
}

void AVLTreeFreeNode(AVLTreeNode *node)
{
	// TODO
}

//------------------------------------------------------------------
// Recursively delete all node in the tree.
//------------------------------------------------------------------
void AVLTreeFreeSubtree(AVLTreeNode *n)
{
	if (n != NULL)
	{
		AVLTreeFreeSubtree(n->left);   // Recursively clear the left sub-tree
		AVLTreeFreeSubtree(n->right);   // Recursively clear the right sub-tree
		AVLTreeFreeNode(n);         // Delete this node
	}
}

//------------------------------------------------------------------
// Insert()
// Insert a new node into the tree then restore the AVL property.
// Assumes that all three pointers (left, right, and parent) have
//  already been set to NULL in the new node
//------------------------------------------------------------------
void AVLTree_insertNode(AVLTree* pthis, AVLTreeNode *newNode)
{
	AVLTreeNode *temp, *back, *ancestor;

	temp = pthis->root;
	back = NULL;
	ancestor = NULL;

	// Check for empty tree first
	if (pthis->root == NULL)
	{
		pthis->root = newNode;
		return;
	}
	// Tree is not empty so search for place to insert
	while (temp != NULL) // Loop till temp falls out of the tree 
	{
		back = temp;
		// Mark ancestor that will be out of balance after
		//   this node is inserted
		if (temp->balanceFactor != '=')
			ancestor = temp;
		if (newNode->key < temp->key)
			temp = temp->left;
		else
			temp = temp->right;
	}
	// temp is now NULL
	// back points to parent node to attach newNode to
	// ancestor points to most recent out of balance ancestor

	newNode->parent = back;   // Set parent
	if (newNode->key < back->key)  // Insert at left
	{
		back->left = newNode;
	}
	else     // Insert at right
	{
		back->right = newNode;
	}

	// Now call function to restore the tree's AVL property
	AVLTree_restoreAVL(pthis, ancestor, newNode);
}

//------------------------------------------------------------------
// restoreAVL() 
// Restore the AVL quality after inserting a new node.
// @param ancestor – most recent node back up the tree that is
//            now out of balance.
// @param newNode– the newly inserted node.
//------------------------------------------------------------------
void AVLTree_restoreAVL(AVLTree* pthis, AVLTreeNode *ancestor, AVLTreeNode *newNode)
{
	//--------------------------------------------------------------------------------
	// Case 1: ancestor is NULL, i.e. balanceFactor of all ancestors' is '='
	//--------------------------------------------------------------------------------
	if (ancestor == NULL)
	{
		if (newNode->key < pthis->root->key)       // newNode inserted to left of root
			pthis->root->balanceFactor = 'L';
		else
			pthis->root->balanceFactor = 'R';   // newNode inserted to right of root
		// Adjust the balanceFactor for all nodes from newNode back up to root
		AVLTree_adjustBalanceFactors(pthis, pthis->root, newNode);
	}

	//--------------------------------------------------------------------------------
	// Case 2: Insertion in opposite subtree of ancestor's balance factor, i.e.
	//  ancestor.balanceFactor = 'L' AND  Insertion made in ancestor's right subtree
	//     OR
	//  ancestor.balanceFactor = 'R' AND  Insertion made in ancestor's left subtree
	//--------------------------------------------------------------------------------
	else if (((ancestor->balanceFactor == 'L') && (newNode->key > ancestor->key)) ||
		((ancestor->balanceFactor == 'R') && (newNode->key < ancestor->key)))
	{
		ancestor->balanceFactor = '=';
		// Adjust the balanceFactor for all nodes from newNode back up to ancestor
		AVLTree_adjustBalanceFactors(pthis, ancestor, newNode);
	}
	//--------------------------------------------------------------------------------
	// Case 3: ancestor.balanceFactor = 'R' and the node inserted is
	//      in the right subtree of ancestor's right child
	//--------------------------------------------------------------------------------
	else if ((ancestor->balanceFactor == 'R') && (newNode->key > ancestor->right->key))
	{
		ancestor->balanceFactor = '='; // Reset ancestor's balanceFactor
		AVLTree_rotateLeft(pthis, ancestor);       // Do single left rotation about ancestor
		// Adjust the balanceFactor for all nodes from newNode back up to ancestor's parent
		AVLTree_adjustBalanceFactors(pthis, ancestor->parent, newNode);
	}

	//--------------------------------------------------------------------------------
	// Case 4: ancestor.balanceFactor is 'L' and the node inserted is
	//      in the left subtree of ancestor's left child
	//--------------------------------------------------------------------------------
	else if ((ancestor->balanceFactor == 'L') && (newNode->key < ancestor->left->key))
	{
		ancestor->balanceFactor = '='; // Reset ancestor's balanceFactor
		AVLTree_rotateRight(pthis, ancestor);       // Do single right rotation about ancestor
		// Adjust the balanceFactor for all nodes from newNode back up to ancestor's parent
		AVLTree_adjustBalanceFactors(pthis, ancestor->parent, newNode);
	}

	//--------------------------------------------------------------------------------
	// Case 5: ancestor.balanceFactor is 'L' and the node inserted is
	//      in the right subtree of ancestor's left child
	//--------------------------------------------------------------------------------
	else if ((ancestor->balanceFactor == 'L') && (newNode->key > ancestor->left->key))
	{
		// Perform double right rotation (actually a left followed by a right)
		AVLTree_rotateLeft(pthis, ancestor->left);
		AVLTree_rotateRight(pthis, ancestor);
		// Adjust the balanceFactor for all nodes from newNode back up to ancestor
		AVLTree_adjustLeftRight(pthis, ancestor, newNode);
	}

	//--------------------------------------------------------------------------------
	// Case 6: ancestor.balanceFactor is 'R' and the node inserted is 
	//      in the left subtree of ancestor's right child
	//--------------------------------------------------------------------------------
	else
	{
		// Perform double left rotation (actually a right followed by a left)
		AVLTree_rotateRight(pthis, ancestor->right);
		AVLTree_rotateLeft(pthis, ancestor);
		AVLTree_adjustRightLeft(pthis, ancestor, newNode);
	}
}

//------------------------------------------------------------------
// Adjust the balance factor in all nodes from the inserted node's
//   parent back up to but NOT including a designated end node.
// @param end– last node back up the tree that needs adjusting
// @param start – node just inserted 
//------------------------------------------------------------------
void AVLTree_adjustBalanceFactors(AVLTree* pthis, AVLTreeNode *end, AVLTreeNode *start)
{
	AVLTreeNode *temp = start->parent; // Set starting point at start's parent
	while (temp != end)
	{
		if (start->key < temp->key)
			temp->balanceFactor = 'L';
		else
			temp->balanceFactor = 'R';
		temp = temp->parent;
	} // end while
}

//------------------------------------------------------------------
// rotateLeft()
// Perform a single rotation left about n.  This will rotate n's
//   parent to become n's left child.  Then n's left child will
//   become the former parent's right child.
//------------------------------------------------------------------
void AVLTree_rotateLeft(AVLTree* pthis, AVLTreeNode *n)
{
	AVLTreeNode *temp = n->right;   //Hold pointer to n's right child
	n->right = temp->left;      // Move temp 's left child to right child of n
	if (temp->left != NULL)      // If the left child does exist
		temp->left->parent = n;// Reset the left child's parent
	if (n->parent == NULL)       // If n was the root
	{
		pthis->root = temp;      // Make temp the new root
		temp->parent = NULL;   // Root has no parent
	}
	else if (n->parent->left == n) // If n was the left child of its' parent
		n->parent->left = temp;   // Make temp the new left child
	else               // If n was the right child of its' parent
		n->parent->right = temp;// Make temp the new right child

	temp->left = n;         // Move n to left child of temp
	n->parent = temp;         // Reset n's parent
}

//------------------------------------------------------------------
// rotateRight()
// Perform a single rotation right about n.  This will rotate n's
//   parent to become n's right child.  Then n's right child will
//   become the former parent's left child.
//------------------------------------------------------------------
void AVLTree_rotateRight(AVLTree* pthis, AVLTreeNode *n)
{
	AVLTreeNode *temp = n->left;   //Hold pointer to temp
	n->left = temp->right;      // Move temp's right child to left child of n
	if (temp->right != NULL)      // If the right child does exist
		temp->right->parent = n;// Reset right child's parent
	if (n->parent == NULL)       // If n was root
	{
		pthis->root = temp;      // Make temp the root
		temp->parent = NULL;   // Root has no parent
	}
	else if (n->parent->left == n) // If was the left child of its' parent
		n->parent->left = temp;   // Make temp the new left child
	else               // If n was the right child of its' parent
		n->parent->right = temp;// Make temp the new right child

	temp->right = n;         // Move n to right child of temp
	n->parent = temp;         // Reset n's parent
}

//------------------------------------------------------------------
// adjustLeftRight()
// @param end- last node back up the tree that needs adjusting
// @param start - node just inserted 
//------------------------------------------------------------------
void AVLTree_adjustLeftRight(AVLTree* pthis, AVLTreeNode *end, AVLTreeNode *start)
{
	if (end == pthis->root)
		end->balanceFactor = '=';
	else if (start->key < end->parent->key)
	{
		end->balanceFactor = 'R';
		AVLTree_adjustBalanceFactors(pthis, end->parent->left, start);
	}
	else
	{
		end->balanceFactor = '=';
		end->parent->left->balanceFactor = 'L';
		AVLTree_adjustBalanceFactors(pthis, end, start);
	}
}

//------------------------------------------------------------------
// adjustRightLeft
// @param end- last node back up the tree that needs adjusting
// @param start - node just inserted 
//------------------------------------------------------------------
void AVLTree_adjustRightLeft(AVLTree* pthis, AVLTreeNode *end, AVLTreeNode *start)
{
	if (end == pthis->root)
		end->balanceFactor = '=';
	else if (start->key > end->parent->key)
	{
		end->balanceFactor = 'L';
		AVLTree_adjustBalanceFactors(pthis, end->parent->right, start);
	}
	else
	{
		end->balanceFactor = '=';
		end->parent->right->balanceFactor = 'R';
		AVLTree_adjustBalanceFactors(pthis, end, start);
	}
}
