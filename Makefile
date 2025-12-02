# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# Target executable
TARGET = client_app

# Source file(s)
SRC = Client_App.cpp Menu.cpp Reciever.cpp

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean target
clean:
	rm -f $(TARGET)
