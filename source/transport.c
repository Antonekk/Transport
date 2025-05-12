#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>
#include <math.h>

#include "helpers.h"
#include "wrappers.h"

#define DEBUG


void setup_server_addr(struct sockaddr_in *server_addr, int port_number, char *ip_addr){
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port_number);

    // Translate string form of ip addr 
    my_inet_pton(AF_INET, ip_addr, &server_addr->sin_addr);

}


// After my advanced calculations this won't result in momory usage > 8MB (i hope)
void init_window(datagram_t *window, size_t to_send, int to_recive){
    for(int i = 0; i < to_send; i++){
        int start = i * MAX_RESPONSE_BODY_LEN;
        window[i].start = start;
        window[i].length = to_recive - start < MAX_RESPONSE_BODY_LEN ? to_recive - start : MAX_RESPONSE_BODY_LEN;
        window[i].status = IDLE;
    }
}


void send_datagrams(int socket_fd, frame_t *window, struct sockaddr_in *server_addr){
    socklen_t server_addr_len = sizeof(*server_addr);

    struct timespec current_time;
    my_clock_gettime(CLOCK_MONOTONIC, &current_time);

    datagram_t *cur_window = window->frame_ptr;
    int frame_index = window->frame_index;
    for(int i = 0; i<SENDER_WINDOW_SIZE; i++){
        int idx = frame_index + i;
        if(cur_window[idx].status == IDLE || (cur_window[idx].status == SENT 
            && time_diff_ms(&cur_window[idx].timestamp, &current_time)) > RESEND_TIME){
            char request_buf[MAX_REQUEST_LEN];
            int n = snprintf(request_buf, sizeof(request_buf), "GET %d %d\n", cur_window[idx].start, cur_window[idx].length);
            if (n < 0) {
                error_exit("snprintf");
            }
            my_sendto(socket_fd,request_buf , strlen(request_buf), 0, (struct sockaddr*)server_addr, server_addr_len); 
            #ifdef DEBUG
            printf("Sent\n");
            #endif   
            cur_window[idx].status = SENT;
            cur_window[idx].timestamp = current_time;
        }
    }
}

void transport(int socket_fd, int to_recive, struct sockaddr_in *server_addr, FILE* file ){
    

    frame_t window;
    window.frame_index = 0;
    window.datagrams_count = to_recive / MAX_RESPONSE_BODY_LEN + (to_recive % MAX_RESPONSE_BODY_LEN != 0);
    window.frame_ptr = my_calloc(window.datagrams_count, sizeof(datagram_t));
    init_window(window.frame_ptr, window.datagrams_count, to_recive);
    
    
    struct pollfd pfd= {
        .fd = socket_fd, 
        .events = POLLIN
    };


    // Loop until all data is recived
    while (window.frame_index < window.datagrams_count){

        
        send_datagrams(socket_fd, &window, server_addr);

        int ready = poll(&pfd, 1, TIMEOUT);
        if (ready < 0){
            error_exit("poll");
        }
        else if(ready == 0){
            continue;
        }

        char response_buf[MAX_RESPONSE_LEN];
        struct sockaddr_in sender_addr;
        socklen_t sender_len = sizeof(sender_addr);
        my_recvfrom(socket_fd, response_buf, MAX_RESPONSE_LEN, 0, (struct sockaddr*)&sender_addr,&sender_len);
        if (sender_addr.sin_addr.s_addr != server_addr->sin_addr.s_addr || sender_addr.sin_port != server_addr->sin_port){
            #ifdef DEBUG
            printf("Recived datagram from invalid sender");
            #endif
            continue;
        }
        
        #ifdef DEBUG
        printf("Recived\n");
        #endif   

        int response_start;
        int response_length;
        int header_len;
        int tokens = sscanf(response_buf, "DATA %d %d\n%n", &response_start, &response_length, &header_len);
        if (tokens != 2){
            // Check if correctly matched
            continue;
        }

        int idx = response_start / MAX_RESPONSE_BODY_LEN;
        if(idx < window.frame_index 
            || idx >= window.frame_index + SENDER_WINDOW_SIZE
            || window.frame_ptr[idx].start != response_start
            || window.frame_ptr[idx].length != response_length
            || window.frame_ptr->status == RECIVED){
            continue;
        }

        memcpy(window.frame_ptr[idx].data, response_buf+header_len, response_length);
        window.frame_ptr[idx].status = RECIVED;

        while(window.frame_index < window.datagrams_count 
              && window.frame_ptr[window.frame_index].status == RECIVED){
            
            datagram_t w_datagram = window.frame_ptr[window.frame_index];
            size_t written = fwrite(w_datagram.data, sizeof(char), w_datagram.length, file);
            if (written < response_length){
                error_exit("fwrite");
            }
            window.frame_index++;
        }
        
    }
}


int main(int argc, char *argv[]){
    
    // Print usage and exit
    if (argc != 5){
        msg_exit("Usage: transport <ip address> <port number> <filename> <size>");
    }

    // Parse arguments
    char *ip_addr = argv[1];
    int port_number = my_atoi(argv[2]);
    char *filename = argv[3];
    int to_recive = my_atoi(argv[4]);

    // CHeck if port is valid
    if (port_number < 0 || port_number > MAX_PORT){
        msg_exit("Port number outside of valid range");
    }

    #ifdef DEBUG
    printf("transport ip %s | port %d | filename %s | size %d\n", ip_addr, port_number, filename, to_recive);
    #endif

    // Open new socket
    int socket_fd = my_socket(AF_INET, SOCK_DGRAM, 0);

    // Setup sockaddr_in structure with given data
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    setup_server_addr(&server_addr, port_number, ip_addr);
    
    
    // Open given file (w+ is equal to open's O_RDWR | O_CREAT | O_TRUNC flags)
    FILE *file = my_fopen(filename, "w+");
   
    transport(socket_fd, to_recive, &server_addr, file);

    close(socket_fd);
    fclose(file);
    return EXIT_SUCCESS;
}