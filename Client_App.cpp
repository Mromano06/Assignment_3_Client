#include "Receiver.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Matthew Romano - Brodie Arkell - Assignment 3 - Network Theory
// Client side apllication implementaion

int main() {        
    Receiver msgReceiver;

    // Create the socket
    int cSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (cSocket < 0) {
        std::cerr << "ERROR: Failed to create socket" << std::endl;
        return -1;
    } 

    // Define the servers address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(26500); // TODO: Decide on port number used
    server_addr.sin_addr.s_addr = inet_addr("172.16.5.12"); // TODO: Get the ip address of the server

    // Connect to the server
    if (connect(cSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "ERROR: Failed to connect to server" << std::endl;
        close(cSocket);
        return -1;
    }
    std::cout << "Server connection established." << std::endl;

    // Connected
    bool wantsToEnd = false;
    while(!wantsToEnd){
        // initialize menu
        Menu mainMenu;

        // run the main menu
        std::string packetToSend[4096] = mainMenu.run();

        // Send the packet
        if (send(cSocket, packetToSend, sizeof(packetToSend), 0) < 0) {
            std::cerr << "ERROR: Failed to send packet." << std::endl;
        }
        else {
            std::cout << "Packet sent successfully." << std::endl;
        }

        // Reciece a response
        char buffer[1024];
        int bytes = recv(cSocket, buffer, sizeof(buffer) -1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::string result = msgReceiver.checkCommands(buffer);
            std::cout << result;
        }
        else if (bytes == 0) {
            std::cout << "Connection to server terminated." << std::endl;
        }
        else {
            std::cerr << "ERROR: Failed to recieve packet." << std::endl;
        }

        std::string msgFromServer = msgReceiver.checkCommands(buffer);

        if (msgReceiver.terminationFlag == 1) {
        close(cSocket);
        return 0;
        }

    }

    close(cSocket);
    return 0;

}

