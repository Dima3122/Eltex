#include <stdio.h>
#include "calculator.h"

int main()
{
    int a = 0, b = 0, operation = 0;
    printf("Введите a: ");
    scanf("%d", &a);
    printf("Введите b: ");
    scanf("%d", &b);    
    printf("\nВведите 1 если хотите сложить два числа\nВведите 2 если хотите вычесть два числа\nВведите 3 если хотите умножить два числа два числа\nВведите 4 если хотите разделить два числа\n");
    scanf("%d", &operation);
    switch (operation)
    {
    case 1:
        printf("Результат: %d\n", sum(a,b));
        break;
    case 2:
        printf("Результат: %d\n", subtraction(a,b));
        break;
    case 3:
        printf("Результат: %d\n", multiplication(a,b));
        break;
    case 4:
        printf("Результат: %d\n", division(a,b));
        break;
    default:
        printf("Операция выбранна неверно");
        break;
    }
    return 0;
}