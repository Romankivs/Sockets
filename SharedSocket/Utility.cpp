#include "Utility.h"

std::vector<std::wstring> tokenizeInput(std::wstring input)
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