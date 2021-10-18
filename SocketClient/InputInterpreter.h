#pragma once
#include "Utility.h"
#include "VarDispatcherTester.h"
#include <unordered_map>
#include <iostream>

class InputInterpreter
{
public:
	InputInterpreter(TCPSocket connectSocket);
	void start();
private:
	void help(std::vector<std::wstring> arguments);
	void test(std::vector<std::wstring> arguments);
	void quit(std::vector<std::wstring> arguments);

	std::unordered_map<std::wstring, void(InputInterpreter::*)(std::vector<std::wstring>)> commands;
	TCPSocket _connectSocket = TCPSocket(INVALID_SOCKET, nullptr);
	VarDispatcherTester tester;
};

