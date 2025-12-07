#include <iostream>
#include <string>

// Matthew Romano - Assignment 3 - Network Theory
// Reciever class declaration

class Receiver {
    public:   
    std::string recvBuffer;
        int terminationFlag = 0;
        void appendData(const char* data);
        std::string processBuffer();
        std::string checkCommands(const char* buffer);     
};
