#include "InputInterpreter.h"

InputInterpreter::InputInterpreter(TCPSocket connectSocket)
{
    _connectSocket = connectSocket;
    commands.insert({ L"HELP", &InputInterpreter::help });
    commands.insert({ L"QUIT", &InputInterpreter::quit });
    commands.insert({ L"TEST", &InputInterpreter::test });
}

void InputInterpreter::start()
{
    std::wstring userInput;
    std::wstring serverOutput;
    std::wcout << L"Enter \"HELP\" to see all available commands\n";
    while (true)
    {
        std::wcout << L"~>";
        std::getline(std::wcin, userInput);
        std::vector<std::wstring> tokens = tokenizeInput(userInput);
        if (tokens.size() == 0)
            continue;
        std::wstring command = tokens[0];
        if (commands.contains(command))
        {
            tokens.erase(tokens.begin());
            (this->*commands[command])(tokens);
        }
        else
        {
            _connectSocket.send(userInput);
            _connectSocket.recive(serverOutput);
            std::wcout << serverOutput << std::endl;
        }
    }
}

void InputInterpreter::help(std::vector<std::wstring> arguments)
{
    if (!arguments.empty())
    {
        std::wcout << L"ERROR: HELP doesn`t take any arguments" << std::endl;
    }
    else
    {
        std::wstring helpMessage;
        helpMessage += L"Client side commands:\nHELP - show this message\n";
        helpMessage += L"TEST [number of changes] - automaticaly test server:";
        helpMessage += L"change each variable value[number of changes] times and show statistics\n";
        helpMessage += L"QUIT - finish working\n";
        helpMessage += L"Server side commands:\nWho - show information about the author\n";
        helpMessage += L"SET [variable number(0-9)] (TRUE|FALSE) - try to change variables value";
        helpMessage += L"(return SUCCESS or FAIL)\n";
        helpMessage += L"Remark: Anything other than client commands will be interpreted as a message for the server!";
        std::wcout << helpMessage << std::endl;
    }
}

void  InputInterpreter::test(std::vector<std::wstring> arguments)
{
    if (arguments.size() != 1)
        std::wcout << L"ERROR: TEST: Wrong number of arguments" << std::endl;
    else
    {
        std::wstringstream ss;
        ss << arguments[0];

        unsigned int numberOfChanges;
        ss >> numberOfChanges;
        if (ss.fail())
        {
            std::wcout << L"ERROR: TEST: Wrong first argument" << std::endl;
        }
        else
            tester.test(_connectSocket, numberOfChanges);
    }
}

void InputInterpreter::quit(std::vector<std::wstring> arguments)
{
    if (!arguments.empty())
    {
        std::wcout << L"ERROR: QUIT doesn`t take any arguments" << std::endl;
    }
    else
    {
        WSACleanup();
        std::exit(0);
    }
}


