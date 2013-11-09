#include "Kernel.h"

struct TreeNode
{
    UInt32 key;
    TreeNode * left;
    TreeNode * right;
    Byte payload[0];
};

struct TreeHeader
{
    PoolHeader *nodePool;
    TreeNode *root;
};

PoolHeader * headerPool = NULL;

void KPoolTreeInitialize()
{
    headerPool = new KPoolAllocate('THDR', 1024, sideof(TreeHeader)
}

TreeHeader * KTreeAllocate(UInt32 payloadSize, UInt32 poolSize)
{
    nodePool = KPoolAllocate('TREE', poolSize, sizeof(TreeNode) + payloadSize);
}