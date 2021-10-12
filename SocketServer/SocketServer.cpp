#include <WS2tcpip.h> 
#include <iostream>
#include <string>
#include <locale>
#include "SharedVariablesDispatcher.h"
#include "Logger.h"
#include "TCPSocket.h"

#pragma comment (lib, "Ws2_32.lib") // link Ws2_32.lib

constexpr int MAX_CLIENTS = 1;
constexpr int DEFAULT_BUFFER_SIZE = 256;
constexpr PCSTR DEFAULT_PORT = "1049";

#define DEFAULT_LOG_FILE_PATH L"ServerLogs.txt"

int main()
{
    Logger socketInfoLogger(DEFAULT_LOG_FILE_PATH);

    setlocale(LC_ALL, "Ukrainian");
    SetConsoleOutputCP(1251);

    SharedVariablesDispatcher disp;

    // Init WinSock
    WSAData wsData;
    WORD wsVer = MAKEWORD(2,2);

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
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *result = NULL;

    // Resolve the server address and port
    const int getAddrInfoRes = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (getAddrInfoRes!= 0) {
        std::cout << "getaddrinfo failed with error: " << initResult << std::endl;;
        WSACleanup();
        return 1;
    }

    // Create a listening socket

    TCPSocket listeningSocket(result, &socketInfoLogger);

    // Bind the listening socket to an adress

    listeningSocket.bindSocket(result->ai_addr, static_cast<int>(result->ai_addrlen));

    freeaddrinfo(result);

    listeningSocket.listenSocket(MAX_CLIENTS);

    // Accept a client
    
    TCPSocket clientSocket = listeningSocket.acceptSocket(NULL, NULL);

    // No other clients will be accepted - closing the listening socket
    
    listeningSocket.closeSocket();

    // Work until the peer shuts down the connection
    int bytesReceived;
    do {
        std::wstring reciveBuf;
        bytesReceived = clientSocket.reciveSocket(reciveBuf);
        if (bytesReceived > 0) {
            std::cout << "Bytes received: " << bytesReceived << std::endl;

            std::wcout << reciveBuf << std::endl;

            std::wstring result = disp.analyzeClientsInput(reciveBuf);
            std::wcout << result << result.size() * sizeof(wchar_t) << std::endl;
            int iSendResult = clientSocket.sendSocket(result);
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (bytesReceived == 0)
            std::cout << "Connection closing..." << std::endl;
    } while (bytesReceived > 0);

    system("pause");

    // cleanup

    clientSocket.closeSocket();
    WSACleanup();
}