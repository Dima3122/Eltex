#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main(int argc, char **argv)
{
    mqd_t mq;
    char buffer[MAX_SIZE];

    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == 0)
    {
        perror("Creating queue error");
        return -1;
    }

    printf("Send to server (enter \"exit\" to stop it):\n");

    do {
        printf("> ");
        fflush(stdout);

        memset(buffer, 0, MAX_SIZE);
        fgets(buffer, MAX_SIZE, stdin);

        if(!(0 <= mq_send(mq, buffer, MAX_SIZE, 0)))
        {
            perror("mq_send");
            exit(-1);
        }

    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));

    if(!((mqd_t)-1 != mq_close(mq)))
    {
        perror("mq_close error");
        exit(-1);
    }
    return 0;
}