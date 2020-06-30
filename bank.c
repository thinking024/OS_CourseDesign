#include <stdio.h>
#include <windows.h>

int resource[4] = {15,6,9,10};
int require[6][4]  = { {9,5,5,5}, {2,2,3,3}, {7,5,4,4}, {3,3,3,2}, {5,2,2,1}, {4,4,4,4} };
int allocate[6][4] = { {2,0,2,1}, {0,1,1,1}, {4,1,0,2}, {1,0,0,1}, {1,1,0,0}, {1,0,1,1} };
int need[6][4]     = { {7,5,3,4}, {2,1,2,2}, {3,4,4,2}, {2,3,3,1}, {4,1,2,1}, {3,4,3,3} };
int available[4] = {6,3,5,4};

void valuation1(int array[4],int value[4])
{
    int i;
    for (i = 0;i < 4;i ++)
    {
        array[i] = value[i];
    }
}

void valuation2(int array[6][4],int value[6][4])
{
    int i,j;
    for (i = 0;i < 6;i ++)
    {
        for (j = 0;j < 4;j ++)
        {
            array[i][j] = value[i][j];
        }
    }
}

void printInfo()
{
    int i,j;
    printf("\nallocate\n");
    for (i = 0;i < 6;i ++)
    {
        for (j = 0;j < 4;j ++)
        {
            printf("%d ",allocate[i][j]);
        }
        printf("\n");
    }

    printf("\nneed\n");
    for (i = 0;i < 6;i ++)
    {
        for (j = 0;j < 4;j ++)
        {
            printf("%d ",need[i][j]);
        }
        printf("\n");
    }

    printf("\navailable:");
    for (i = 0;i < 4;i ++)
    {
        printf(" %d",available[i]);
    }
    printf("\n");
}

int safe(int threadName,int require0,int require1,int require2,int require3)
{
    int i;
    // 合理性验证
    if (need[threadName][0] < require0 || need[threadName][1] < require1 ||
        need[threadName][2] < require2 || need[threadName][3] < require3)
    {
        return -1;
    }

    // 可能性验证
    if (available[0] < require0 || available[1] < require1 ||
        available[2] < require2 || available[3] < require3)
    {
        return -2;
    }

    // 安全性验证
    int try_allocate[6][4];
    int try_need[6][4];
    int try_available[4];
    valuation1(try_available,available);
    valuation2(try_allocate,allocate);
    valuation2(try_need,need);

    try_allocate[threadName][0] = try_allocate[threadName][0] + require0;
    try_allocate[threadName][1] = try_allocate[threadName][1] + require1;
    try_allocate[threadName][2] = try_allocate[threadName][2] + require2;
    try_allocate[threadName][3] = try_allocate[threadName][3] + require3;

    try_need[threadName][0] = try_need[threadName][0] - require0;
    try_need[threadName][1] = try_need[threadName][1] - require1;
    try_need[threadName][2] = try_need[threadName][2] - require2;
    try_need[threadName][3] = try_need[threadName][3] - require3;

    try_available[0] = try_available[0] - require0;
    try_available[1] = try_available[1] - require1;
    try_available[2] = try_available[2] - require2;
    try_available[3] = try_available[3] - require3;


    for (i = 0;i < 6;i ++)
    {
        if (try_need[i][0] <= try_available[0] && try_need[i][1] <= try_available[1] &&
            try_need[i][2] <= try_available[2] && try_need[i][3] <= try_available[3])
        {
            valuation1(available,try_available);
            valuation2(allocate,try_allocate);
            valuation2(need,try_need);
            return 1;
        }
    }
    return 0;
}

int main()
{
    int i,j,k;

    printf("resource:");
    for (i = 0; i < 4 ;i ++)
    {
        printf(" %d",resource[i]);
    }
    printf("\n");

    printf("\nrequire\n");
    for (i = 0;i < 6;i ++)
    {
        for (j = 0;j < 4;j ++)
        {
            printf("%d ",require[i][j]);
        }
        printf("\n");
    }
    
    printInfo();

    while(1)
    {
        printf("\nstart\n");
        int threadName,require0,require1,require2,require3;
        scanf("%d",&threadName);
        if (threadName < 0)
        {
            break;
        }
        scanf("%d %d %d %d",&require0,&require1,&require2,&require3);

        int status = safe(threadName,require0,require1,require2,require3);
        if (status == 1)
        {
            printf("success\n");
            printInfo();
        }

        if (status == -1)
        {
            printf("more than need\n");
        }

        if (status == -2)
        {
            printf("more than available\n");
        }

        if (status == 0)
        {
            printf("may cause deadlock\n");
        }

    }

    return 0;
}
