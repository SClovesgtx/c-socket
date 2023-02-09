#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int main(){
    // CREATE A SOCKET
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // SPECIFY AN ADDRESS FOR THE SOCKET
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3000);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    return 0;
}