#ifndef WINDOWS_INTERFACE
#define WINDOWS_INTERFACE

#include <winsock2.h>
#include <string>

#define BUFFER_SIZE 1024


class WindowsInterface{
    public:
        //Constructor
        WindowsInterface(int port);
        //Destructor
        ~WindowsInterface();
        
        //Functions
        void wait_for_client();
        void send_msg(std::string message);
        std::string rcv_msg();

    private:
        WSADATA wsaData;    //Stores information about Windows Sockets
        SOCKET server, client;      //Sockets for server and client
        struct sockaddr_in serverAddr, clientAddr;      //Addresses for server & client
        int port;          //The size of the buffer
        char buffer[BUFFER_SIZE];       //The buffer
        int clientAddrLength = sizeof(clientAddr);      //The length of the client address
};
#endif