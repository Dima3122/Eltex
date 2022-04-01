#include "foo.h"

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in client_addr, server_addr;
    struct hostent *hp;
    socklen_t length;
    char *buf = malloc(sizeof(char) * BUF_SIZE);
    int bytes_read;

    if (argc < 4)
    {
        printf("ENTER ./bin/client hostname port message\n");
        exit(1);
    }

    sock = Socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    bcopy(hp->h_addr, &server_addr.sin_addr, hp->h_length);
    server_addr.sin_port = htons(atoi(argv[2]));

    memset(&client_addr, 0, sizeof(client_addr));

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = 0;
    Bind(sock, (struct sockaddr *)&client_addr, sizeof(client_addr));

    printf("Is ready for sending\n");

    memset(buf, 0, sizeof(char) * BUF_SIZE);
    strcpy(buf, argv[3]);
    printf("Message to send: \"%s\"\n", buf);
    if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("sendto");
        exit(3);
    }

    printf("CLIENT: Sending complete\n");

    if ((bytes_read = recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &length)) < 0)
    {
        perror("recvfrom");
        exit(4);
    }

    printf("CLIENT: Received back message: \"%s\"\n", buf);
    int n1 = atoi(argv[3]);
    for (int i = 0; i < n1; i++)
    {
        if (sendto(sock, &n1, sizeof(int), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("sendto");
            exit(3);
        }
        printf("CLIENT: Sent %d from %d\n", i + 1, n1);
        sleep(n1);
    }
    printf("CLIENT: Sending complete\n");
    close(sock);
    return 0;
}