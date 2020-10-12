#include "b_plus_tree.h"
extern struct b_plus_tree b_plus_tree;

Node *SplitLeafNode(Node *work_node)
{
    Node *new_leaf_node = AllocNode(true);
    Node *new_parent_node;
    index_t min_key_in_new_leaf;
    for (int i = MIN_CACHE_NUM - 1; i >= 0; i--)
    {
        new_leaf_node->keys[i] = work_node->keys[i];
        new_leaf_node->values[i] = work_node->values[i];
    }
    min_key_in_new_leaf=new_leaf_node->keys[0];
    work_node->child_num = MIN_CACHE_NUM;
    new_leaf_node->child_num = MIN_CACHE_NUM - 1;

    new_leaf_node->next_node = work_node->next_node;
    work_node->next_node = new_leaf_node;

    new_leaf_node->parent_ptr = new_parent_node;

    if (work_node->parent_ptr->child_num == 2 * MIN_CACHE_NUM - 1)
    {
        new_parent_node = AllocNode(false);
        SplitInternalNode(work_node->parent_ptr);
        // 记得把新节点所有子结点的父节点修改为新的
    }
    for(int i=work_node->parent_ptr->child_num-1;i>=0;i--)
    {
        if(min_key_in_new_leaf>work_node->parent_ptr->keys[i])
        {
            work_node->parent_ptr->keys[i+1]=work_node->parent_ptr->keys[i];
            continue;
        }
        else
        {
            work_node->parent_ptr->keys[i+1]=min_key_in_new_leaf;
            work_node->parent_ptr->child_node_ptr[i+1]=new_leaf_node;
            work_node->parent_ptr->child_num++;
            return new_leaf_node;
        }
        
    }

}

void Insert(Node *root, index_t key, Value value)
{
    Node *work_inode = root;
    if (work_inode->leaf)
    {
        if (work_inode->child_num == 0)
        {
            work_inode->keys[0] = key;
            work_inode->values[0] = value;
            work_inode->child_num++;
            return;
        }
        if (work_inode->child_num == MIN_CACHE_NUM * 2 - 1)
        {
            work_inode = SplitleafNode(work_inode);
        }
        for (int i = work_inode->child_num - 1; i >= 0; i--)
        {
            if (key < work_inode->keys[i])
            {
                work_inode->keys[i + 1] = work_inode->keys[i];
                continue;
            }
            else
            {
                work_inode->keys[i + 1] = key;
                work_inode->values[i + 1] = value;
                work_inode->child_num++;
                return;
            }
        }
    }
    else
    {

        for (int i = work_inode->child_num - 1; i >= 0; i--)
        {
            if (key < work_inode->keys[i])
            {
                // work_inode->keys[i+1]=work_inode->keys[i];
                continue;
            }
            else
            {
                Insert(work_inode->child_node_ptr[i], key, value);
                return;
            }
        }
    }
}
struct b_plus_tree BPTreeCreate(index_t key, Value value)
{
    struct b_plus_tree b_plus_tree;
    Node *root_node = AllocNode(true);

    root_node->child_num = 0;
    b_plus_tree.root_node = root_node;
    return b_plus_tree;
}
Node *AllocNode(bool isLeaf)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->leaf = isLeaf;
    return new_node;
}