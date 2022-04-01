#include "foo.h"

int main()
{
    int sock, n ,bytes_read;;
    socklen_t length;
    struct sockaddr_in server_addr, client_addr;
    char *buf = malloc(sizeof(char) * 256);
    
    sock = Socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = 0;

    Bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    length = sizeof(server_addr);
    Getsockname(sock, (struct sockaddr *)&server_addr, &length);
    printf("port number - %d\n", ntohs(server_addr.sin_port));
    
    while (1)
    {
        memset(buf, 0, sizeof(char) * 255);
        if ((bytes_read = recvfrom(sock, buf, 255, 0, (struct sockaddr *)&client_addr, &length)) < 0)
        {
            perror("recvfrom");
            exit(4);
        }
        printf("Client address: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Received message: \"%s\"\n", buf);
        buf = strcat(buf, "World");
        if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
        {
            perror("sendto");
            exit(5);
        }
        break;
    }
    while (1)
    {
        if (recvfrom(sock, &n, sizeof(int), 0, (struct sockaddr *)&client_addr, &length) < 0)
        {
            perror("recvfrom");
            exit(4);
        }
        printf("Client address: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Received n = %d\n", n);
    }
    close(sock);
    return 0;
}