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
    std::string response(buffer);

    std::vector<std::string> parts;
    size_t start = 0, end;
    const std::string delim = "}+{";   // command delimiter
    const std::string msgDelim = "}#{"; // message body delimiter

    // Split commands by "}+{"
    while ((end = response.find(delim, start)) != std::string::npos) {
        parts.push_back(response.substr(start, end - start));
        start = end + delim.length();
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
            return out.str();
        }
        else if (part.rfind("MESSAGES", 0) == 0) {
            // Everything after "MESSAGES" may contain message bodies
            std::string messages = part.substr(std::string("MESSAGES").length());
            if (!messages.empty()) {
                // Split by "}#{"
                size_t mStart = 0, mEnd;
                while ((mEnd = messages.find(msgDelim, mStart)) != std::string::npos) {
                    std::string msg = trimBraces(messages.substr(mStart, mEnd - mStart));
                    if (!msg.empty()) out << "\n- " << msg;
                    mStart = mEnd + msgDelim.length();
                }
                std::string lastMsg = trimBraces(messages.substr(mStart));
                if (!lastMsg.empty()) out << "\n- " << lastMsg;
            }
        }
        else if (part == "DISCONNECT") {
            terminationFlag = 1;
        }
        else if (!part.empty()) {
            out << "\n- " << part;
        }
    }

    return out.str();
}