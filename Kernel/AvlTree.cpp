#include "Kernel.h"

struct TreeNode {
    UInt32 data;
    TreeNode *left;
    TreeNode *right;
};

struct AvlTree {
	TreeNode *root;
	TreeNode *firstFree;
	UInt32 reservedSpace;
	Byte nodes[0];
};

// AVL Tree Implementation in C
//An AVL tree is a self-FixBalance binary search tree. It was invented by G.M. Adelson-Velski and E.M. Landis. In an AVL tree, heights of the two paired subtrees of any node differ by at most one and maintain an O(logn) search time. An AVL tree has the following properties:
//     1. Every sub-tree is an AVL tree.
//     2. The sub-trees of every node differ in height by at most one.
//The balance factor of a node is the height of its left subtree minus the height of its right subtree (sometimes opposite) and a node with balance factor 1, 0, or ?1 is considered balanced. A node with any other balance factor is considered unbalanced and requires rebalancing the tree. The balance factor is either stored directly at each node or computed from the heights of the subtrees. The following is an example for a balanced tree.
//
//
//Balance factor of root node = height of left sub-tree - height of right sub-tree. Here, "4" is the root node. Its balance factor =0.
//
static Int32 FindHeight(TreeNode *node)
{
    if (node != NULL)
	{
        Int32 lHeight = FindHeight(node->left);
        Int32 rHeight = FindHeight(node->right);

        Int32 max = (lHeight > rHeight) ? lHeight : rHeight;

        return 1 + max;
    }

    return 0;
}

static Int32 GetDifference(TreeNode *node)
{
    Int32 lHeight = FindHeight(node->left);
    Int32 rHeight = FindHeight(node->right);
    Int32 b_factor= lHeight - rHeight;
    return b_factor;
}  

//
//Operations on AVL Tree
//The basic operation that carried out on an unbalanced binary search tree is the tree rotations. It helps to restore the height balance of the sub-trees.
//Insertion
//After inserting a node, it is necessary to check whether  the trees satisfies the rules of AVL. For each node checked, if the balance factor remains ?1, 0, or +1 then no rotations are necessary. However, if the balance factor becomes ±2 then the subtree rooted at this node is unbalanced. Rotations are necessary in this case.
// There are four cases which need to be considered. Let P be the root of the unbalanced subtree, with R and L denoting the right and left children of P respectively.
//
static TreeNode* RotateRight(TreeNode *parent)
{
	//1. Right-Right case 
	//
	//If the balance factor of P is -2 then the right subtree outweights the left subtree of the given node, and the balance factor of the right child (R) must be checked. If the balance factor of R is -1, a single left rotation (with P as the root) is needed.
	//The code for Right-Right rotation is ,
	//
    TreeNode *temp = parent->right;
    parent->right  = temp->left;
    temp->left     = parent;
    return temp;
}

static TreeNode* RotateLeft(TreeNode *parent)
{
	//3. Left-Left case
	//
	//If the balance factor of P is +2, then the left subtree outweighs the right subtree of the given node, and the balance factor of the left child (L) must be checked. 
	//If the balance factor of L is +1, a single right rotation (with P as the root) is needed.
	//Code for Left-Left rotation is,
	//
    TreeNode *temp = parent->left;
    parent->left   = temp->right;
    temp->right    = parent;
    return temp;
}

static TreeNode* RotateLeftOfRight(TreeNode *parent)
{
	//2. Right-Left case 
	//
	//If the balance factor of P is -2 then the right subtree outweights the left subtree of the given node, and the balance factor of the right child (R) must be checked.
	//If the balance factor of R is +1, two different rotations are needed. The first rotation is a right rotation with R as the root. The second is the left rotation with P as the root (Right-Left case).
	//The code for Right-Left rotation is,
	//
    parent->right = RotateLeft(parent->right);
    return RotateRight(parent);
}

static TreeNode* RotateRightOfLeft(TreeNode *parent)
{
	//4. Left-Right case
	//
	//If the balance factor of P is +2, then the left subtree outweighs the right subtree of the given node, and the balance factor of the left child (L) must be checked. If the balance factor of L is -1, two different rotations are needed. The first rotation is a left rotation with L as the root. The second is a right rotation with P as the root.
	//Code for Left-Right rotation is,
	//
    parent->left = RotateRight(parent->left);
    return RotateLeft(parent);
}

//The following functions check the balance factor of the parent nodes, and will decide the necessary rotations,
//
static TreeNode* FixBalance(TreeNode *node)
{
    Int32 balance = GetDifference(node);

    if (balance > 1)
	{
        if (GetDifference(node->left) > 0)
            node = RotateLeft(node);
        else
            node = RotateRightOfLeft(node);
    }
    else if(balance < -1)
	{
        if(GetDifference(node->right) > 0)
            node = RotateLeftOfRight(node);
        else
            node = RotateRight(node);
    }

    return node;
}       

///////////////////////////////////////////////////////////////////////////////
// Public interface
//
TreeNode* GetFirstFree(AvlTree *tree)
{
	TreeNode *node = tree->firstFree;

	if (!node)
		return NULL;

	tree->firstFree = node->right;
	return node;
}

void AddFreeNode(AvlTree *tree)
{
}

TreeNode* TreeInsert(AvlTree *tree, TreeNode *root, UInt32 val)
{
    if (root==NULL) 
	{
		root = GetFirstFree(tree);
        root -> data = val;
        root -> left = NULL;
        root -> right = NULL;
        return root;
    }
    else if (val < root->data) {
        root->left = TreeInsert(tree, root->left, val);
        root=FixBalance(root);
    }
    else if (val > root->data) {
        root->right = TreeInsert(tree, root->right, val);
        root=FixBalance(root);
    }
    return root;
}

TreeNode* TreeFind(AvlTree *tree, UInt32 val)
{
	TreeNode *root = tree->root;

	while(root != NULL)
	{
		if(root->data == val)
			return root;

		if(root->data > val)
			root = root->right;
		else 
			root = root->left;
	}

    return NULL;
}

bool TreeRemove(AvlTree *tree, UInt32 val)
{
	TreeNode *root = tree->root;

	if(root != NULL)
	{
		if(root->data == val)
		{
			if(root->left && root->right)
			{
				tree->root = TreeInsert(tree, root->left, root->right->data);
			}
			else if(root->left)
			{
				tree->root = root->left;
			}
			else
			{
				tree->root = root->right;
			}
			AddFreeNode(root);
		}
	}

	while(root != NULL)
	{
		if(root->data > val)
		{
			root = root->right;
		}
		else 
		{
			root = root->left;
			
			if(root->data == val)
			{

			
				return true;
			}

		}
	}

	// Not found
	return false;
}
