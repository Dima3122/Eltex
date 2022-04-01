#include <arpa/inet.h>
#include <memory.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>

#define MAX_QUEUE 10
#define BUF_SIZE 256
#define SOCKET_BIND_ERROR -2
#define SOCKET_GETSOCKNAME_ERROR -3
#define SOCKET_ACCEPT_ERROR -4
#define SOCKET_LISTEN_ERROR -5
#define SOCKET_RECV_ERROR -6
#define SELECT_ERROR -10

void Inet_pton(int af, const char *src, void *dst);
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void Listen(int sockfd, int backlog);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Socket(int domain, int type, int protocol);
void Getsockname(int sockfd, struct sockaddr *addr, socklen_t *len);

void Select(int __nfds, fd_set * __readfds, fd_set * __writefds, fd_set *__restrict__ __exceptfds, struct timeval *__restrict__ __timeout);