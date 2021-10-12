#include "TCPSocket.h"

TCPSocket::TCPSocket(addrinfo* adressInfo)
{
    _socket = socket(adressInfo->ai_family, adressInfo->ai_socktype, adressInfo->ai_protocol);
    if (_socket == INVALID_SOCKET)
    {
        std::cout << "Creating listening socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        std::terminate();
    }
}

void TCPSocket::bindSocket(const sockaddr* addr, int namelen)
{
    const int bindRes = bind(_socket, addr, namelen);
    if (bindRes == SOCKET_ERROR)
    {
        cleanup("bind failed with error: " + WSAGetLastError());
    }
}

void TCPSocket::listenSocket(int maxClients)
{
    const int listenRes = listen(_socket, maxClients);
    if (listenRes == SOCKET_ERROR)
    {
        cleanup("listen failed with error: " + WSAGetLastError());
    }
}

void TCPSocket::acceptSocket(sockaddr* addr, int* addrlen)
{
    SOCKET clientSocket = accept(_socket, addr, addrlen);
    if (clientSocket == INVALID_SOCKET) {
        cleanup("accept failed with error: " + WSAGetLastError());
    }
}

void TCPSocket::closeSocket()
{
    closesocket(_socket);
}

int TCPSocket::reciveSocket(std::wstring& recivebuf)
{
    int bytesReceived = recv(_socket, socketBuffer, DEFAULT_SOCKET_BUFFER_SIZE, 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        cleanup("recv failed with error: " + WSAGetLastError());
    }
    recivebuf = stringToWideString(std::string(socketBuffer));
    return bytesReceived;
}

int TCPSocket::sendSocket(std::wstring sendbuf)
{
    std::string sendbufConverted = wideStringToString(sendbuf);
    int bytesSent = send(_socket, sendbufConverted.c_str(), sendbufConverted.size() + 1, 0);
    if (bytesSent == SOCKET_ERROR) {
        cleanup("send failed with error: " + WSAGetLastError());
    }
}

void TCPSocket::cleanup(const std::string& errorMessage)
{
    std::cout << errorMessage << std::endl;
    closesocket(_socket);
    WSACleanup();
    std::terminate();
}
