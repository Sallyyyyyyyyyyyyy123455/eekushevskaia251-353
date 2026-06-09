#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstring>
#include <sstream>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

class Server {
public:
    Server(int port);
    ~Server();
    void start();

private:
    void handleClient(SOCKET clientSocket);
    std::string processRequest(const std::string& request);
    
    SOCKET serverSocket;
    int port;
    bool running;
};

#endif
