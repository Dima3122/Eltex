#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX 100000000

int main()
{
    int tmp = 0, count = 0;
    for (int i = 0; i < MAX; i++)
    {
        tmp = count;
        tmp++;
        count = tmp;
    }
    printf("count = %d", count);
    return 0;
}