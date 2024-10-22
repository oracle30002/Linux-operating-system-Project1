#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


//void * get_my_physical_addr(void *);
void *get_my_physical_addr(void *vaddr)
{
    return (void *)syscall(449, vaddr);
}


int a[2000000];


int main()
{
    int      loc_a;
    void     *phy_add;


    phy_add=get_my_physical_addr(&a[0]);
    printf("global element a[0]:\n");
    printf("Offest of logical address:[%p]   Physical address:[%p]\n", &a[0], phy_add);
    printf("========================================================================\n");
    phy_add=get_my_physical_addr(&a[1999999]);
    printf("global element a[1999999]:\n");
    printf("Offest of logical address:[%p]   Physical address:[%p]\n", &a[1999999], phy_add);
    printf("========================================================================\n");


}
