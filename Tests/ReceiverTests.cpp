#include "Receiver.h"
#include <gtest/gtest.h>

// Fixture for Receiver tests
class ReceiverTest : public ::testing::Test {
protected:
    Receiver receiver;
};

TEST_F(ReceiverTest, HandlesPostOk) {
    const char* buffer = "}+{POST_OK}+{";
    std::string result = receiver.checkCommands(buffer);
    EXPECT_EQ(result, "Post succeeded:");
}

TEST_F(ReceiverTest, HandlesPostError) {
    const char* buffer = "}+{POST_ERROR}+{";
    std::string result = receiver.checkCommands(buffer);
    EXPECT_EQ(result, "SERVER ERROR: Command failed.");
}

TEST_F(ReceiverTest, HandlesMessages) {
    const char* buffer = "}+{MESSAGES}+{Hello}+{World}+{";
    std::string result = receiver.checkCommands(buffer);
    EXPECT_EQ(result, "\n- Hello\n- World");
}

TEST_F(ReceiverTest, HandlesDisconnect) {
    const char* buffer = "}+{DISCONNECT}+{";
    std::string result = receiver.checkCommands(buffer);
    EXPECT_EQ(result, "Dissconnet recieved. Connection closing...");
}

TEST_F(ReceiverTest, HandlesMultipleCommands) {
    const char* buffer = "}+{POST_OK}+{Hello}+{World}+{";
    std::string result = receiver.checkCommands(buffer);
    EXPECT_EQ(result, "Post succeeded:\n- Hello\n- World");
}


TEST_F(ReceiverTest, HandlesUnknownCommand) {
    const char* buffer = "}+{FOO}+{";
    std::string result = receiver.checkCommands(buffer);
    EXPECT_EQ(result, "\n- FOO");
}