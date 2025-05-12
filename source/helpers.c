#include <helpers.h>


int time_diff_ms(struct timespec *prev, struct timespec *cur){
    return (cur->tv_sec - prev->tv_sec) * 1000 + (cur->tv_nsec - prev->tv_nsec) / 1000000;
}

datagram_t *get_from_circular(datagram_t *frame_ptr, int idx){
    int slot = idx % SENDER_WINDOW_SIZE;
    return &frame_ptr[slot];
}

void error_exit(const char *msg){
     fprintf(stderr, "%s: %s\n", msg, strerror(errno));
     exit(EXIT_FAILURE);
}

void msg_exit(const char *msg){
    fprintf(stderr, "Exit: %s\n", msg);
    exit(EXIT_FAILURE);
}