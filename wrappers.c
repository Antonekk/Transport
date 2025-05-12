#include "wrappers.h"



int my_socket(int domain, int type, int protocol){
    int s_fd = socket(domain, type, protocol);
    if(!s_fd){
        error_exit("socket");
    }
    return s_fd;
}

ssize_t my_sendto(int fd, const void *buf, size_t n, int flags, const struct sockaddr *addr, socklen_t addr_len){
    ssize_t result = sendto(fd, buf, n, flags, addr, addr_len);
    if(result < 0){
        error_exit("sendto");
    }
    return result;
}

ssize_t my_recvfrom(int sockfd, void *buf, size_t len, int flags,
                    struct sockaddr *addr, socklen_t *addr_len){
    ssize_t result = recvfrom(sockfd, buf, len, flags, addr, addr_len);
    if (result < 0){
        error_exit("recvfrom");
    }
    return result;
}


void *my_calloc(size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    if (!ptr)
      error_exit("Calloc error");
    return ptr;
}

int my_clock_gettime(clockid_t clockid, struct timespec *tp){
    int res = clock_gettime(clockid, tp);
    if(res < 0){
        error_exit("clock_gettime error");
    }
    return res;
}


void my_inet_pton(int af, const char *src, void *dst){
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

int my_atoi(char *src){
    int result;
    if (!(result = atoi(src))){
        msg_exit("Can't convert port to int");
    }
    return result;
}

FILE *my_fopen(const char *filename, const char *mode){
    FILE *fd = fopen(filename, mode);
    if(!fd){
        error_exit("fopen");
    }
    return fd;
}