#include "b_plus_tree.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#define AMOUNT_OF_DATA 4000000
struct b_plus_tree;
Node *first_leaf_node;

void PrintAllLeafNode();
void PrintInternalNode(int levels);

int main()
{
    FILE *fp;
    char domain_buf[16];
    char ip_buf[16];
    Value *value;
    unsigned int ip;
    if ((fp = fopen("ip_log", "r")) == NULL)
    {
        printf("open error\n");
        return -1;
    }
    b_plus_tree = BPTreeCreate();
    b_plus_tree.leaf_nums = 0;
    for (unsigned int i = 0; i < AMOUNT_OF_DATA; ++i)
    {
        fscanf(fp, "%s%s%u", ip_buf, domain_buf, &ip);
        Value value;
        strcpy(value.domain, domain_buf);
        Insert(&b_plus_tree, ip, value);
        // printf("%s\t%u\n", domain_buf, ip);
        // printf("nums:%lld\n", b_plus_tree.leaf_nums);
    // PrintAllLeafNode();
    }

    return 0;
}
void PrintAllLeafNode()
{
    Node *work_node = first_leaf_node;
    int i = 0;
    do
    {
        printf("node%u:\n", work_node->node_num);
        for (int i = 0; i < work_node->child_num; ++i)
        {
            printf("%u:%s\n", work_node->keys[i], work_node->values[i].domain);
        }
        // sleep(1);
        
        work_node = work_node->next_node;
    } while (work_node != NULL);
    // printf("\n");
    // do
    // {
    //     printf("node%u:\n", work_node->node_num);
        
    //     work_node = work_node->pre_node;
    // } while (work_node != NULL);
}
void PrintInternalNode(int levels)
{
    Node *work_node = first_leaf_node;
    int i = 0;
    for (i = 0; i < levels; ++i)
    {
        work_node = work_node->parent_ptr;
    }
    if (work_node)
    {
        do
        {
            // printf("node%d:\n", i++);
            for (int i = 0; i < work_node->child_num; ++i)
            {
                printf("%u:%p\n", work_node->keys[i], work_node->child_node_ptr[i]);
            }
            // sleep(1);
            work_node = work_node->next_node;
        } while (work_node != NULL);
    }
}
