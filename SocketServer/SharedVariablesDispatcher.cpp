#include "SharedVariablesDispatcher.h"

SharedVariablesDispatcher::SharedVariablesDispatcher()
{
	variables = {};

	commands.insert({ L"Who", &SharedVariablesDispatcher::who });
	commands.insert({ L"SET", &SharedVariablesDispatcher::set });

	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(-1, 1);
	getRand = []() { return distribution(generator); };
}

std::wstring SharedVariablesDispatcher::analyzeClientsInput(std::wstring input)
{
	std::vector<std::wstring> tokens = tokenizeInput(input);
	if (tokens.empty())
		return L"ERROR: Empty input";
	std::wstring command = tokens[0];
	if (!commands.contains(command))
		return L"ERROR: Wrong command";
	tokens.erase(tokens.begin());
	std::wstring result = (this->*commands[command])(tokens);
	return result;
}

std::vector<std::wstring> SharedVariablesDispatcher::tokenizeInput(std::wstring input)
{
	std::vector<std::wstring> result;
	wchar_t* buffer = nullptr;
	wchar_t* token = wcstok_s(input.data(), L" ", &buffer);
	while (token != NULL)
	{
		result.push_back(std::wstring(token));
		token = wcstok_s(NULL, L" ", &buffer);
	}
	return result;
}

std::wstring SharedVariablesDispatcher::who(std::vector<std::wstring> arguments)
{
	if (!arguments.empty())
	{
		return L"ERROR: Who doesn`t take any arguments";
	}
	return L"Романків Святослав К-25. 24 варіант( Диспетчер розділяємих змінних)";
}

std::wstring SharedVariablesDispatcher::set(std::vector<std::wstring> arguments)
{
	if (arguments.size() != 2)
		return L"ERROR: SET: Wrong number of arguments";
	std::wstringstream ss;
	ss << arguments[0];

	size_t varNumber;
	ss >> varNumber;
	if (ss.fail() || varNumber < 0 || varNumber > 9)
	{
		return L"ERROR: SET: Wrong first argument";
	}

	bool setResult;
	if (arguments[1] == L"TRUE")
	{
		setResult = setVariableValue(varNumber, true);
	}
	else if (arguments[1] == L"FALSE")
	{
		setResult = setVariableValue(varNumber, false);
	}
	else
	{
		return L"ERROR: SET: Wrong second argument";
	}
	if (setResult)
		return L"SUCCESS";
	return L"FAIL";
}

bool SharedVariablesDispatcher::setVariableValue(size_t varNumber, bool value)
{
	if (getRand() >= 0) // success
	{
		variables[varNumber].value = value;
		return true;
	}
	else
	{
		return false;
	}
}

bool SharedVariablesDispatcher::getVariableValue(size_t varNumber)
{
	return variables[varNumber].value;
}
