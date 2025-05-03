#include "wrappers.h"


int safe_socket(int domain, int type, int protocol){
    int s_fd = socket(domain, type, protocol);
    if(!s_fd){
        error_exit("socket");
    }
    return s_fd;
}


void safe_inet_pton(int af, const char *src, void *dst){
    int res = inet_pton(af, src, dst);
    if (res == 1){
        return;
    }
    if (res == 0){
        msg_exit("src does not contain a string representing a valid network address in the specified address family");
    }
    else{{
        error_exit("inet_pton");
    }}
}

int safe_atoi(char *src){
    int result;
    if (!(result = atoi(src))){
        msg_exit("Can't convert port to int");
    }
    return result;
}

FILE *safe_fopen(const char *filename, const char *mode){
    FILE *fd = fopen(filename, mode);
    if(!fd){
        error_exit("fopen");
    }
    return fd;
}