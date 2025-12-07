#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Button.H>
#include "Receiver.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Globals
int clientSocket;
Receiver msgReceiver; // Server Message Receiver
// input fields
Fl_Input* authorInput;
Fl_Input* titleInput;
Fl_Multiline_Input* messageInput;
// output fields
Fl_Text_Display* serverBox;
Fl_Text_Buffer* serverBuffer;
Fl_Multiline_Output* clientBox;
// accumilated posts
std::string savedPosts;


// Append text to output box
void appendServer(const std::string& text){
    serverBuffer->append((text + "\n").c_str());

    // auto-scroll to bottom
    int total_lines = serverBuffer->count_lines(0, serverBuffer->length());
    int visible_lines = serverBox->count_lines(0, serverBox->h(), 0);
    if(total_lines > visible_lines){
        serverBox->scroll(total_lines - visible_lines, 0);
    }
}


void appendClient(const std::string& text){
    clientBox->value(""); // clear previous message
    clientBox->value(text.c_str()); // add new message
} 

// Button callbacks
void sendPostCallBack(Fl_Widget*, void*){

    // check if saved posts are empty
    if (savedPosts.empty()){
        appendClient("There are no saved posts to send");
        return;
    }

    // add post ending symbol
    savedPosts.append("}}&{{");

    send(clientSocket, savedPosts.c_str(), savedPosts.size(), 0);
    appendClient("Post sent. "); // show sent msg

    // clear text field values
    authorInput->value(""); 
    titleInput->value("");
    messageInput->value("");    

    // clear saved posts after sending
    savedPosts.clear();

}


void savePostCallBack(Fl_Widget*, void*){
    // get text fields from post
    std::string author = authorInput->value();
    std::string title = titleInput->value();
    std::string msg = messageInput->value();
    
    // check for empty message
    if (author.empty()) author = "";
    if (title.empty()) title = "";
    if (msg.empty()) {
        appendClient("Post cannot be empty."); 
        return;
    }


    if (savedPosts.empty()){
        savedPosts.append("POST}+{"); // if first post add command
    } else{
        savedPosts.append("}#{"); // else add second post delimiter
    }

    // build protocol
    std::string post = author + "}+{" + title + "}+{" + msg;
    // save the post
    savedPosts.append(post);

    // clear text field values
    authorInput->value(""); 
    titleInput->value("");
    messageInput->value("");  
}


void getBoardCallBack(Fl_Widget*, void*){
    // clear previous server messages
    serverBuffer->text(""); // clear messages

    std::string packet = "GET_BOARD}}&{{";
    send(clientSocket, packet.c_str(), packet.size(), 0);
    appendClient("Requesting discussion board...");
}

void quitCallback(Fl_Widget*, void*){
    std::string packet = "QUIT}}&{{";
    send(clientSocket, packet.c_str(), packet.size(), 0);
    appendClient("Disconnected.");
    close(clientSocket);
    exit(0);
}


// Receiving function
void receiveTimeout(void*) {
    char buffer[2048];
    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
    if (bytes > 0){
        buffer[bytes] = '\0';

        // append to Receiver's internal buffer
        msgReceiver.appendData(buffer);

        // process any complete messages for GUI display
        std::string result = msgReceiver.processBuffer();
        if (!result.empty()){
            appendServer(result);

            // check for POST_OK / POST_ERROR in the result to update client log
            if (result.find("Post succeeded") != std::string::npos){
                appendClient("Post sent successfully.");
            }
            else if (result.find("SERVER ERROR") != std::string::npos){
                appendClient("Post Failed.");
            }
            else if (result.find("POST ERROR") != std::string::npos) {
                appendClient("Post fetch failed.");
            }
            
        }
    }

    // schedule the same function again
    Fl::repeat_timeout(0.1, receiveTimeout, nullptr);
}

int main(void){

    // Socket setup
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0){
        std::cerr << "ERROR: Failed to setup client socket";
        return 1; 
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(26500);
    server_addr.sin_addr.s_addr = inet_addr("172.16.5.12"); // server IP address

    if (connect(clientSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        std::cerr << "ERROR: Connection failed" << std::endl;
        return 1;
    }
    std::cout << "Connected to server." << std::endl;

    // FLTK Window
    // window size
    int winHeight = 500;
    int winWidth = 650;

    Fl_Window* window = new Fl_Window(winWidth, winHeight, "Discussion Board Client");
    window->resizable(window);

    // Input fields
    int fieldWidth = 360;
    int fieldHeight = 30;
    int x = (winWidth - fieldWidth) / 2; // center the field
    int y = 20;
    int spacing = 40;

    authorInput = new Fl_Input(x, y, fieldWidth, fieldHeight, "Author (Blank for Anonymous):");
    y += spacing; // add a space after each input field

    
    titleInput = new Fl_Input(x, y, fieldWidth, fieldHeight, "Title:");
    y += spacing;
    
    messageInput = new Fl_Multiline_Input(x, y, fieldWidth, 120, "Message:");
    y+= 140; // space for buttons

    // Buttons
    int btnWidth = 120;
    int btnHeight = 40;
    int btnSpacing = 20;
    int totalBtnWidth = 4 * btnWidth + 3 * btnSpacing;
    int btnX = (winWidth - totalBtnWidth) / 2; // Center the buttons   
    
    Fl_Button* saveButton = new Fl_Button(btnX, y, btnWidth, btnHeight, "Save Post");
    saveButton->callback(savePostCallBack);
    
    Fl_Button* sendButton = new Fl_Button(btnX + (btnWidth + btnSpacing), y, btnWidth, btnHeight, "Send Post");
    sendButton->callback(sendPostCallBack);

    Fl_Button* getBoardButton = new Fl_Button(btnX + 2*(btnWidth + btnSpacing), y, btnWidth, btnHeight, "Get Board");
    getBoardButton->callback(getBoardCallBack);

    Fl_Button* quitButton = new Fl_Button(btnX + 3*(btnWidth + btnSpacing),y, btnWidth, btnHeight, "Quit");
    quitButton->callback(quitCallback);

    y += btnHeight + 20;

    // client box
    // label above client log
    Fl_Box* clientLabel = new Fl_Box(20, y, 200, 20, "Client Log:");
    clientLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y += 25;
    clientBox = new Fl_Multiline_Output(20, y, window->w() - 40, 80);
    y+= 100;

   // Server output (fills remaining window)
   // label above server output
    Fl_Box* serverLabel = new Fl_Box(20, y, 200, 20, "Server Output:");
    serverLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    y += 25;
    serverBuffer = new Fl_Text_Buffer();
    serverBox = new Fl_Text_Display(20, y, window->w() - 40, window->h() - y - 20);
    serverBox->buffer(serverBuffer);
    serverBox->wrap_mode(1, 0); // wrap at words

    window->resizable(serverBox);
    window->end();
    window->show();


    // Start receiving
    Fl::repeat_timeout(0.1, receiveTimeout, nullptr);


        return Fl::run();


    }


