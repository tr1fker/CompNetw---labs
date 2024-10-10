#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    SOCKET socket1 = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1280);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int connection = bind(socket1, (struct sockaddr*)&server_addr, sizeof(server_addr));
    int status_listening = listen(socket1, 5);
    cout << "Server is running" << endl;
    char res[255], pocket[255];
    struct sockaddr_in remote_addr;
    int size = sizeof(remote_addr);
    char addr[50];
    while (true) {
        SOCKET newSocketConnection = accept(socket1, (struct sockaddr*)&remote_addr, &size);
        inet_ntop(AF_INET, &(remote_addr.sin_addr), addr, 49);
        cout << "--------------------------------------------------------" << endl;
        cout << "client_port = " << ntohs(remote_addr.sin_port) << endl;
        cout << "client_IP-addr = " << addr << endl << endl;
        while (recv(newSocketConnection, pocket, sizeof(pocket), 0) != 0){
            cout << "Введите числа:  " << pocket << endl;
            int lengthLine = strlen(pocket) + 1, counter = 0;
            for (int letter = 0, number = 0; letter < lengthLine; letter++) {
                if (pocket[letter] >= '0' && pocket[letter] <= '9')
                    number = number * 10 + pocket[letter] - '0';
                else if (number != 0 && number % 3 == 0)
                    counter++;
                else number = 0;
            }
            _itoa_s(counter, res, 10);
            send(newSocketConnection, res, sizeof(res), 0);
            cout << "Server return:  " << res << endl << endl;
        }
        closesocket(newSocketConnection);
    }
    WSACleanup();
    return 0;
}



