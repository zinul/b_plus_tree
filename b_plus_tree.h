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
    index_t keys[MAX_CACHE_NUM+1];

    struct Node *pre_node;
    struct Node *next_node;
    // leaf node
    Value values[MAX_CACHE_NUM+1];
    // off_t disk_pos;

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
    int root_disk_pos;
    int leaf_nums;
    int node_nums;
};
extern struct b_plus_tree BPTreeCreate();
void SplitSplitLeafNode(Node *work_node);
void SplitInternalNode(Node *work_node);

void Insert(struct b_plus_tree *b_plus_tree, index_t key, Value value);
void InsertLeafNode(Node *work_node,index_t key,Value value);
void InsertInternalNode(Node *work_node,index_t key,Node *new_node);

Value *Search(index_t key);
int SearchPos(Node *work_node, index_t key,bool is_search);

int DeleteInternalNodeOneItem(Node *work_node,index_t key);
int DeleteLeafNodeOneItem(Node *work_node, index_t key);
int DeleteLeafNode(Node *work_node,index_t key);
int Delete(index_t key);
int LendLeafNode(Node *work_node);
int LendInternalNode(Node *work_node);
void MergeLeafNode(Node *work_node);
void MergeInternalNode(Node *work_node);
void MergeParentNode(Node *work_node);
void UpdateKey(Node *work_node,index_t old_key);
Node *AllocNode(bool isLeaf);

int PutTreeHead(char path[],struct b_plus_tree b_plus_tree);
int PutAllTree(char path[],struct b_plus_tree b_plus_tree);
int Get(char path[],struct b_plus_tree b_plus_tree);
// extern Node *InsertNode(index_t key,Value value);