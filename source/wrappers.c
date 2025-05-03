#include "wrappers.h"


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