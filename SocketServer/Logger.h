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
	Logger(std::string outputFileName);
	~Logger();
	void recv(std::string command);
	void send(std::string command);
private:
	void log(std::string message);
	std::ofstream outputFile;
};

