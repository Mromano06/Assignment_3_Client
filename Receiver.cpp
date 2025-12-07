#include "Receiver.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

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

// Append incoming data to buffer
void Receiver::appendData(const char* data){
    recvBuffer += data;
}

// Process all complete messages in the buffer
std::string Receiver::processBuffer(){
    std::string displayText;
    size_t pos;
    const std::string terminator = "}}&{{"; // message terminator

    while ((pos = recvBuffer.find(terminator)) != std::string::npos){
        std::string fullMsg = recvBuffer.substr(0, pos); // take one full message
        std::string result = checkCommands(fullMsg.c_str()); // process step by step
        if (!result.empty()){
            displayText += result + "\n";
        }
        recvBuffer.erase(0, pos + terminator.length()); // remove processed message
    }

    return displayText;
}

// Process a single message step by step
std::string Receiver::checkCommands(const char* buffer) {
    std::string response(buffer);
    std::ostringstream out;

    // Step 1: Trim braces
    response = trimBraces(response);

    // Step 2: Check for single commands
    if (response == "POST_OK") {
        out << "Post succeeded:";
        return out.str();
    }
    if (response == "POST_ERROR") {
        return "SERVER ERROR: Command failed.";
    }
    if (response == "DISCONNECT") {
        terminationFlag = 1;
        return "Disconnected";
    }

    // Step 3: Handle GET_BOARD
    if (response.rfind("GET_BOARD", 0) == 0) {
        std::string messages = response.substr(std::string("GET_BOARD").length());
        messages = trimBraces(messages); // remove outer braces if present

        const std::string msgDelim = "}#{"; // message separator
        const std::string partDelim = "}+{"; // author/title/body separator
        size_t start = 0, end;
        bool firstMsg = true;

        // Process each message block
        while ((end = messages.find(msgDelim, start)) != std::string::npos) {
            std::string msgBlock = trimBraces(messages.substr(start, end - start));

            // Split block into parts
            size_t subStart = 0, subEnd;
            while ((subEnd = msgBlock.find(partDelim, subStart)) != std::string::npos) {
                std::string part = trimBraces(msgBlock.substr(subStart, subEnd - subStart));
                if (!part.empty()) {
                    if (!firstMsg) out << "\n";
                    out << "- " << part;
                    firstMsg = false;
                }
                subStart = subEnd + partDelim.length();
            }
            // Last part of the block
            std::string lastPart = trimBraces(msgBlock.substr(subStart));
            if (!lastPart.empty()) {
                if (!firstMsg) out << "\n";
                out << "- " << lastPart;
                firstMsg = false;
            }

            start = end + msgDelim.length();
            if (start < messages.length()) out << "\n===========\n\n"; // separator between messages
        }

        // Handle last message block (if any)
        std::string lastMsgBlock = trimBraces(messages.substr(start));
        if (!lastMsgBlock.empty()) {
            size_t subStart = 0, subEnd;
            while ((subEnd = lastMsgBlock.find(partDelim, subStart)) != std::string::npos) {
                std::string part = trimBraces(lastMsgBlock.substr(subStart, subEnd - subStart));
                if (!part.empty()) {
                    if (!firstMsg) out << "\n";
                    out << "- " << part;
                    firstMsg = false;
                }
                subStart = subEnd + partDelim.length();
            }
            std::string lastPart = trimBraces(lastMsgBlock.substr(subStart));
            if (!lastPart.empty()) {
                if (!firstMsg) out << "\n";
                out << "- " << lastPart;
            }
        }

        return out.str();
    }

    // Step 4: Otherwise, treat as normal line
    out << "- " << response;
    return out.str();
}
