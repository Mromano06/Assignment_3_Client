#include "Receiver.h"
#include <iostream>
#include <sstream>
#include <vector>

// Matthew Romano - Assignment 3 - Network Theory
// Receiver class implementation

// Trims off the braces of the message
static std::string trimBraces(const std::string& s) {
    std::string result = s;
    if (!result.empty() && result.front() == '{') {
        result.erase(result.begin());
    }
    if (!result.empty() && result.back() == '}') {
        result.pop_back();
    }
    return result;
}

std::string Receiver::checkCommands(const char* buffer) {
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

    for (const auto& rawPart : parts) {
        std::string part = trimBraces(rawPart);
        
        if (part == "POST_OK") {
            out << "Post succeeded:";
        }
        else if (part == "POST_ERROR") {
            out << "SERVER ERROR: Command failed.";
            return out.str(); // Return error message asap
        }
        else if (part == "MESSAGES") {
            out << "";
        }
        else if (part == "DISCONNECT") {
            out << "Dissconnet recieved. Connection closing...";
        }
        else if (!part.empty()) {
            out << "\n- " << part;
        }
    }

    return out.str(); // Return the success/other messages
}