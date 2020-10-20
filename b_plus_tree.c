#include <stdio.h>
#include <memory.h>
#include "b_plus_tree.h"

extern struct b_plus_tree b_plus_tree;
extern Node *first_leaf_node;
int GetTreeHead(char path[])
{
    FILE *fp;
    int node_size;
    if ((fp = fopen(path, "r")) == NULL)
    {
        printf("open error\n");
        return -1;
    }
    fread(&b_plus_tree.node_nums, sizeof(int), 1, fp);
    fread(&b_plus_tree.leaf_nums, sizeof(int), 1, fp);
    fread(&b_plus_tree.root_disk_pos, sizeof(int), 1, fp);
    fread(&node_size, sizeof(int), 1, fp);

    fclose(fp);
    return 0;
}
int PutTreeHead(char path[])
{
    FILE *fp;
    int node_size = sizeof(DiskNode);
    if ((fp = fopen(path, "w")) == NULL)
    {
        printf("open error\n");
        return -1;
    }
    // fseek(fp,DATA_BEGIN,SEEK_SET);
    fwrite(&b_plus_tree.node_nums, sizeof(int), 1, fp);
    fwrite(&b_plus_tree.leaf_nums, sizeof(int), 1, fp);
    fwrite(&b_plus_tree.root_disk_pos, sizeof(int), 1, fp);
    fwrite(&node_size, sizeof(int), 1, fp);
    fclose(fp);
    return 0;
}
DiskNode *CreateDiskNode(Node *mem_node)
{
    DiskNode *disk_node = malloc(sizeof(DiskNode));
    memcpy(disk_node->keys, mem_node->keys, sizeof(disk_node->keys));
    memcpy(disk_node->values, mem_node->values, sizeof(disk_node->values));

    disk_node->leaf = mem_node->leaf;
    disk_node->child_num = mem_node->child_num;
    if (mem_node->next_node)
        disk_node->next_node_num = mem_node->next_node->node_num;
    else
    {
        disk_node->next_node_num = -1;
    }

    disk_node->node_num = mem_node->node_num;
    return disk_node;
}
void NumberTheTree()
{
    Node *work_node = b_plus_tree.root_node;
    int index = 0;
    while (!work_node->leaf)
    {
        Node *first_node = work_node;
        while (work_node)
        {
            work_node->node_num = index++;
            // printf("%u ", work_node->node_num);
            work_node = work_node->next_node;
        }
        // printf("\n");
        work_node = first_node->child_node_ptr[0];
    }
    work_node = first_leaf_node;
    while (work_node)
    {
        work_node->node_num = index++;
        work_node = work_node->next_node;
    }
    return;
}
Node *CreateMemNode(DiskNode *disk_node)
{
    Node *mem_node = malloc(sizeof(Node));
    memcpy(mem_node->keys, disk_node->keys, sizeof(disk_node->keys));
    memcpy(mem_node->values, disk_node->values, sizeof(disk_node->values));

    mem_node->leaf = disk_node->leaf;
    mem_node->child_num = disk_node->child_num;
    mem_node->node_num = disk_node->node_num;

    return mem_node;
}
void ReadDiskNode(DiskNode *disk_node, int node_num,FILE *fp)
{
    fseek(fp, DATA_BEGIN + node_num * sizeof(DiskNode), SEEK_SET);
    fread(disk_node, sizeof(DiskNode), 1, fp);
    return ;
}
int GetAllTree(char path[])
{
    FILE *fp;
    DiskNode *work_node = malloc(sizeof(DiskNode));
    DiskNode *first_disk_node;
    DiskNode *parent_disk_node = malloc(sizeof(DiskNode));
    Node *parent_mem_node;
    Node *first_mem_node;
    Node *mem_node;
    if ((fp = fopen(path, "r")) == NULL)
    {
        printf("open error\n");
        return -1;
    }

    GetTreeHead(path);
    ReadDiskNode(parent_disk_node,0,fp);
    parent_mem_node = CreateMemNode(parent_disk_node);
    b_plus_tree.root_node = parent_mem_node;
    printf("%d ",b_plus_tree.node_nums);
    while (!parent_mem_node->leaf)
    {
        first_mem_node = parent_mem_node;
        first_disk_node = parent_disk_node;
        while (parent_mem_node)
        {
            printf("11111%d\n",parent_mem_node->child_num);

            for (int i = 0; i < parent_mem_node->child_num; i++)
            {
                ReadDiskNode(work_node,parent_disk_node->child_node_nums[i],fp);
                mem_node = CreateMemNode(work_node);
                parent_mem_node->child_node_ptr[i] = mem_node;
                printf("%d:%p\n",i,parent_mem_node->child_node_ptr[i]);
                mem_node->parent_ptr = parent_mem_node;
            }
            printf("%d\n",parent_mem_node->child_num);
            for (int i = 0; i < parent_mem_node->child_num; i++)
            {
                printf("%d:%p\n",i,parent_mem_node->child_node_ptr[i]);
                parent_mem_node->child_node_ptr[i]->next_node = parent_mem_node->child_node_ptr[i + 1];
                if (parent_mem_node->child_node_ptr[i + 1])
                    parent_mem_node->child_node_ptr[i + 1]->pre_node = parent_mem_node->child_node_ptr[i];
            }
            if(parent_mem_node->pre_node)
            {
                parent_mem_node->child_node_ptr[0]->pre_node=parent_mem_node->pre_node->child_node_ptr[parent_mem_node->pre_node->child_num-1];
            printf("00000000%p\n",parent_mem_node->pre_node->child_node_ptr[parent_mem_node->pre_node->child_num-3]);
            printf("00000000%d\n",parent_mem_node->pre_node->node_num);

                parent_mem_node->pre_node->child_node_ptr[parent_mem_node->pre_node->child_num-2]->next_node=parent_mem_node->child_node_ptr[0];
            }
            if (!parent_mem_node->next_node)
                break;
            parent_mem_node=first_mem_node;
            
            ReadDiskNode(parent_disk_node,parent_disk_node->next_node_num,fp);
            parent_mem_node=parent_mem_node->next_node;
            printf("%p\n",parent_mem_node->next_node);
            // printf("node %d:\n",parent_mem_node->pre_node->node_num);
            // for(int i=0;i<parent_mem_node->pre_node->child_num;i++)
            // {
            //     printf("%u:%p\n",parent_mem_node->pre_node->keys[i],parent_mem_node->pre_node->child_node_ptr[i]);
            // }
        }
        
        parent_mem_node = first_mem_node->child_node_ptr[0];
        ReadDiskNode(parent_disk_node,first_disk_node->child_node_nums[0],fp);
        printf("************************\n");
    }
    first_leaf_node=first_mem_node->child_node_ptr[0];

    fclose(fp);
    free(work_node);
    free(parent_disk_node);
    // PrintAllLeafNode();
}
int PutAllTree(char path[])
{
    FILE *fp;
    Node *work_node = b_plus_tree.root_node;
    if ((fp = fopen(path, "w")) == NULL)
    {
        printf("open error\n");
        return -1;
    }
    PutTreeHead(path);
    // fseek(fp,16,SEEK_SET);
    NumberTheTree();
    int count = 0;
    while (!work_node->leaf)
    {
        Node *first_node = work_node;
        while (work_node)
        {
            DiskNode *disk_node = CreateDiskNode(work_node);
            for (int i = 0; i < work_node->child_num; i++)
            {
                disk_node->child_node_nums[i] = work_node->child_node_ptr[i]->node_num;
            }

            if (work_node->next_node)
                disk_node->next_node_num = work_node->next_node->node_num;
            else
                disk_node->next_node_num = -1;
            if(disk_node->node_num==3)
                printf("dddddddd%d\n",disk_node->child_num);
            fseek(fp, DATA_BEGIN + (disk_node->node_num) * sizeof(DiskNode), SEEK_SET);
            fwrite(disk_node, sizeof(DiskNode), 1, fp);

            free(disk_node);
            work_node = work_node->next_node;
        }
        work_node = first_node->child_node_ptr[0];
    }
    work_node = first_leaf_node;
    while (work_node)
    {
        DiskNode *disk_node = CreateDiskNode(work_node);
        fseek(fp, DATA_BEGIN + (disk_node->node_num) * sizeof(DiskNode), SEEK_SET);
        fwrite(disk_node, sizeof(DiskNode), 1, fp);

        if (work_node->next_node)
            disk_node->next_node_num = -1;
        free(disk_node);

        work_node = work_node->next_node;
    }
    fclose(fp);
    // PrintAllLeafNode();
}

