#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "helpers.h"
#include "wrappers.h"

#define MAX_PORT 65535
#define MAX_FILESIZE 10000000

#define MAX_REQUEST_LEN 50

#define MAX_RESPONSE_LEN 1100
#define MAX_RESPONSE_BODY_LEN 1000


int main(int argc, char *argv[]){
    
    if (argc != 5){
        msg_exit("Usage: transport [ip address] [port number] [filename] [size]");
    }
    char *ip_addr = argv[1];
    int port_number = safe_atoi(argv[2]);
    char *filename = argv[3];
    int to_recive = safe_atoi(argv[4]);

    if (port_number < 0 || port_number > MAX_PORT){
        msg_exit("Port number outside of valid range");
    }


    printf("transport ip %s | port %d | filename %s | size %d\n", ip_addr, port_number, filename, to_recive);
    int socket_fd = safe_socket(AF_INET, SOCK_DGRAM, 0);


    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    safe_inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    FILE *file = safe_fopen(filename, "w+");
    
    char request_buf[MAX_REQUEST_LEN];
    char response_buf[MAX_RESPONSE_LEN];

    int start = 0;
    while (start < to_recive){
        int rq_length = to_recive - start < MAX_RESPONSE_BODY_LEN ? to_recive - start : MAX_RESPONSE_BODY_LEN;
        snprintf(request_buf, sizeof(request_buf), "GET %d %d\n", start, rq_length);

        sendto(socket_fd, request_buf, strlen(request_buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    }


    return EXIT_SUCCESS;
}