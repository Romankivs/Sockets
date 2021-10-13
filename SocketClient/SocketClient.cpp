#include "TCPSocket.h"
#include "InputInterpreter.h"

#pragma comment (lib, "Ws2_32.lib") // link Ws2_32.lib

constexpr int DEFAULT_BUFFER_SIZE = 256;
constexpr PCSTR DEFAULT_PORT = "1049";

#define DEFAULT_LOG_FILE_PATH L"ClientLogs.txt"

int main()
{
    Logger socketInfoLogger(DEFAULT_LOG_FILE_PATH);

    setlocale(LC_ALL, "Ukrainian");
    SetConsoleOutputCP(1251);

    // Init WinSock
    initializeWSA();

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* result = NULL;

    // Resolve the server address and port
    const int getAddrInfoRes = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (getAddrInfoRes != 0) {
        std::cout << "getaddrinfo failed with error: " << getAddrInfoRes << std::endl;;
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    TCPSocket connectSocket(INVALID_SOCKET, &socketInfoLogger);
    for (struct addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        connectSocket = TCPSocket(ptr, &socketInfoLogger);

        // Connect to server.
        int connectRes = connectSocket.tryToConnect(ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
        if (connectRes == SOCKET_ERROR) {
            connectSocket.close();
            connectSocket.setUnderlyingSocket(INVALID_SOCKET);
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (connectSocket.getUnderlyingSocket() == INVALID_SOCKET) {
        std::cout << "Failed to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }

    InputInterpreter interpreter;
    interpreter.start(connectSocket);

    system("pause");
    
    // cleanup
    connectSocket.close();
    WSACleanup();
}
