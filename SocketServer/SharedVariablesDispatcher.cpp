#include "SharedVariablesDispatcher.h"

SharedVariablesDispatcher::SharedVariablesDispatcher()
{
	variables = {};
	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(-1, 1);
	getRand = []() { return distribution(generator); };
}

std::string SharedVariablesDispatcher::analyzeClientsInput(std::string input)
{
	return std::string();
}

std::wstring SharedVariablesDispatcher::who()
{
	return L"Романків Святослав К-25. 24 варіант( Диспетчер розділяємих змінних)";
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
