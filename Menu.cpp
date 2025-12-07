#include "Menu.h"
#include <limits>


std::string Menu::run(){
    bool running = true;
    bool firstPost = true;
    std::string accumulatedMessage; // holds accumilated messages/posts

    while (running) {
        //Print Menu
        std::cout << "--------------- Main Menu ---------------" << std::endl;
        std::cout << "--------------- a) Create Post ---------------" << std::endl;
        std::cout << "--------------- b) See Discussion Board ---------------" << std::endl;
        std::cout << "--------------- q) quit ---------------" << std::endl;
        std::cout << "Select Option: ";

        char choice = getInput();

        switch (choice){
            case 'a':
            case 'A': 
            // handle posts
            handleChoice(choice, accumulatedMessage, firstPost);
            // append end marker
            accumulatedMessage += "}}&{{";
            return accumulatedMessage;

            case 'b':
            case 'B': 
            return "GET_BOARD}}&{{";

            case 'q':
            case 'Q':
            return "QUIT}}&{{";

            default:
            std::cout << "Invalid option." << std::endl;
            break;
        }
    
    } 
    return "ERROR";
}

char Menu::getInput(){
    char input;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
    return input;
}

std::string Menu::getLine(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

void Menu::handleChoice(char choice, std::string& accumulatedMessage, bool& firstPost){
    std::string command = "POST";
    bool addAnotherPost = true;

    // loop for multiple posts
    while(addAnotherPost){

        // Author (optional)
        std::string author = getLine("Enter author (Leave blank for Anonymous): ");
        if (author.empty()){
            author = "Anonymous";
        }

        // Title (optional)
        std::string title = getLine("Enter title (Leave blank for Anonymous): ");
        if (title.empty()){
            title = "Untitled";
        }

        // Message (required)
        std::string msg;
        do {
            msg = getLine("Enter Post: ");
            if (msg.empty()){
                std::cout << "Post cannot be empty" << std::endl;
            } 
        } while(msg.empty());
    

        // Format POST for protocol
        std::string post = author + "}+{" + title + "}+{" + msg;

        // Add command to first post 
        if (firstPost){
            accumulatedMessage = command + "}+{" + post;
            firstPost = false;
        } else{
            accumulatedMessage += "}#{" + post;
        }

        // Add another post?
        std::string again = getLine("Add another post? (Y/n): ");
        if (again != "y" && again != "Y"){
            addAnotherPost = false;
        }
    }
}