#pragma once
#include <array>
#include <vector>
#include <random>
#include <functional>
#include <unordered_map>
#include <string>
#include <chrono>
#include <sstream>

struct Variable
{
	bool value;
	unsigned int accessCounter;
};

constexpr size_t NUMBER_OF_VARIABLES = 10;

class SharedVariablesDispatcher
{
public:
	SharedVariablesDispatcher();
	// perform commands according to clients input and return command output or error message
	std::wstring analyzeClientsInput(std::wstring input);

	// split input into tokens
	std::vector<std::wstring> tokenizeInput(std::wstring input);

	// return info about the author
	std::wstring who(std::vector<std::wstring> arguments);

	// try to set variable and return L"SUCCESS" if done succesfully and L"FAILURE" if failed
	std::wstring set(std::vector<std::wstring> arguments);

	// randomly check whether the variable can be changed and return true if succeeded or false if failed.
	bool setVariableValue(size_t varNumber, bool value);

	bool getVariableValue(size_t varNumber);
private:
	std::array<Variable, NUMBER_OF_VARIABLES> variables;
	std::unordered_map<std::wstring, std::wstring(SharedVariablesDispatcher::*)(std::vector<std::wstring>)> commands;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
	std::function<int(void)> getRand;
};

