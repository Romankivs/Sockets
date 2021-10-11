#include "Logger.h"

Logger::Logger(std::string outputFileName)
{
	outputFile.open(outputFileName, std::ios::out | std::ios::trunc);
	assert(outputFile.is_open() && "Unable to open a file!");
}

Logger::~Logger()
{
	outputFile.close();
}

void Logger::recv(std::string command)
{
	log("recv: " + command);
}

void Logger::send(std::string command)
{
	log("send: " + command);
}

void Logger::log(std::string message)
{
	const time_t now = std::time(nullptr);
	std::tm tm = {};
	localtime_s(&tm, &now);
	outputFile << std::put_time(&tm, "%Y-%m-%d %X") << " : " << message << '\n';
}
