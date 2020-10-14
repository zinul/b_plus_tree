#include "b_plus_tree.h"
extern struct b_plus_tree b_plus_tree;

Node *SplitLeafNode(Node *work_node)
{
    Node *new_leaf_node = AllocNode(true);
    Node *new_parent_node;
    index_t min_key_in_new_leaf;
    for (int i = 0; i < MIN_CACHE_NUM-1 ; i++)
    {
        new_leaf_node->keys[i] = work_node->keys[work_node->child_num-i];
        new_leaf_node->values[i] = work_node->values[work_node->child_num-i];
    }
    min_key_in_new_leaf = new_leaf_node->keys[0];
    work_node->child_num = MIN_CACHE_NUM;
    new_leaf_node->child_num = MIN_CACHE_NUM - 1;

    new_leaf_node->next_node = work_node->next_node;
    work_node->next_node = new_leaf_node;

    if (work_node->parent_ptr->child_num == 2 * MIN_CACHE_NUM - 1)
    {
        // new_parent_node = AllocNode(false);
        SplitInternalNode(work_node->parent_ptr);
        // 记得把新节点所有子结点的父节点修改为新的
        if(min_key_in_new_leaf>work_node->parent_ptr->keys[work_node->parent_ptr->child_num-1])
        {
            new_leaf_node->parent_ptr=work_node->parent_ptr->next_node;
        }
        else
        {
            new_leaf_node->parent_ptr=work_node->parent_ptr;
        }
    }
    InsertInternalNode(new_leaf_node->parent_ptr, min_key_in_new_leaf, new_leaf_node);

    // for(int i=work_node->parent_ptr->child_num-1;i>=0;i--)
    // {
    //     if(min_key_in_new_leaf>work_node->parent_ptr->keys[i])
    //     {
    //         work_node->parent_ptr->keys[i+1]=work_node->parent_ptr->keys[i];
    //         continue;
    //     }
    //     else
    //     {
    //         work_node->parent_ptr->keys[i+1]=min_key_in_new_leaf;
    //         work_node->parent_ptr->child_node_ptr[i+1]=new_leaf_node;
    //         work_node->parent_ptr->child_num++;
    //         return new_leaf_node;
    //     }

    // }
}

void Insert(Node *work_node, index_t key, Value value)
{
    if (work_node->leaf)
    {
        if (work_node->child_num == 0)
        {
            work_node->keys[0] = key;
            work_node->values[0] = value;
            work_node->child_num++;
            return;
        }
        if (work_node->child_num == MIN_CACHE_NUM * 2 - 1)
        {
            SplitLeafNode(work_node);
            if(key>work_node->keys[work_node->child_num-1])
            {
                work_node=work_node->next_node;
            }
        }
        InsertLeafNode(work_node, key, value);
    }
    else
    {
        int i=SearchInsertPos(work_node,key);
        Insert(work_node->child_node_ptr[i], key, value);
    }
        return;
}
int SearchInsertPos(Node *work_node, index_t key)
{
    for (int i = work_node->child_num - 1; i >= 0; i--)
    {
        if (key < work_node->keys[i])
        {
            continue;
        }
        else
        {
            return i;
        }
    }    
}

void InsertLeafNode(Node *work_node, index_t key, Value value)
{
    for (int i = work_node->child_num - 1; i >= 0; i--)
    {
        if (key < work_node->keys[i])
        {
            work_node->keys[i + 1] = work_node->keys[i];
            continue;
        }
        else
        {
            work_node->keys[i + 1] = key;
            work_node->values[i + 1] = value;
            work_node->child_num++;
            return;
        }
    }
}
void InsertInternalNode(Node *work_node, index_t key, Node *new_node)
{
    for (int i = work_node->child_num - 1; i >= 0; i--)
    {
        if (key < work_node->keys[i])
        {
            work_node->keys[i + 1] = work_node->keys[i];
            continue;
        }
        else
        {
            work_node->keys[i + 1] = key;
            work_node->parent_ptr->child_node_ptr[i + 1] = new_node;
            work_node->child_num++;
            return;
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