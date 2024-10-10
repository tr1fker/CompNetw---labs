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
    cout << "wVersionRequested = " << wVersionRequested << endl;
    cout << "Fields of struct wsaData: " << wsaData.wVersion << ' ' << wsaData.wHighVersion
        << ' ' << wsaData.iMaxSockets << ' ' << wsaData.iMaxUdpDg << endl << endl;
    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(1280);
    InetPton(AF_INET, _T("127.0.0.1"), &peer.sin_addr.s_addr);
    SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
    char buf[255], b[255];
    connect(s, (struct sockaddr*)&peer, sizeof(peer));
    while (true){
        cout << "Введите числа: " << endl;
        cin.getline(buf, 255, '\n');
        if (!strlen(buf)) break;
        send(s, buf, sizeof(buf), 0);
        if (recv(s, b, sizeof(b), 0) != 0){
            cout << endl << "Кол-во чисел кратных 3:  " << b << endl << endl;
        }
        cin.clear();
    }
    closesocket(s);
    WSACleanup();
    return 0;
}