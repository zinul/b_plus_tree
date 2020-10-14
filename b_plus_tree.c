#include "b_plus_tree.h"
extern struct b_plus_tree b_plus_tree;

void SplitInternalNode(Node *work_node)
{
    Node *new_internal_node = AllocNode(false);
    index_t min_key_in_new_node;
    for (int i = 0; i < MIN_CACHE_NUM-1 ; i++)
    {
        new_internal_node->keys[i] = work_node->keys[work_node->child_num-i];
        new_internal_node->child_node_ptr[i] = work_node->child_node_ptr[work_node->child_num-i];
        new_internal_node->child_node_ptr[i]->parent_ptr=new_internal_node;
    }    
    min_key_in_new_node = new_internal_node->keys[0];
    work_node->child_num=MIN_CACHE_NUM;
    new_internal_node->child_num=MIN_CACHE_NUM-1;

    new_internal_node->next_node = work_node->next_node;
    work_node->next_node = new_internal_node;

    if (work_node->parent_ptr->child_num == 2 * MIN_CACHE_NUM - 1)
    {
        // new_parent_node = AllocNode(false);
        SplitInternalNode(work_node->parent_ptr);
        // 记得把新节点所有子结点的父节点修改为新的
        if(min_key_in_new_node>work_node->parent_ptr->keys[work_node->parent_ptr->child_num-1])
        {
            new_internal_node->parent_ptr=work_node->parent_ptr->next_node;
        }
        else
        {
            new_internal_node->parent_ptr=work_node->parent_ptr;
        }
    }
    InsertInternalNode(new_internal_node->parent_ptr, min_key_in_new_node, new_internal_node);
}

void SplitLeafNode(Node *work_node)
{
    Node *new_leaf_node = AllocNode(true);
    Node *new_parent_node;
    index_t min_key_in_new_node;
    for (int i = 0; i < MIN_CACHE_NUM-1 ; i++)
    {
        new_leaf_node->keys[i] = work_node->keys[work_node->child_num-i];
        new_leaf_node->values[i] = work_node->values[work_node->child_num-i];
    }
    min_key_in_new_node = new_leaf_node->keys[0];
    work_node->child_num = MIN_CACHE_NUM;
    new_leaf_node->child_num = MIN_CACHE_NUM - 1;

    new_leaf_node->next_node = work_node->next_node;
    work_node->next_node = new_leaf_node;

    if (work_node->parent_ptr->child_num == 2 * MIN_CACHE_NUM - 1)
    {
        // new_parent_node = AllocNode(false);
        SplitInternalNode(work_node->parent_ptr);
        // 记得把新节点所有子结点的父节点修改为新的
        if(min_key_in_new_node>work_node->parent_ptr->keys[work_node->parent_ptr->child_num-1])
        {
            new_leaf_node->parent_ptr=work_node->parent_ptr->next_node;
        }
        else
        {
            new_leaf_node->parent_ptr=work_node->parent_ptr;
        }
    }
    InsertInternalNode(new_leaf_node->parent_ptr, min_key_in_new_node, new_leaf_node);
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
        b_plus_tree.leaf_nums++;
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
struct b_plus_tree BPTreeCreate()
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