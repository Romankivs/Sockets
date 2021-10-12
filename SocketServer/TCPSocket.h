#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include "UnicodeConversions.h"

#define DEFAULT_SOCKET_BUFFER_SIZE 256

class TCPSocket
{
public:
	TCPSocket(struct addrinfo* adressInfo);
	void bindSocket(const sockaddr* addr, int namelen);
	void listenSocket(int maxClients);
	void acceptSocket(sockaddr* addr, int* addrlen);
	void closeSocket();
	int reciveSocket(std::wstring& recivebuf);
	int sendSocket(std::wstring sendbuf);
private:
	void cleanup(const std::string& errorMessage);
	SOCKET _socket;
	char* socketBuffer;
};

