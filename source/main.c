#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include "helpers.h"
#include "wrappers.h"

#define DEBUG

#define TIMEOUT 1000

#define MAX_PORT 65535
#define MAX_FILESIZE 10000000

#define MAX_REQUEST_LEN 50

#define MAX_RESPONSE_LEN 1100
#define MAX_RESPONSE_BODY_LEN 1000


void setup_server_addr(struct sockaddr_in *server_addr, int port_number, char *ip_addr){
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port_number);

    // Translate string form of ip addr 
    safe_inet_pton(AF_INET, ip_addr, &server_addr->sin_addr);

}


void transport(int socket_fd, int to_recive, struct sockaddr_in *server_addr, FILE* file ){
    
    socklen_t server_addr_len = sizeof(*server_addr);
    
    // Setup request/response buffors 
    char request_buf[MAX_REQUEST_LEN];
    char response_buf[MAX_RESPONSE_LEN];

    int start = 0;
    while (start < to_recive){

        // Setup request bufor
        int rq_length = to_recive - start < MAX_RESPONSE_BODY_LEN ? to_recive - start : MAX_RESPONSE_BODY_LEN;
        int n = snprintf(request_buf, sizeof(request_buf), "GET %d %d\n", start, rq_length);
        if (n < 0) {
            error_exit("snprintf");
        }

        safe_sendto(socket_fd, request_buf, strlen(request_buf), 0, (struct sockaddr*)server_addr, server_addr_len); 
        #ifdef DEBUG
        printf("Sent\n");
        #endif   

        struct pollfd pfd= {
            .fd = socket_fd, 
            .events = POLLIN
        };
        int ready = poll(&pfd, 1, TIMEOUT);
        if (ready < 0){
            error_exit("poll");
        }
        else if(ready == 0){
            continue;
        }


        safe_recvfrom(socket_fd, response_buf, MAX_RESPONSE_LEN, 0, (struct sockaddr*)server_addr,&server_addr_len);
        
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

        if (response_start != start || response_length != rq_length){
            continue;
        }
        size_t written = fwrite(response_buf+header_len, sizeof(char), response_length, file);
        if (written < response_length){
            error_exit("fwrite");
        }


        start+=rq_length;
    }
}


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
    setup_server_addr(&server_addr, port_number, ip_addr);
    
    
    // Open given file (w+ is equal to open's O_RDWR | O_CREAT | O_TRUNC flags)
    FILE *file = safe_fopen(filename, "w+");
    
    transport(socket_fd, to_recive, &server_addr, file);

    close(socket_fd);
    fclose(file);
    return EXIT_SUCCESS;
}