#include "calculator.h"
#include <stdio.h>
int division(int a, int b)
{
    if (b == 0)
    {
        printf("На ноль делить нельзя");
    }
    
    return a / b;
}