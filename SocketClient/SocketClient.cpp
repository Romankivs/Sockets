#include "Client.h"

#pragma comment (lib, "Ws2_32.lib") // link Ws2_32.lib

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Client client;
    client.start();
}
