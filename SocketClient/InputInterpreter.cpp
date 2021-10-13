#include "InputInterpreter.h"

void InputInterpreter::start(TCPSocket connectSocket)
{
	VarDispatcherTester tester;
    std::wstring userInput;
    std::wstring serverOutput;
    while (true)
    {
        std::wcout << "~>";
        std::getline(std::wcin, userInput);
        std::vector<std::wstring> tokens = tokenizeInput(userInput);
        if (tokens.size() == 1 && tokens[0] == L"QUIT")
            break;
        else if (tokens.size() == 2 && tokens[0] == L"TEST")
        {
            std::wstringstream ss;
            ss << tokens[1];

            unsigned int numberOfChanges;
            ss >> numberOfChanges;
            if (ss.fail())
            {
                std::wcout << L"ERROR: TEST: Wrong first argument" << std::endl;
            }
            else
                tester.test(connectSocket, numberOfChanges);
        }
        else
        {
            connectSocket.send(userInput);
            connectSocket.recive(serverOutput);
            std::wcout << serverOutput << std::endl;
        }
    }
}
