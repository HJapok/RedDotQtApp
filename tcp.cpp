#include "tcp.h"
#include <iostream>

// ---------- TCP SERVER ----------
TCPServer::TCPServer(const std::string& ip, int port) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    bind(serverSocket, (sockaddr*)&addr, sizeof(addr));
    listen(serverSocket, 1);
}

TCPServer::~TCPServer() {
    closesocket(serverSocket);
    WSACleanup();
}

bool TCPServer::waitForTrigger(std::string& message) {
    sockaddr_in client{};
    int size = sizeof(client);

    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&client, &size);
    if (clientSocket == INVALID_SOCKET)
        return false;

    char buffer[256]{};
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    closesocket(clientSocket);

    if (bytes > 0) {
        message.assign(buffer, bytes);
        return true;
    }
    return false;
}

// ---------- TCP CLIENT ----------
TCPClient::TCPClient(const std::string& ip, int port) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    connect(clientSocket, (sockaddr*)&addr, sizeof(addr));
}

TCPClient::~TCPClient() {
    closesocket(clientSocket);
    WSACleanup();
}

bool TCPClient::sendResult(const VisionResult& result) {
    return send(clientSocket,
                reinterpret_cast<const char*>(&result),
                sizeof(result),
                0) > 0;
}
