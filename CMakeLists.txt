cmake_minimum_required(VERSION 3.10)
project(ClientApp)

# Find FLTK
find_package(FLTK REQUIRED)

# CLI version (optional, keeps your old console app)
add_executable(client_cli Client_App.cpp Receiver.cpp Menu.cpp)
target_link_libraries(client_cli fltk)

# GUI version
add_executable(client_gui ClientGUI.cpp Receiver.cpp Menu.cpp)
target_link_libraries(client_gui fltk)
