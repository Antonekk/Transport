#include <stdio.h>
#include <arpa/inet.h>

#include "helpers.h"



int main(int argc, char *argv[]){
    
    if (argc != 5){
        msg_exit("Usage: transport [ip address] [port number] [filename] [size]");
    }
    char *ip_addr = argv[1];
    char *filename = argv[3];


    int port_number;
    if (!(port_number = atoi(argv[2]))){
        msg_exit("Can't convert port to int");
    }
    int size;
    if (!(size = atoi(argv[4]))){
        msg_exit("Can't convert port to int");
    }

    printf("transport ip %s | port %d | filename %s | size %d\n", ip_addr, port_number, filename, size);
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(!socket_fd){
        error_exit("socket: ");
    }



}