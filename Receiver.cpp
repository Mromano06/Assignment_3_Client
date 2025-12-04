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

int Receiver::getTermFlag() {
    return terminationFlag;
}

void Receiver::setTermFlag(int teminationFlag) {
    this->terminationFlag = terminationFlag;
}

std::string Receiver::checkCommands(const char* buffer) {
    std::string response(buffer); // convert to a string

    std::vector<std::string> parts;
    size_t start = 0, end;
    const std::string delim = "}+{";
    const std::string messageDelim = "}#{";

      // First check if we have a message delimiter
    size_t msgPos = response.find(messageDelim);
    std::string commandSection = response;
    std::string messageSection;

       if (msgPos != std::string::npos) {
        // Split into command part and message part
        commandSection = response.substr(0, msgPos);
        messageSection = response.substr(msgPos + messageDelim.length());
    }

    // Parse the command section using "}+{"
    start = 0;
    while ((end = commandSection.find(delim, start)) != std::string::npos) {
        parts.push_back(commandSection.substr(start, end - start));
        start = end + delim.length();
    }
    parts.push_back(commandSection.substr(start));

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
        else if (part == "GET_BOARD_ERROR") {
            out << "MESSAGE ERROR: Message send failed.";
            return out.str(); // Return error message asap
        }
        else if (part == "MESSAGES") {
            // handled separately below
        }
        else if (part == "DISCONNECT") {
            terminationFlag = 1;
        }
        else if (!part.empty()) {
            out << "\n- " << part;
        }
    }

    // If there was a message section after "}#{", append it
    if (!messageSection.empty()) {
        std::string trimmedMsg = trimBraces(messageSection);
        if (!trimmedMsg.empty()) {
            out << "\nMessage:\n" << trimmedMsg;
        }
    }

    return out.str(); // Return the success/other messages
}
