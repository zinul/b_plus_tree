#include <sys/types.h>
#include <stdbool.h>

#define MAX_CACHE_NUM 5
#define MIN_CACHE_NUM 3

struct b_plus_tree b_plus_tree;

typedef unsigned int index_t;
typedef struct Value
{
    char domain[16];
} Value;

typedef struct Node
{
    Node *parent_ptr;
    // leaf==true means leaf node,otherwise it is internal node
    bool leaf;
    int child_num; 
    // I need to alloc a MIN_CACHE_NUM*sizeof(Key) bytes memory when a node is created
    // then when a new child join in, I will alloc the MAX_CACHE_NUM*sizeof(Key) bytes mem
    // Key *keys;
    index_t keys[MAX_CACHE_NUM];

    // leaf node
    Node *pre_node;
    Node *next_node;
    Value values[MAX_CACHE_NUM];
    off_t disk_pos;

    // internal node
    Node *child_node_ptr[MAX_CACHE_NUM];
       
} Node;
struct b_plus_tree
{
    Node *root_node;
    long long leaf_nums;
};
extern Node *SplitSplitLeafNode(Node *work_node);
extern Node *SplitInternalNode(Node *work_node);
void InsertLeafNode(Node *work_node,index_t key,Value value);
void InsertInternalNode(Node *work_node,index_t key,Node *new_node);
int SearchInsertPos(Node *work_node, index_t key);

// extern Node *InsertNode(index_t key,Value value);