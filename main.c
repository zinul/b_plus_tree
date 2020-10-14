#include "b_plus_tree.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
struct b_plus_tree;
int main()
{
    FILE *fp;
    char domain_buf[16];
    char ip_buf[16];
    int ip;
    if ((fp = fopen("ip_log", "r")) == -1)
    {
        printf("open error\n");
        return -1;
    }    
    b_plus_tree = BPTreeCreate();
    for(int i=0;i<10;++i)
    {
        fscanf(fp,"%s\t%s\t%d",ip_buf,domain_buf,ip);
        Value value;
        strcpy(value.domain,domain_buf);
        Insert(b_plus_tree.root_node,ip,value);
    }
    
    return 0;
}