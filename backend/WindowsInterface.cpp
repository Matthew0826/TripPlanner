#include "WindowsInterface.h"

//Constructor implementation
WindowsInterface::WindowsInterface(int port){
    //Initialize WinSock & make sure it is working
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { return; }

    //Create a socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
    serverAddr.sin_port = htons(port); // Port 8111

    //If the socket is bad, return
    if (server == INVALID_SOCKET) { WSACleanup(); return; }

    //Bind the socket
    bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 1);

}

//Destructor
WindowsInterface::~WindowsInterface(){
    // Cleanup
    closesocket(client);
    closesocket(server);
    WSACleanup();
}

void WindowsInterface::wait_for_client(){
    client = accept(server, (struct sockaddr*)&clientAddr, &clientAddrLength);
}

void WindowsInterface::send_msg( std::string message ){
    send(client, message.c_str(), strlen(message.c_str()), 0);
}

std::string WindowsInterface::rcv_msg(){
    //Get the number of bytes
    int bytesReceived = recv(client, buffer, BUFFER_SIZE - 1, 0);
    
    //Create the string and return it
    std::string s;
    for( int i = 0; i < bytesReceived; i++ ){
        s += buffer[i];
    }
    return s;
}
