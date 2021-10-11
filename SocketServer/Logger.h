#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <cassert>

class Logger
{
public:
	Logger(std::wstring outputFileName);
	~Logger();
	void recv(std::wstring command);
	void send(std::wstring command);
private:
	void log(std::wstring message);
	std::wofstream outputFile;
};

