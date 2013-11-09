#pragma once

#pragma warning(push)

#pragma warning(disable: 4200)

struct AVLTreeNode
{
	UInt32 key;	
	UIntPtr value;

	AVLTreeNode *left;
	AVLTreeNode *right;
	AVLTreeNode *parent;

	SByte balanceFactor;
};

struct AVLTree
{
	AVLTreeNode   *root;
	UInt32 NextUnused;
	AVLTreeNode *firstUnused;	// Unsorted
	AVLTreeNode *firstFree;		// Sorted smallest first
	AVLTreeNode nodes[0];
};

AVLTree * AVLTreeInit(UIntPtr addr);
void AVLTreeInsert(AVLTree* pthis, UInt32 key, UIntPtr value);
UIntPtr AVLTreeSearch(AVLTree* pthis, UInt32 key);
UIntPtr AVLTreeRemove(AVLTree* pthis, UInt32 key);

#pragma warning(pop)
