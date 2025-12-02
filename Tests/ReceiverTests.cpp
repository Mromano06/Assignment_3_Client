#include <gtest/gtest.h>
#include "Receiver.h"

// Matthew Romano - Assignment 3 - Network Theory
// Reciever class unit tests

// Checks to see if error was returned early
TEST(StringTest, ErrorExitsEarly) {
    Receiver parser;
    const char* recievedMessage = "}+{POST_ERROR}+{this shouldn't matter}";
    std::string expected = "SERVER ERROR: Command failed.";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

TEST(StringTest, PostOkMessage) {
    Receiver parser;
    const char* recievedMessage = "}+{POST_OK}+{Hello World}";
    std::string expected =
        "Post succeeded.\n"
        " - Hello World\n";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

TEST(StringTest, MessagesCommand) {
    Receiver parser;
    const char* recievedMessage = "}+{MESSAGES}+{First}+{Second}";
    std::string expected =
        "Post succeeded:\n"
        " - First\n"
        " - Second\n";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

TEST(StringTest, DisconnectCommand) {
    Receiver parser;
    const char* recievedMessage = "}+{DISCONNECT}";
    std::string expected =
        "Dissconnet recieved. Connection closing...\n";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

TEST(StringTest, MixedCommands) {
    Receiver parser;
    const char* recievedMessage = "}+{POST_OK}+{MESSAGES}+{Item1}+{Item2}";
    std::string expected =
        "Post succeeded.\n"
        "Post succeeded:\n"
        " - Item1\n"
        " - Item2\n";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

TEST(StringTest, EmptyBuffer) {
    Receiver parser;
    const char* recievedMessage = "";
    std::string expected = "";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}

TEST(StringTest, UnknownCommand) {
    Receiver parser;
    const char* recievedMessage = "}+{FOO}+{BAR}";
    std::string expected =
        " - FOO\n"
        " - BAR\n";
    EXPECT_EQ(parser.checkCommands(recievedMessage), expected);
}
