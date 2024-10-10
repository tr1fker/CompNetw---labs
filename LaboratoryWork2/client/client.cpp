#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main(){
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData)){
		cout << "Error connecting the winsock2 library";
		return 1;
	}
	cout << "wVersionRequested = " << wVersionRequested << endl
		<< "Fields of struct wsaData: " << wsaData.wVersion << ' ' << wsaData.wHighVersion
		<< ' ' << wsaData.iMaxSockets << ' ' << wsaData.iMaxUdpDg << endl << endl;
	SOCKET socket_connection = socket(PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(1024);
	inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);
	int size_address = sizeof(address);
	cout << "server_port = " << ntohs(address.sin_port) << endl;
	char address_line[50];
	inet_ntop(AF_INET, &(address.sin_addr), address_line, 49);
	cout << "server_IP-addr = " << address_line << endl << endl;
	char buf[255], result[265];
	while (true){
		cout << "Enter the line of symbols:" << endl;
		cin.getline(buf, 255, '\n');
		if (!strlen(buf)) break;
		sendto(socket_connection, buf, strlen(buf) + 1, 0, (struct sockaddr*)&address, size_address);
		recvfrom(socket_connection, result, sizeof(result), 0, (struct sockaddr *)&address, &size_address);
		cout << endl << "Answer: " << result << endl << endl;
	}
	closesocket(socket_connection);
	WSACleanup();
	return 0;
}
