#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
int main(void){
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0) { return -1; }
	while (true) {
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(1280);
		inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr.s_addr);
		connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));
		char buf[100];
		cout << "Vvedite stroky(Pystaia stroke - vihod):";
		cin.getline(buf, 100, '\n');
		if (!strlen(buf)) break;
		send(s, buf, strlen(buf) + 1, 0);
		if (recv(s, buf, sizeof(buf), 0) != 0) {
			cout << "Poluchenaya stroka:" << buf << endl;
		}
		closesocket(s);
	}
	WSACleanup();
	return 0;
}
