#include <stdio.h>
int main()
{
    int *a[5];
    int (*b)[5]={0};
    int *p=*b;
    printf("%p %p",p,p+1);

    return 0;
}