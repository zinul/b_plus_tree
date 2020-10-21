#include "b_plus_tree.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define AMOUNT_OF_DATA 10000000
struct b_plus_tree;
Node *first_leaf_node;

void PrintAllLeafNode();
void d_PrintAllLeafNode();
void PrintInternalNode(int levels);
void PrintTheTree();
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
    // b_plus_tree = BPTreeCreate();
    // b_plus_tree.leaf_nums = 0;
    // for (unsigned int i = 0; i < AMOUNT_OF_DATA; ++i)
    // {
    //     fscanf(fp, "%s%s%u", ip_buf, domain_buf, &ip);
    //     Value value;
    //     strcpy(value.domain, domain_buf);
    //     Insert(&b_plus_tree, ip, value);
    //     // printf("%s\t%u\n", domain_buf, ip);
    //     // printf("nums:%lld\n", b_plus_tree.leaf_nums);
    // // PrintAllLeafNode();
    // }
    // PutAllTree("a.bpt");
    // PrintAllLeafNode();
    // sleep(1);
    // for(int i=0;i<10;i++)
    // printf("\n\n\n");

    GetAllTree("a.bpt");
    // PrintTheTree();
    // PrintAllLeafNode();
    // PrintInternalNode(1);
    // printf("%d\n",sizeof(DiskNode));
    // printf("%lld %lld\n",b_plus_tree.node_nums,b_plus_tree.leaf_nums);
    // printf("%d",sizeof(Node));
    // pause();
    // PrintInternalNode(1);
    // printf("\n");
    // PrintInternalNode(2);
    // printf("\n");

    // PrintInternalNode(3);
    // printf("%p",b_plus_tree.root_node);
    // printf("%lld\n",b_plus_tree.root_node->node_num);
    // PrintInternalNode(6);
    // fseek(fp, 0, SEEK_SET);
    // int count = 0;
    // for (unsigned int i = 0; i < AMOUNT_OF_DATA; ++i)
    // {
    //     fscanf(fp, "%s%s%u", ip_buf, domain_buf, &ip);
    //     value = Search(ip);
    //     if (!value)
    //     {
    //         // printf("aaaaaaa\n");
    //         // printf("no value for%u\n", ip);
    //         // return 0;
    //         continue;
    //     }
    //     count++;
    //     // printf("%u:%s\n",ip,value->domain);
    // }
    // // printf("%d", count);

    // printf("insert finished%lld\n", b_plus_tree.leaf_nums);
    // fseek(fp,0,SEEK_SET);
    // for (unsigned int i = 0; i < AMOUNT_OF_DATA; i++)
    // {
    //     fscanf(fp, "%s%s%u", ip_buf, domain_buf, &ip);
    //     // printf("********************************%d\n",i);
    //     if(Delete(ip)==-1)
    //     {
    //         // printf("there is no domain for ip:%u\n",ip);
    //     }
    //     else
    //     {
    //         // printf("delete ip:%u\n",ip);
    //     }
    
    // // PrintInternalNode(2);
    // }
    // PrintAllLeafNode();
    // PrintInternalNode(1);
    // printf("\n");
    return 0;
}
void PrintTheTree()
{
    Node *work_node=b_plus_tree.root_node;
    Node *queue[b_plus_tree.node_nums];
    int head=0;
    int tail=0;
    while (!work_node->leaf)
    {
        // for(int i=0;i<work_node->child_num;i++)
        // {
        //     queue[tail++]=work_node->child_node_ptr[i];
        //     printf("%u ",work_node->keys[i]);
        // }
        // work_node=queue[head++];
        // if(!work_node->next_node)
        //     printf("\n");
        Node *first_node=work_node;
        while (work_node)
        {
            printf("%u ",work_node->node_num);
            work_node=work_node->next_node;
        }
        printf("\n");
        work_node=first_node->child_node_ptr[0];
    }
    work_node=first_leaf_node;
    while (work_node)
    {
        printf("%d ",work_node->node_num);
        work_node=work_node->next_node;
    }
    
}

void PrintAllLeafNode()
{
    Node *work_node = first_leaf_node;
    Node *last_node;
    int i = 0;
    do
    {
        printf("node%u:\n", work_node->node_num);
        for (int i = 0; i < work_node->child_num; ++i)
        {
            printf("%u:%s\n", work_node->keys[i], work_node->values[i].domain);
        }
        // sleep(1);
        if(work_node->next_node&&work_node->parent_ptr!=work_node->next_node->parent_ptr)printf("\n");
        if(!work_node->next_node)
            last_node=work_node;        
        work_node = work_node->next_node;

    } while (work_node != NULL);
    // work_node=last_node;
    // printf("\n");
    // do
    // {
    //     printf("node%u:\n", work_node->node_num);
        
    //     work_node = work_node->pre_node;
    // } while (work_node != NULL);
}
void d_PrintAllLeafNode()
{
    Node *work_node = first_leaf_node;
    DiskNode *disk_node=malloc(sizeof(DiskNode));
    Node *last_node;
    int i = 0;
    FILE *fp=fopen("a.bpt","r");
    do
    {
        
        ReadDiskNode(disk_node,work_node->node_num,fp);
        printf("node%u:\n", disk_node->node_num);
        for (int i = 0; i < disk_node->child_num; ++i)
        {
            printf("%u:%s\n", disk_node->keys[i], disk_node->values[i].domain);
        }
        // sleep(1);
        if(work_node->next_node&&work_node->parent_ptr!=work_node->next_node->parent_ptr)printf("\n");
    
        work_node = work_node->next_node;

    } while (work_node != NULL);
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
            printf("node%d:\n", work_node->node_num);
            for (int i = 0; i < work_node->child_num; ++i)
            {
                printf("%u:%p\n", work_node->keys[i], work_node->child_node_ptr[i]);
            }
            // sleep(1);
            work_node = work_node->next_node;
        } while (work_node != NULL);
    }
}
