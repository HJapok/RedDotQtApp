#ifndef TCP_H
#define TCP_H

#include <winsock2.h>
#include <string>

// Link WinSock library
#pragma comment(lib, "ws2_32.lib")

// Struct sent over TCP
struct VisionResult {
    int id;
    int x;
    int y;
};

// TCP SERVER: receives trigger
class TCPServer {
public:
    TCPServer(const std::string& ip, int port);
    ~TCPServer();

    bool waitForTrigger(std::string& message);

private:
    SOCKET serverSocket;
};

// TCP CLIENT: sends vision result
class TCPClient {
public:
    TCPClient(const std::string& ip, int port);
    ~TCPClient();

    bool sendResult(const VisionResult& result);

private:
    SOCKET clientSocket;
};

#endif
