#pragma once
#include <string>
#include <cassert>
#include <Windows.h>

std::wstring stringToWideString(const std::string& string)
{
    if (string.empty())
    {
        return L"";
    }
    const auto size_needed = MultiByteToWideChar(CP_UTF8, 0, &string.at(0), (int)string.size(), nullptr, 0);
    assert(size_needed > 0 && "MultiByteToWideChar failed");
    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &string.at(0), (int)string.size(), &result.at(0), size_needed);
    return result;
}

std::string wideStringToString(const std::wstring& wide_string)
{
    if (wide_string.empty())
    {
        return "";
    }
    const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), nullptr, 0, nullptr, nullptr);
    assert(size_needed > 0 && "WideCharToMultiByte failed");
    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), &result.at(0), size_needed, nullptr, nullptr);
    return result;
}
