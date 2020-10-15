#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_CACHE_NUM 5
#define MIN_CACHE_NUM 3
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
    // leaf==true means leaf node,otherwise it is internal node
    bool leaf;
    int child_num; 
    int node_num;
    // I need to alloc a MIN_CACHE_NUM*sizeof(Key) bytes memory when a node is created
    // then when a new child join in, I will alloc the MAX_CACHE_NUM*sizeof(Key) bytes mem
    // Key *keys;
    index_t keys[MAX_CACHE_NUM];

    // leaf node
    struct Node *pre_node;
    struct Node *next_node;
    Value values[MAX_CACHE_NUM];
    off_t disk_pos;

    // internal node
    struct Node *child_node_ptr[MAX_CACHE_NUM];
       
} Node;
struct b_plus_tree
{
    Node *root_node;
    long long leaf_nums;
    long long node_nums;
};
extern struct b_plus_tree BPTreeCreate();
extern void SplitSplitLeafNode(Node *work_node);
extern void SplitInternalNode(Node *work_node);
extern void Insert(struct b_plus_tree *b_plus_tree, index_t key, Value value);
extern int SearchInsertPos(Node *work_node, index_t key,bool is_search);
extern void InsertLeafNode(Node *work_node,index_t key,Value value);
extern void InsertInternalNode(Node *work_node,index_t key,Node *new_node);
Value *Search(index_t key);

void UpdateKey(Node *work_node,index_t old_key);
extern Node *AllocNode(bool isLeaf);
// extern Node *InsertNode(index_t key,Value value);