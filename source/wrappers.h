#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <arpa/inet.h>
#include "helpers.h"

void safe_inet_pton(int af, const char *src, void *dst);
int safe_atoi(char *src);

#endif