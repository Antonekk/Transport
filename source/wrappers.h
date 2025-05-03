#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <arpa/inet.h>
#include "helpers.h"

void safe_inet_pton(int af, const char *src, void *dst);
int safe_socket(int domain, int type, int protocol);

int safe_atoi(char *src);

FILE *safe_fopen(const char *filename, const char *mode);

#endif