int Delete(index_t key)
{
    Node *work_node = b_plus_tree.root_node;
    int delete_pos;
    while (!work_node->leaf)
    {
        delete_pos = SearchPos(work_node, key, true);
        if (delete_pos == LESS_THAN_MIN)
        {
            return -1;
        }
        work_node = work_node->child_node_ptr[delete_pos];
    }

    if (DeleteLeafNodeOneItem(work_node, key) == -1)
    {
        return -1;
    }
    return 0;
}
int DeleteLeafNodeOneItem(Node *work_node, index_t key)
{
    int delete_pos = SearchPos(work_node, key, true);
    if (delete_pos >= 0)
    {
        return -1;
    }
    else
    {
        delete_pos = -1 * (delete_pos + 1);
        index_t old_key = work_node->keys[0];
        for (int i = delete_pos; i < work_node->child_num; i++)
        {
            work_node->keys[i] = work_node->keys[i + 1];
            work_node->values[i] = work_node->values[i + 1];
        }
        work_node->child_num--;
        b_plus_tree.leaf_nums--;
        if (delete_pos == 0)
        {
            UpdateKey(work_node, old_key);
        }
        if (work_node->child_num < MIN_CACHE_NUM)
        {
            if (LendLeafNode(work_node) == 0)
            {
                return 0;
            }
            else
            {
                MergeLeafNode(work_node);
            }
        }
    }
    return 0;
}
int DeleteInternalNodeOneItem(Node *work_node, index_t key)
{
    int delete_pos = SearchPos(work_node, key, true);
    index_t old_key = work_node->keys[0];
    for (int i = delete_pos; i < work_node->child_num - 1; i++)
    {
        work_node->keys[i] = work_node->keys[i + 1];
        work_node->child_node_ptr[i] = work_node->child_node_ptr[i + 1];
    }
    work_node->child_num--;
    if (delete_pos == 0)
    {
        UpdateKey(work_node, old_key);
    }

    if (work_node == b_plus_tree.root_node)
    {
        return 0;
    }
    if (work_node->child_num < MIN_CACHE_NUM)
    {
        if (LendInternalNode(work_node) == 0)
        {
            return 0;
        }
        else
        {
            MergeInternalNode(work_node);
        }
    }

    return 0;
}
int LendInternalNode(Node *work_node)
{
    Node *lend_from;
    if (work_node->next_node && work_node->next_node->child_num > MIN_CACHE_NUM)
    {
        lend_from = work_node->next_node;
        work_node->keys[work_node->child_num] = lend_from->keys[0];
        work_node->child_node_ptr[work_node->child_num] = lend_from->child_node_ptr[0];
        work_node->child_node_ptr[work_node->child_num]->parent_ptr = work_node;
        for (int i = 0; i < lend_from->child_num - 1; i++)
        {
            lend_from->keys[i] = lend_from->keys[i + 1];
            lend_from->child_node_ptr[i] = lend_from->child_node_ptr[i + 1];
        }
        work_node->child_num++;
        lend_from->child_num--;
        UpdateKey(lend_from, work_node->keys[work_node->child_num - 1]);
    }
    else if (work_node->pre_node && work_node->pre_node->child_num > MIN_CACHE_NUM)
    {
        lend_from = work_node->pre_node;
        for (int i = work_node->child_num - 1; i >= 0; i--)
        {
            work_node->keys[i + 1] = work_node->keys[i];
            work_node->child_node_ptr[i + 1] = work_node->child_node_ptr[i];
        }
        work_node->keys[0] = lend_from->keys[lend_from->child_num - 1];
        work_node->child_node_ptr[0] = lend_from->child_node_ptr[lend_from->child_num - 1];
        work_node->child_node_ptr[0]->parent_ptr = work_node;

        lend_from->child_num--;
        work_node->child_num++;
        UpdateKey(work_node, work_node->keys[1]);
    }
    else
    {
        return -1;
    }
    return 0;
}
int LendLeafNode(Node *work_node)
{
    Node *lend_from;
    if (work_node->next_node && work_node->next_node->child_num > MIN_CACHE_NUM)
    {
        lend_from = work_node->next_node;
        work_node->keys[work_node->child_num] = lend_from->keys[0];
        work_node->values[work_node->child_num] = lend_from->values[0];
        for (int i = 0; i < lend_from->child_num - 1; i++)
        {
            lend_from->keys[i] = lend_from->keys[i + 1];
            lend_from->values[i] = lend_from->values[i + 1];
        }
        work_node->child_num++;
        lend_from->child_num--;
        UpdateKey(lend_from, work_node->keys[work_node->child_num - 1]);
    }
    else if (work_node->pre_node && work_node->pre_node->child_num > MIN_CACHE_NUM)
    {
        lend_from = work_node->pre_node;
        for (int i = work_node->child_num - 1; i >= 0; i--)
        {
            work_node->keys[i + 1] = work_node->keys[i];
            work_node->values[i + 1] = work_node->values[i];
        }
        work_node->keys[0] = lend_from->keys[lend_from->child_num - 1];
        work_node->values[0] = lend_from->values[lend_from->child_num - 1];
        lend_from->child_num--;
        work_node->child_num++;
        UpdateKey(work_node, work_node->keys[1]);
    }
    else
    {
        return -1;
    }
    return 0;
}
void MergeInternalNode(Node *work_node)
{
    Node *merge_from;
    if (work_node == b_plus_tree.root_node)
    {
        return;
    }
    if (work_node->next_node)
    {
        merge_from = work_node->next_node;
    }
    else if (work_node->pre_node)
    {
        merge_from = work_node;
        work_node = work_node->pre_node;
    }
    else
    {
        merge_from = work_node->parent_ptr;
        // MergeParentNode(work_node);
        work_node->next_node = merge_from->next_node;
        if (merge_from->next_node)
        {
            merge_from->next_node->pre_node = work_node;
        }
        work_node->parent_ptr = merge_from->parent_ptr;
        if (merge_from == b_plus_tree.root_node)
        {
            b_plus_tree.root_node = work_node;
            b_plus_tree.root_disk_pos = work_node->node_num;
        }
        free(merge_from);
        return;
    }
    for (int i = 0; i < merge_from->child_num; i++)
    {
        work_node->keys[work_node->child_num + i] = merge_from->keys[i];
        work_node->child_node_ptr[work_node->child_num + i] = merge_from->child_node_ptr[i];
        work_node->child_node_ptr[work_node->child_num + i]->parent_ptr = work_node;
    }
    work_node->next_node = merge_from->next_node;
    if (merge_from->next_node)
    {

        merge_from->next_node->pre_node = work_node;
    }
    work_node->child_num += merge_from->child_num;
    if (merge_from->parent_ptr)
    {
        DeleteInternalNodeOneItem(merge_from->parent_ptr, merge_from->keys[0]);
    }
    free(merge_from);

    return;
}
void MergeLeafNode(Node *work_node)
{
    Node *merge_from;
    if (work_node->next_node)
    {
        merge_from = work_node->next_node;
    }
    else if (work_node->pre_node)
    {
        merge_from = work_node;
        work_node = work_node->pre_node;
    }
    else
    {

        return;
    }
    for (int i = 0; i < merge_from->child_num; i++)
    {
        work_node->keys[work_node->child_num + i] = merge_from->keys[i];
        work_node->values[work_node->child_num + i] = merge_from->values[i];
    }
    work_node->next_node = merge_from->next_node;
    if (merge_from->next_node)
    {
        merge_from->next_node->pre_node = work_node;
    }
    work_node->child_num += merge_from->child_num;
    if (merge_from->parent_ptr)
    {
        DeleteInternalNodeOneItem(merge_from->parent_ptr, merge_from->keys[0]);
    }

    free(merge_from);

    return;
}
void SplitInternalNode(Node *work_node)
{
    Node *new_internal_node = AllocNode(false);
    Node *new_parent_node;
    index_t min_key_in_new_node;
    for (int i = 0; i < MIN_CACHE_NUM; i++)
    {
        new_internal_node->keys[i] = work_node->keys[work_node->child_num - MIN_CACHE_NUM + i];
        new_internal_node->child_node_ptr[i] = work_node->child_node_ptr[work_node->child_num - MIN_CACHE_NUM + i];
        new_internal_node->child_node_ptr[i]->parent_ptr = new_internal_node;
    }
    min_key_in_new_node = new_internal_node->keys[0];
    work_node->child_num = MIN_CACHE_NUM;
    new_internal_node->child_num = MIN_CACHE_NUM;
    if (work_node->next_node)
        work_node->next_node->pre_node = new_internal_node;
    new_internal_node->pre_node = work_node;
    new_internal_node->next_node = work_node->next_node;
    work_node->next_node = new_internal_node;

    if (work_node->parent_ptr == NULL)
    {
        new_parent_node = AllocNode(false);
        new_parent_node->child_node_ptr[0] = work_node;
        new_parent_node->keys[0] = work_node->keys[0];
        new_parent_node->child_num = 1;

        work_node->parent_ptr = new_parent_node;
        b_plus_tree.root_node = new_parent_node;
        b_plus_tree.root_disk_pos = new_parent_node->node_num;
    }
    InsertInternalNode(work_node->parent_ptr, min_key_in_new_node, new_internal_node);
    if (work_node->parent_ptr->child_num == 2 * MIN_CACHE_NUM)
    {
        SplitInternalNode(work_node->parent_ptr);
    }
}
void SplitLeafNode(Node *work_node)
{
    Node *new_leaf_node = AllocNode(true);
    Node *new_parent_node;
    index_t min_key_in_new_node;
    for (int i = 0; i < MIN_CACHE_NUM; i++)
    {
        new_leaf_node->keys[i] = work_node->keys[work_node->child_num - MIN_CACHE_NUM + i];
        new_leaf_node->values[i] = work_node->values[work_node->child_num - MIN_CACHE_NUM + i];
    }
    min_key_in_new_node = new_leaf_node->keys[0];
    work_node->child_num = MIN_CACHE_NUM;
    new_leaf_node->child_num = MIN_CACHE_NUM;

    if (work_node->next_node)
        work_node->next_node->pre_node = new_leaf_node;
    new_leaf_node->pre_node = work_node;
    new_leaf_node->next_node = work_node->next_node;
    work_node->next_node = new_leaf_node;
    if (work_node->parent_ptr == NULL)
    {
        new_parent_node = AllocNode(false);
        new_parent_node->child_node_ptr[0] = work_node;
        new_parent_node->keys[0] = work_node->keys[0];
        new_parent_node->child_num = 1;

        work_node->parent_ptr = new_parent_node;
        b_plus_tree.root_node = new_parent_node;
        b_plus_tree.root_disk_pos = new_parent_node->node_num;
        work_node->leaf = true;
    }
    InsertInternalNode(work_node->parent_ptr, min_key_in_new_node, new_leaf_node);
    if (work_node->parent_ptr->child_num == 2 * MIN_CACHE_NUM)
    {
        SplitInternalNode(work_node->parent_ptr);
    }
}
void Insert(struct b_plus_tree *b_plus_tree, index_t key, Value value)
{
    Node *work_node = b_plus_tree->root_node;
    while (!work_node->leaf)
    {
        int i = SearchPos(work_node, key, true);
        if (i == LESS_THAN_MIN)
        {
            i = 0;
        }
        work_node = work_node->child_node_ptr[i];
    }
    if (work_node->leaf)
    {
        InsertLeafNode(work_node, key, value);
        if (work_node->child_num == MIN_CACHE_NUM * 2)
        {
            SplitLeafNode(work_node);
        }
        b_plus_tree->leaf_nums++;
    }
    return;
}
int SearchPos(Node *work_node, index_t key, bool is_search)
{
    if (key > work_node->keys[work_node->child_num - 1])
    {
        if (is_search)
        {
            return work_node->child_num - 1;
        }
        else
        {
            return work_node->child_num;
        }
    }
    for (int i = work_node->child_num - 1; i >= 0; i--)
    {
        if (key < work_node->keys[i])
        {
            continue;
        }

        else if (key == work_node->keys[i] && work_node->leaf)
        {
            return -1 * (i + 1);
        }
        else
        {
            if (is_search)
            {
                return i;
            }
            else
            {
                return i + 1;
            }
        }
    }
    if (is_search)
    {
        return LESS_THAN_MIN;
    }
    else
    {
        return 0;
    }
}
void InsertLeafNode(Node *work_node, index_t key, Value value)
{
    int i;
    if (work_node->child_num == 0)
    {
        work_node->keys[0] = key;
        work_node->values[0] = value;
        work_node->child_num++;
        return;
    }
    int insert_pos = SearchPos(work_node, key, false);
    if (insert_pos < 0)
    {
        work_node->values[-1 * (insert_pos + 1)] = value;
        b_plus_tree.leaf_nums--;
        return;
    }
    for (int i = work_node->child_num - 1; i >= insert_pos; i--)
    {
        work_node->keys[i + 1] = work_node->keys[i];
        work_node->values[i + 1] = work_node->values[i];
    }
    work_node->keys[insert_pos] = key;
    work_node->values[insert_pos] = value;
    work_node->child_num++;
    if (insert_pos == 0 && work_node->parent_ptr)
    {
        UpdateKey(work_node, work_node->keys[1]);
    }
    return;
}
void InsertInternalNode(Node *work_node, index_t key, Node *new_node)
{
    int i;
    if (work_node->child_num == 0)
    {
        work_node->keys[0] = key;
        work_node->child_node_ptr[0] = new_node;
        work_node->child_num++;
        return;
    }
    int insert_pos = SearchPos(work_node, key, false);
    if (insert_pos == LESS_THAN_MIN)
    {
        insert_pos = 0;
    }
    for (int i = work_node->child_num - 1; i >= insert_pos; i--)
    {
        work_node->keys[i + 1] = work_node->keys[i];
        work_node->child_node_ptr[i + 1] = work_node->child_node_ptr[i];
    }
    work_node->keys[insert_pos] = key;
    work_node->child_node_ptr[insert_pos] = new_node;

    new_node->parent_ptr = work_node;

    work_node->child_num++;
    if (insert_pos == 0 && work_node->parent_ptr)
    {
        UpdateKey(work_node, work_node->keys[1]);
    }
}
Value *Search(index_t key)
{
    Node *work_node = b_plus_tree.root_node;
    while (!work_node->leaf)
    {
        int i = SearchPos(work_node, key, true);
        if (i == LESS_THAN_MIN)
        {
            return NULL;
        }
        if (key >= work_node->keys[i])
        {
            work_node = work_node->child_node_ptr[i];
        }
    }
    int i = SearchPos(work_node, key, true);
    if (i == LESS_THAN_MIN)
    {
        return NULL;
    }
    if (i < 0)
    {
        return &(work_node->values[-1 * (i + 1)]);
    }
    else
    {
        return NULL;
    }
}
void UpdateKey(Node *work_node, index_t old_key)
{
    if (!work_node->parent_ptr)
    {
        return;
    }
    for (int i = 0; i < work_node->parent_ptr->child_num; i++)
    {
        if (work_node->parent_ptr->keys[i] == old_key)
        {
            work_node->parent_ptr->keys[i] = work_node->keys[0];
            if (i == 0)
            {
                UpdateKey(work_node->parent_ptr, old_key);
            }
            return;
        }
    }
}
struct b_plus_tree BPTreeCreate()
{
    b_plus_tree.node_nums = 0;
    b_plus_tree.root_node = AllocNode(true);

    b_plus_tree.root_node->child_num = 0;
    first_leaf_node = b_plus_tree.root_node;
    return b_plus_tree;
}
Node *AllocNode(bool isLeaf)
{
    Node *new_node = malloc(sizeof(Node));
    new_node->leaf = isLeaf;
    new_node->node_num = b_plus_tree.node_nums;
    b_plus_tree.node_nums++;
    return new_node;
}