#include "SharedVariablesDispatcher.h"
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
    initializeWSA();

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
        std::cout << "getaddrinfo failed with error: " << getAddrInfoRes << std::endl;;
        WSACleanup();
        return 1;
    }

    // Create a listening socket

    TCPSocket listeningSocket(result, &socketInfoLogger);

    // Bind the listening socket to an adress

    listeningSocket.bind(result->ai_addr, static_cast<int>(result->ai_addrlen));

    freeaddrinfo(result);

    listeningSocket.listen(MAX_CLIENTS);

    // Accept a client
    
    TCPSocket clientSocket = listeningSocket.accept(NULL, NULL);

    // No other clients will be accepted - closing the listening socket
    
    listeningSocket.close();

    // Work until the peer shuts down the connection
    int bytesReceived;
    do {
        std::wstring reciveBuf;
        bytesReceived = clientSocket.recive(reciveBuf);
        if (bytesReceived > 0) {
            std::cout << "Bytes received: " << bytesReceived << std::endl;

            std::wcout << reciveBuf << std::endl;

            std::wstring result = disp.analyzeClientsInput(reciveBuf);
            std::wcout << result << std::endl;
            int iSendResult = clientSocket.send(result);
            std::cout << "Bytes sent: " << iSendResult << std::endl;
        }
        else 
            std::cout << "Connection closing..." << std::endl;
    } while (bytesReceived > 0);

    system("pause");

    // cleanup

    clientSocket.close();
    WSACleanup();
}