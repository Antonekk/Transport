// Antoni Strasz 339096

#ifndef HELPERS_H
#define HELPERS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define TIMEOUT 1000

#define MAX_PORT 65535
#define MAX_FILESIZE 10000000

#define MAX_REQUEST_LEN 50

#define MAX_RESPONSE_LEN 1100
#define MAX_RESPONSE_BODY_LEN 1000


// Window

#define SENDER_WINDOW_SIZE 5

#define IDLE 1
#define SENT 2
#define RECIVED 3

typedef struct Datagram{
    int status;
    int start;
    int length;
    struct timespec timestamp;
    char data[MAX_RESPONSE_LEN];
} datagram_t;



// Error handling
void error_exit(const char *msg);
void msg_exit(const char *msg);



#endif