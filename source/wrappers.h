#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <arpa/inet.h>
#include "helpers.h"

void my_inet_pton(int af, const char *src, void *dst);
int my_socket(int domain, int type, int protocol);
ssize_t my_sendto(int fd, const void *buf, size_t n, int flags, 
                    const struct sockaddr *addr, socklen_t addr_len);

ssize_t my_recvfrom(int sockfd, void *buf, size_t len, int flags,
                    struct sockaddr *addr, socklen_t *addr_len);



int my_atoi(char *src);

FILE *my_fopen(const char *filename, const char *mode);


#endif