#include <gtest/gtest.h>
#include "Receiver.h"

// Matthew Romano - Assignment 3 - Network Theory
// Reciever class unit tests

TEST(StringTest, ErrorExitsEarly) {
    Receiver parser;
    const char* recievedMessage = "}+{POST_ERROR}+{this shouldn't matter}";
    std::string expected = "SERVER ERROR: Command failed.";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

