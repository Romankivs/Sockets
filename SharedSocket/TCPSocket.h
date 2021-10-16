#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <Windows.h>
#include "UnicodeConversions.h"
#include "Logger.h"

#define DEFAULT_SOCKET_BUFFER_SIZE 256

void initializeWSA();
void errorWSACleanup();
int getAdressInfo(PCSTR name, PCSTR port, const ADDRINFO* hints, PADDRINFOA* result);

class TCPSocket
{
public:
	TCPSocket(struct addrinfo* adressInfo, Logger* logger);
	TCPSocket(SOCKET otherSocket, Logger* logger);
	void bind(const sockaddr* addr, int namelen);
	int tryToConnect(const sockaddr* name, int namelen);
	void listen(int maxClients);
	TCPSocket accept(sockaddr* addr, int* addrlen);
	void shutdown(int how);
	void close();
	int recive(std::wstring& recivebuf);
	int send(std::wstring sendbuf);
	SOCKET getUnderlyingSocket();
	void setUnderlyingSocket(SOCKET sock);
private:
	void cleanup(const std::string& errorMessage);
	SOCKET _socket;
	char socketBuffer[DEFAULT_SOCKET_BUFFER_SIZE] = {};
	Logger* log;
};

