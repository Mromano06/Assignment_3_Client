## Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++17 -I. -I./googletest/include
LDFLAGS = -L./googletest/build/lib -lgtest -lgtest_main -lpthread

# Target executable
TARGET = client_app

# Source file(s)
SRC = Client_App.cpp Menu.cpp Receiver.cpp

# Build target (main app)
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Test target (Google Test)
Tests/ReceiverTests: Tests/ReceiverTests.o Receiver.o
	$(CXX) -o $@ $^ $(LDFLAGS)

Tests/ReceiverTests.o: Tests/ReceiverTests.cpp Receiver.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

Receiver.o: Receiver.cpp Receiver.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) Tests/*.o *.o Tests/ReceiverTests