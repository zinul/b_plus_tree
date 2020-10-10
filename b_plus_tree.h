#include <sys/types.h>

#define MAX_CACHE_NUM 5
#define MIN_CACHE_NUM MAX_CACHE_NUM/2 

typedef int index_t;
typedef struct Value
{
    char domain[16];
} Value;

typedef struct Key
{
    Node *l_node;
    Node *r_node;
    index_t index;
} Key;
typedef struct Node
{
    Node *parent_ptr;
    // leaf==true means leaf node,otherwise it is internal node
    bool leaf;
    // I need to alloc a MIN_CACHE_NUM*sizeof(Key) bytes memory when a node is created
    // then when a new child join in, I will alloc the MAX_CACHE_NUM*sizeof(Key) bytes mem
    Key *keys;
    int key_num;

    // leaf node
    Node *pre_node;
    Node *next_node;
    Value values;
    off_t disk_pos;

    // internal node
    Node *child_node_ptr[MIN_CACHE_NUM];
} Node;

Node *InsertNode(Key key,Value value)