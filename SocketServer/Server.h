#pragma once
#include "SharedVariablesDispatcher.h"
#include "TCPSocket.h"

constexpr int MAX_CLIENTS = 1;
constexpr int DEFAULT_BUFFER_SIZE = 256;
constexpr PCSTR DEFAULT_PORT = "1049";

#define DEFAULT_LOG_FILE_PATH L"ServerLogs.txt"

class Server
{
public:
	void start();
};

