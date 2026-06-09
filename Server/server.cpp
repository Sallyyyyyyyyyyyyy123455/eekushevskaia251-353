#include "server.h"
#include "database.h"
#include "vigenere.h"

Server::Server(int port) : port(port), running(false) {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
}

Server::~Server() {
    closesocket(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif
}

void Server::start() {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    listen(serverSocket, 5);
    running = true;
    std::cout << "Server started on port " << port << std::endl;
    
    while (running) {
        SOCKET client = accept(serverSocket, nullptr, nullptr);
        if (client != INVALID_SOCKET) {
            std::thread(&Server::handleClient, this, client).detach();
        }
    }
}

void Server::handleClient(SOCKET clientSocket) {
    char buffer[4096] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        std::string response = processRequest(std::string(buffer));
        send(clientSocket, response.c_str(), response.size(), 0);
    }
    closesocket(clientSocket);
}

std::string Server::processRequest(const std::string& request) {
    // Формат: COMMAND|arg1|arg2
    std::vector<std::string> parts;
    std::stringstream ss(request);
    std::string item;
    while (getline(ss, item, '|')) {
        parts.push_back(item);
    }
    
    if (parts.empty()) return "ERROR|Invalid request";
    
    std::string cmd = parts[0];
    
    if (cmd == "REGISTER" && parts.size() == 3) {
        if (registerUser(parts[1], parts[2])) {
            return "OK|Registered";
        } else {
            return "ERROR|User exists";
        }
    }
    else if (cmd == "LOGIN" && parts.size() == 3) {
        int userId = loginUser(parts[1], parts[2]);
        if (userId > 0) {
            return "OK|" + std::to_string(userId);
        } else {
            return "ERROR|Invalid credentials";
        }
    }
    else if (cmd == "ENCRYPT" && parts.size() == 3) {
        return "OK|" + encryptVigenere(parts[1], parts[2]);
    }
    else if (cmd == "DECRYPT" && parts.size() == 3) {
        return "OK|" + decryptVigenere(parts[1], parts[2]);
    }
    
    return "ERROR|Unknown command";
}
