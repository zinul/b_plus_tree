#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define DATA_BEGIN 8
#define MAX_CACHE_NUM 19
#define MIN_CACHE_NUM 10
#define LESS_THAN_MIN 100
struct b_plus_tree b_plus_tree;

typedef unsigned int index_t;
typedef struct Value
{
    char domain[16];
} Value;

typedef struct Node
{
    struct Node *parent_ptr;
    bool leaf;
    int child_num; 
    int node_num;

    index_t keys[MAX_CACHE_NUM+1];

    struct Node *pre_node;
    struct Node *next_node;

    // leaf node
    Value values[MAX_CACHE_NUM+1];

    // internal node
    struct Node *child_node_ptr[MAX_CACHE_NUM+1];
       
} Node;
typedef struct DiskNode
{
    bool leaf;
    int node_num;
    int next_node_num;
    index_t keys[MAX_CACHE_NUM];
    // 最后一个不复制
    // leaf node
    Value values[MAX_CACHE_NUM];
    // internal node
    int child_num;
    int child_node_nums[MAX_CACHE_NUM];
}DiskNode;
struct b_plus_tree
{
    Node *root_node;
    // int root_disk_pos;
    int leaf_nums;
    int node_nums;
};
extern struct b_plus_tree BPTreeCreate();

void Insert(struct b_plus_tree *b_plus_tree, index_t key, Value value);
int Delete(index_t key);
Value *Search(index_t key);
int GetAllTree(char path[] );
int PutAllTree(char path[]);
// extern Node *InsertNode(index_t key,Value value);