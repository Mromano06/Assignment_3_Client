# Assignment 3 Network Theory

### Disclaimer
This project was designed to test our skills with netowrks and VMs. Therefore our enivironments used to create this projet were within a VM.

## Description
We were tasked with building a simple message board application. One part of the group was focused on the server and the other on the client.
This is the client side gui/application. This app handles sending and recieveing messages/posts from a emssage board. The board then stores these messages and allows us to view them.

## Setup
Asumming you have both git and cmake installed...
1. Clone the repo
2. Open the terminal and run "cd build"
3. Then run "sudo apt install libfltk1.3-dev" to install dependencies for the GUI
4. If a cache error appears this is due to left over cmake build paths, run "rm -rf build" (shouldn't appear more then once)
5. Then run "make .."
6. Then compile by running "make"
7. Then we are going to need to download the second, server side repo (link below)
8. To run/compile follow the steps in the server's read me
9. To run ther client GUI app run "./server_gui" in the client's terminal
10. If the GUI is broken/fails used "./server_app"
