#pragma once
#include <array>
#include <random>
#include <functional>
#include <string>

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

	// return info about the author
	std::wstring who();

	// randomly check whether the variable can be changed and return true if succeeded or false if failed.
	bool setVariableValue(size_t varNumber, bool value);

	bool getVariableValue(size_t varNumber);
private:
	std::array<Variable, NUMBER_OF_VARIABLES> variables;
	std::function<int(void)> getRand;
};

