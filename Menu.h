#pragma once

#include <iostream>
#include <string>

class Menu{

    public:

    Menu() = default;

    // runs the menu loop 
    std::string run();

    private:
    // handles a menu choice and updates the message
    void handleChoice(char choice, std::string& accumulatedMessage, bool& firstPost);

    std::string getLine(const std::string& prompt);

    // get character input safely
    char getInput();



};