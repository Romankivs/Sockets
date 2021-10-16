#include "Client.h"

void Client::start()
{
    Logger socketInfoLogger(DEFAULT_LOG_FILE_PATH);

    // Init WinSock
    initializeWSA();

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* result = NULL;

    // Resolve the server address and port
    getAdressInfo("localhost", DEFAULT_PORT, &hints, &result);

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
        errorWSACleanup();
    }

    InputInterpreter interpreter(connectSocket);
    interpreter.start();

    system("pause");

    // cleanup
    connectSocket.close();
    WSACleanup();
}
