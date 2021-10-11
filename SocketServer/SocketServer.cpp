#include <WS2tcpip.h> 
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include "UnicodeConversions.h"
#include "SharedVariablesDispatcher.h"
#include "Logger.h"

#pragma comment (lib, "Ws2_32.lib") // link Ws2_32.lib

constexpr int MAX_CLIENTS = 1;
constexpr int DEFAULT_BUFFER_SIZE = 256;
constexpr PCSTR DEFAULT_PORT = "1049";

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    SetConsoleOutputCP(1251);

    /*std::wstring cum = L"РоманківfСвя";
    std::string cumt = wide_string_to_string(cum);
    std::wcout << cum.size() * sizeof(wchar_t) << std::endl;
    std::cout << cumt << cumt.size() * sizeof(char) << std::endl;
    std::wstring cumtt = string_to_wide_string(cumt);
    std::wcout << cumtt << cumtt.size() * sizeof(wchar_t) << std::endl;*/

    Logger logger(L"1.txt");
    logger.recv(L"cum");
    logger.send(L"Fff");
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
    SOCKET listeningSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listeningSocket == INVALID_SOCKET)
    {
        std::cout << "Creating listening socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Bind the listening socket to an adress
    const int bindRes = bind(listeningSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (bindRes == SOCKET_ERROR)
    {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    const int listenRes = listen(listeningSocket, MAX_CLIENTS);
    if (listenRes == SOCKET_ERROR)
    {
        std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client
    SOCKET clientSocket = accept(listeningSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "accepting a client failed with error: " <<  WSAGetLastError() << std::endl;
        closesocket(listeningSocket);
        WSACleanup();
        return 1;
    }

    // No other clients will be accepted - closing the listening socket
    closesocket(listeningSocket);

    // Receive until the peer shuts down the connection
    int bytesReceived;
    char recvbuf[DEFAULT_BUFFER_SIZE];
    const int recvbuflen = DEFAULT_BUFFER_SIZE;
    do {

        bytesReceived = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (bytesReceived > 0) {
            std::cout << "Bytes received: " << bytesReceived << std::endl;

            std::cout << recvbuf << std::endl;

            // ??????????????????????????????????
            // Echo the buffer back to the sender
            std::wstring result = disp.analyzeClientsInput(stringToWideString(std::string(recvbuf)));
            std::wcout << result << result.size() * sizeof(wchar_t) << std::endl;
            std::string resstr = wideStringToString(result);
            int iSendResult = send(clientSocket, resstr.data(), wideStringToString(result).size() + 1, 0);
            if (iSendResult == SOCKET_ERROR) {
                std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (bytesReceived == 0)
            std::cout << "Connection closing..." << std::endl;
        else {
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

    } while (bytesReceived > 0);

    // shutdown the connection 
    const int shutdownRes = shutdown(clientSocket, SD_SEND);
    if (shutdownRes == SOCKET_ERROR) {
        std::cout << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    system("pause");

    // cleanup
    closesocket(clientSocket);
    WSACleanup();
}