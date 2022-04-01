#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "foo.h"

#define MAX_QUEUE 10
#define BUF_SIZE 256

#define PTHREAD_CREATE_ERROR -2
#define PTHREAD_DETACH_ERROR -3
#define PTHREAD_MUTEX_LOCK_ERROR -4
#define FILE_OPEN_ERROR -5
#define FILE_WRITE_ERROR -6

typedef struct
{
    int sock_client;
    struct sockaddr_in client_addr;
    int fd;
} PthreadData;

pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEXFILE = PTHREAD_MUTEX_INITIALIZER;

void *process(void *inputData)
{
    int n,fd,status, sock_client;
    char *buf = malloc(BUF_SIZE * sizeof(char));;
    struct sockaddr_in client_addr;

    PthreadData *data = (PthreadData *)inputData;
    sock_client = data->sock_client;
    client_addr = data->client_addr;
    fd = data->fd;
    pthread_mutex_unlock(&MUTEX);

    while (1)
    {
        if ((recv(sock_client, &n, sizeof(int), 0)) <= 0)
        {
            break;
        }
        printf("Client address: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Received n = %d\n", n);
        pthread_mutex_lock(&MUTEXFILE);

        status = sprintf(buf, "%s %s%c%d\n%s %d\n", "Client address:", inet_ntoa(client_addr.sin_addr), ':', ntohs(client_addr.sin_port), "Received n = ", n);
        if (status < 0)
        {
            perror("sprintf");
            exit(FILE_WRITE_ERROR);
        }
        if (write(fd, buf, strlen(buf)) == -1)
        {
            perror("write");
            exit(FILE_WRITE_ERROR);
        }
        pthread_mutex_unlock(&MUTEXFILE);
    }
    close(sock_client);
    pthread_exit(NULL);
}

int main()
{
    int sock_client, sock_server, status,fd;
    socklen_t length;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread;
    PthreadData data;

    fd = open("data.txt", O_CREAT | O_EXCL | O_WRONLY, S_IRWXU);
    if (fd == -1)
    {
        fd = open("data.txt", O_WRONLY | O_TRUNC, S_IRWXU);
        if (fd == -1)
        {
            perror("open");
            exit(FILE_OPEN_ERROR);
        }
    }

    sock_server = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = 0;

    Bind(sock_server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    length = sizeof(server_addr);
    Getsockname(sock_server, (struct sockaddr *)&server_addr, &length);
    printf("port number - %d\n", ntohs(server_addr.sin_port));
    Listen(sock_server, MAX_QUEUE);

    while (1)
    {
        sock_client = Accept(sock_server, (struct sockaddr *)&client_addr, &length);
        pthread_mutex_lock(&MUTEX);
        
        data.sock_client = sock_client;
        data.client_addr = client_addr;
        data.fd = fd;

        status = pthread_create(&thread, NULL, process, &data);
        if (status != 0)
        {
            perror("pthread_create");
            exit(PTHREAD_CREATE_ERROR);
        }
        status = pthread_detach(thread);
        if (status != 0)
        {
            perror("pthread_detach");
            exit(PTHREAD_DETACH_ERROR);
        }
    }
    close(fd);
    close(sock_server);
    return 0;
}