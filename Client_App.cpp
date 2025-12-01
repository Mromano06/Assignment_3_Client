#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Matthew Romano - Brodie Arkell - Assignment 3 - Network Theory
// Client side apllication implementaion

int main() {
    std::string packetToSend[1024];
    
    // Create the socket
    int cSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (cSocket < 0) {
        std::cerr << "ERROR: Failed to create socket" << std::endl;
        return -1;
    }

    // Define the servers address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2222); // TODO: Decide on port number used
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.0"); // TODO: Get the ip address of the server

    // Connect to the server
    if (connect(cSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "ERROR: Failed to connect to server" << std::endl;
        close(cSocket);
        return -1;
    }
    std::cout << "Server connection established." << std::endl;

    // Build the packet

    // Send the packet
    if (send(cSocket, packetToSend, sizeof(packetToSend), 0) < 0) {
        std::cerr << "ERROR: Failed to send packet." << std::endl;
    }
    else {
        std::cout << "Packet sent successfully." << std::endl;
    }

    close(cSocket);
    return 0;

}

