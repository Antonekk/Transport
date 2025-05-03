#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "helpers.h"
#include "wrappers.h"

#define MAX_PORT 65535
#define MAX_FILESIZE 10000000
#define MAX_DATAGRAM_DATA_LENGTH 1000

int main(int argc, char *argv[]){
    
    if (argc != 5){
        msg_exit("Usage: transport [ip address] [port number] [filename] [size]");
    }
    char *ip_addr = argv[1];
    int port_number = safe_atoi(argv[2]);
    char *filename = argv[3];
    int size = safe_atoi(argv[4]);

    if (port_number < 0 || port_number > MAX_PORT){
        msg_exit("Port number outside of valid range");
    }


    printf("transport ip %s | port %d | filename %s | size %d\n", ip_addr, port_number, filename, size);
    int socket_fd = safe_socket(AF_INET, SOCK_DGRAM, 0);


    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    safe_inet_pton(AF_INET, ip_addr, &server_addr.sin_addr);

    



    return EXIT_SUCCESS;
}