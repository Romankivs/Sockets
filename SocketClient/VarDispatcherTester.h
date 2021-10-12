#pragma once
#include "TCPSocket.h"
#include <random>
#include <functional>

class VarDispatcherTester
{
public:
	VarDispatcherTester();
	void test(TCPSocket dispatcherSocket, int numberOfChanges);
private:
	std::string getTimeInfo(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end);
	std::string getAttemptsInfo(int numberOfChanges);
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
	std::function<int(void)> getRand;

	std::vector<int> numberOfAttemptsForEachVar;
};

