#include "Logger.h"

Logger::Logger(std::wstring outputFileName)
{
	outputFile.open(outputFileName, std::ios::out | std::ios::trunc | std::ios::binary);
	assert(outputFile.is_open() && "Unable to open a file!");
}

Logger::~Logger()
{
	outputFile.close();
}

void Logger::recv(std::wstring command)
{
	log(L"recv: " + command);
}

void Logger::send(std::wstring command)
{
	log(L"send: " + command);
}

void Logger::log(std::wstring message)
{
	std::string converted = wideStringToString(message);
	const time_t now = std::time(nullptr);
	std::tm tm = {};
	localtime_s(&tm, &now);
	outputFile << std::put_time(&tm, "%Y-%m-%d %X") << " : " << converted << '\n';
}
#include "Logger.h"
