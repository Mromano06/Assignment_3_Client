#include "Reciever.h"
#include <iostream>
#include <sstream>
#include <vector>

// Matthew Romano - Assignment 3 - Network Theory
// Reciever class implementation

std::string Reciever::checkCommands(char* buffer) {
    std::string response(buffer); // convert to a string

    std::vector<std::string> parts;
    size_t start = 0, end;
    const std::string delim = "}+{";

    // Sorts through the string using the delim to sort substrings
    while ((end = response.find(delim, start)) != std::string::npos) {
        parts.push_back(response.substr(start, end - start));
        start = end +delim.length();
    }
    parts.push_back(response.substr(start));

    std::ostringstream out;

    for (const auto& part : parts) {
        if (part == "POST_OK") {
            out << "Post succeeded." << std::endl;
        }
        else if (part == "POST_ERROR") {
            out << "SERVER ERROR: Command failed." << std::endl;
            return; // Return error message asap
        }
        else if (part == "MESSAGES") {
            out << "Post succeeded:" << std::endl;
        }
        else if (part == "DISCONNECT") {
            out << "Dissconnet recieved. Connection closing..." << std::endl;
        }
        else {
            out << " - " << part << std::endl;
        }
    }

    return out.str(); // Return the success/other messages
}