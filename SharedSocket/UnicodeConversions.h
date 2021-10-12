#pragma once
#include <string>
#include <cassert>
#include <Windows.h>

std::wstring stringToWideString(const std::string& string);

std::string wideStringToString(const std::wstring& wide_string);