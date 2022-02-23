#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        printf("Дочерний процесс. Его pid: %d. Родительский pid: %d.\n", getpid(), getppid());
        exit(0);
    }
    else
    {
        printf("Родительский процесс. Его pid: %d. Родительский pid: %d.\n", getpid(), getppid());
        wait(&status);
    }
    return 0;
}