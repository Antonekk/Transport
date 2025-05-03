#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include "helpers.h"
#include "wrappers.h"

#define DEBUG

#define MAX_PORT 65535
#define MAX_FILESIZE 10000000

#define MAX_REQUEST_LEN 50

#define MAX_RESPONSE_LEN 1100
#define MAX_RESPONSE_BODY_LEN 1000


int main(int argc, char *argv[]){
    
    // Print usage and exit
    if (argc != 5){
        msg_exit("Usage: transport <ip address> <port number> <filename> <size>");
    }

    // Parse arguments
    char *ip_addr = argv[1];
    int port_number = safe_atoi(argv[2]);
    char *filename = argv[3];
    int to_recive = safe_atoi(argv[4]);

    // CHeck if port is valid
    if (port_number < 0 || port_number > MAX_PORT){
        msg_exit("Port number outside of valid range");
    }

    #ifdef DEBUG
    printf("transport ip %s | port %d | filename %s | size %d\n", ip_addr, port_number, filename, to_recive);
    #endif

    // Open new socket
    int socket_fd = safe_socket(AF_INET, SOCK_DGRAM, 0);

    // Setup sockaddr_in structure with given data
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    socklen_t server_addr_len = sizeof(server_addr);

    // Translate string form of ip addr 
    safe_inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    // Open given file
    FILE *file = safe_fopen(filename, "w+");
    
    // Setup request/response buffors 
    char request_buf[MAX_REQUEST_LEN];
    char response_buf[MAX_RESPONSE_LEN];

    int start = 0;
    while (start < to_recive){

        // Setup request bufor
        int rq_length = to_recive - start < MAX_RESPONSE_BODY_LEN ? to_recive - start : MAX_RESPONSE_BODY_LEN;
        snprintf(request_buf, sizeof(request_buf), "GET %d %d\n", start, rq_length);

        safe_sendto(socket_fd, request_buf, strlen(request_buf), 0, (struct sockaddr*)&server_addr, server_addr_len);

        struct pollfd pfd= {
            .fd = socket_fd, 
            .events = POLLIN
        };
        int ready = poll(&pfd, 1, -1);
        if (ready < 0){
            error_exit("poll");
        }
        else if(ready == 0){
            continue;
        }


        recvfrom(socket_fd, response_buf, MAX_RESPONSE_LEN, 0, (struct sockaddr*)&(server_addr),&server_addr_len);


        start+=rq_length;
    }


    return EXIT_SUCCESS;
}