#include <stdio.h>
#include "calculator.h"
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <dirent.h>
#include <unistd.h>

int MAX_COUNT_PLUGINS = 10;
char **plugins;
int read_plugins_dir(/*char **plugins,*/ char* path)
{
    struct dirent *entry;
    int i = 0;
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        return -1;
    }
    while ((entry = readdir(dir)))
    {
        if (strcmp(".", entry->d_name) != 0 && strcmp("..", entry->d_name) != 0)
        {
            strcpy(plugins[i], entry->d_name);
            i++;
        }
        if (i >= MAX_COUNT_PLUGINS)
        {
            plugins = realloc(plugins, i*2);
            for (int j = i; j < i*2; j++)
            {
                plugins[j] = malloc(sizeof(char) * 256);
            }
            MAX_COUNT_PLUGINS = i*2;
        }
    }
    closedir(dir);
    return i;
}

int main(int argc, char *argv[])
{
    char* path = malloc(sizeof(char) * 256);
    //char **plugins;
    int (*arithmetic_operation)(int, int);
    void *handle;
    int operation, num1, num2;
    if (argc == 2)
    {
        strcpy(path ,argv[1]);
    }
    else
    {
        strcpy(path, "./lib/");
    }    
    plugins = (char**)malloc(sizeof(char*) * MAX_COUNT_PLUGINS);
    for (int i = 0; i < MAX_COUNT_PLUGINS; i++)
    {
        plugins[i] = malloc(sizeof(char) * 256);
    }
    int count = read_plugins_dir(path);
    printf("%d\n", count);
    printf("Выберите операцию:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d - %s\n", i, plugins[i]);
    }
    printf("Введите номер операции: ");
    scanf("%d", &operation);
    printf("Введите первое число: ");
    scanf("%d", &num1);
    printf("Введите второе число: ");
    scanf("%d", &num2);
    char *string_path = malloc(sizeof(char) * 256);
    string_path = strcpy(string_path, path);
    string_path = strcat(string_path, plugins[operation]);
    handle = dlopen(string_path, RTLD_LAZY);
    if (!handle)
    {
        fputs(dlerror(), stderr);
        exit(1);
    }
    char *string_name_func = malloc(sizeof(char) * 256);
    string_name_func = strcpy(string_name_func, plugins[operation]);
    string_name_func = string_name_func + 3;
    string_name_func[strlen(string_name_func) - 3] = '\0';
    arithmetic_operation = dlsym(handle, string_name_func);
    int rezult = arithmetic_operation(num1, num2);
    printf("результат: %d\n", rezult);
    free(string_path);
    free(string_name_func - 3);
    free(path);
    for (int i = 0; i < MAX_COUNT_PLUGINS; i++)
    {
        free(plugins[i]);
    }
    free(plugins);
    return 0;
}