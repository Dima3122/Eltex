#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main(int argc, char **argv)
{
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    int must_stop = 0;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == 0)
    {
        perror("Creating queue error");
        return -1;
    }
    do
    {
        ssize_t bytes_read;
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if(!(bytes_read >= 0))
        {
            perror("mq_receive error");
            exit(-1);
        }

        buffer[bytes_read] = '\0';
        if (!strncmp(buffer, MSG_STOP, strlen(MSG_STOP)))
        {
            must_stop = 1;
        }
        else
        {
            printf("Received: %s\n", buffer);
        }
    } while (!must_stop);

    /* cleanup */
    
    if(!((mqd_t)-1 != mq_close(mq)))
    {
        perror("mq_close_error");
        exit(-1);
    }
    if(!((mqd_t)-1 != mq_unlink(QUEUE_NAME)))
    {
        perror("mq_unlink error");
        exit(-1);
    }

    return 0;
}