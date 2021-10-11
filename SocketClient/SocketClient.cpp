#include <WS2tcpip.h> 
#include <iostream>
#include <string>
#include "../SocketServer/Logger.h"

#pragma comment (lib, "Ws2_32.lib") // link Ws2_32.lib

constexpr int DEFAULT_BUFFER_SIZE = 256;
constexpr PCSTR DEFAULT_PORT = "1049";

int main()
{
    // Init WinSock
    WSAData wsData;
    WORD wsVer = MAKEWORD(2, 2);

    int initResult = WSAStartup(wsVer, &wsData);
    if (initResult != 0)
    {
        std::cout << "WSAStartup failed with error: " << initResult << std::endl;
        return 1;
    }

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* result = NULL;

    // Resolve the server address and port
    const int getAddrInfoRes = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (getAddrInfoRes != 0) {
        std::cout << "getaddrinfo failed with error: " << initResult << std::endl;;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    SOCKET connectSocket = INVALID_SOCKET;
    for (struct addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        // Connect to server.
        int connectRes = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (connectRes == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET) {
        std::cout << "Failed to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    char sendbuf[DEFAULT_BUFFER_SIZE] = "ABOBA";
    const int sendRes = send(connectSocket, sendbuf, DEFAULT_BUFFER_SIZE, 0);
    if (sendRes == SOCKET_ERROR) {
        std::cout << "send failed with error: " <<  WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", sendRes);

    // shutdown the connection since no more data will be sent
    const int shutdownRes = shutdown(connectSocket, SD_SEND);
    if (shutdownRes == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " <<  WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    int bytesReceived;
    char recvbuf[DEFAULT_BUFFER_SIZE];
    const int recvbuflen = DEFAULT_BUFFER_SIZE;
    do {
        bytesReceived = recv(connectSocket, recvbuf, recvbuflen, 0);
        if (bytesReceived > 0)
        {
            std::cout << "Bytes received: " << bytesReceived << std::endl;
            std::cout << recvbuf << std::endl;
        }
        else if (bytesReceived == 0)
            std::cout << "Connection closed" << std::endl;
        else
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;

    } while (bytesReceived > 0);

    system("pause");
    
    // cleanup
    closesocket(connectSocket);
    WSACleanup();
}