#pragma once
#include "TCPSocket.h"
#include "InputInterpreter.h"

constexpr int DEFAULT_BUFFER_SIZE = 256;
constexpr PCSTR DEFAULT_PORT = "1049";

#define DEFAULT_LOG_FILE_PATH L"ClientLogs.txt"

class Client
{
public:
	void start();
};

