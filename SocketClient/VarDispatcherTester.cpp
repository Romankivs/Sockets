#include "VarDispatcherTester.h"

VarDispatcherTester::VarDispatcherTester()
{
    numberOfAttemptsForEachVar = std::vector<int>(10);

    generator = std::default_random_engine();
    generator.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count());
    distribution = std::uniform_int_distribution<int>(0, 1);
    getRand = [&]() { return distribution(generator); };
}

void VarDispatcherTester::test(TCPSocket dispatcherSocket, int numberOfChanges)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    int numberOfChangesLeft = numberOfChanges;
    while (numberOfChangesLeft > 0)
    {
        for (int var = 0; var < 10; ++var)
        {
            std::wstring value = [this] {if (getRand()) return L" TRUE"; else return L" FALSE"; }(); // pick random value for var
            std::wstring sendstr = L"SET " + std::to_wstring(var) + value;
            std::wstring dispatcherResponse;
            bool success = false;
            while (!success)
            {
                dispatcherSocket.send(sendstr);
                dispatcherSocket.recive(dispatcherResponse);
                if (dispatcherResponse == L"SUCCESS")
                    success = true;
                else if (dispatcherResponse != L"FAIL") // ERROR message
                {
                    std::wcout << dispatcherResponse << std::endl;
                    errorWSACleanup();
                }
                ++numberOfAttemptsForEachVar[var];
            }
        }
        --numberOfChangesLeft;
    }

	auto endTime = std::chrono::high_resolution_clock::now();
    
    std::wcout << getTimeInfo(startTime, endTime);
    std::wcout << getAttemptsInfo(numberOfChanges);
}

std::wstring VarDispatcherTester::getTimeInfo(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end)
{
    auto timeTaken = end - start;
    auto timeTakenInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken);
    return L"Elapsed time in milliseconds: " + std::to_wstring(timeTakenInMilliseconds.count()) + L" ms\n";
}

std::wstring VarDispatcherTester::getAttemptsInfo(int numberOfChanges)
{
    std::wstring result = L"Number of attempts made to succesfully change each variable " 
        + std::to_wstring(numberOfChanges) +  L" times:\n";
    for (size_t i = 0; i < 10; ++i)
    {
        result += L"Var " + std::to_wstring(i) + L" - " + std::to_wstring(numberOfAttemptsForEachVar[i]) + L" attempts\n";
    }
    return result;
}
