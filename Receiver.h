#include <iostream>
#include <string>

// Matthew Romano - Assignment 3 - Network Theory
// Reciever class declaration

class Receiver {
    public:   
        int terminationFlag = 0;        
        int getTermFlag();
        void setTermFlag(int terminationFlag);
        std::string checkCommands(const char* buffer);     
};
