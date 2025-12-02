cmake_minimum_required(VERSION 3.10)
project(MyProjectTests)

enable_testing()

add_subdirectory(googletest)

add_executable(ReceiverTests
    Tests/ReceiverTests.cpp
    Receiver.cpp
)

target_include_directories(ReceiverTests PRIVATE .)

target_link_libraries(ReceiverTests
    gtest
    gtest_main
    pthread
)

add_test(NAME ReceiverTests COMMAND ReceiverTests)
