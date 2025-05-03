#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <arpa/inet.h>
#include "helpers.h"

void safe_inet_pton(int af, const char *src, void *dst);
int safe_socket(int domain, int type, int protocol);
ssize_t safe_sendto(int fd, const void *buf, size_t n, int flags, 
                    const struct sockaddr *addr, socklen_t addr_len);

ssize_t safe_recvfrom(int sockfd, const void *buf, size_t len, int flags,
                    struct sockaddr *addr, socklen_t *addr_len);

int safe_atoi(char *src);

FILE *safe_fopen(const char *filename, const char *mode);


#endif