#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <Windows.h>
#include "UnicodeConversions.h"
#include "Logger.h"

#define DEFAULT_SOCKET_BUFFER_SIZE 256

class TCPSocket
{
public:
	TCPSocket(struct addrinfo* adressInfo, Logger* logger);
	TCPSocket(SOCKET otherSocket, Logger* logger);
	void bindSocket(const sockaddr* addr, int namelen);
	void listenSocket(int maxClients);
	TCPSocket acceptSocket(sockaddr* addr, int* addrlen);
	void shutdownSocket(int how);
	void closeSocket();
	int reciveSocket(std::wstring& recivebuf);
	int sendSocket(std::wstring sendbuf);
private:
	void cleanup(const std::string& errorMessage);
	SOCKET _socket;
	char socketBuffer[DEFAULT_SOCKET_BUFFER_SIZE];
	Logger* log;
};

