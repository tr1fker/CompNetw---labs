#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
struct LDTC {
	char dateConversation[15], nameCity[15], tariff[15];
	int code, callDuration, number, numberSub;
};
LDTC arr[5] = {
	{"10.10.2024", "Minsk", "Bezlimitishe", 33, 678, 803566320, 33},
	{"10.10.2024", "Brest", "VseVkl", 29, 123, 801536320, 29},
	{"09.10.2024", "Minsk", "Interesno", 22, 430, 803234120, 22},
	{"09.09.2024", "Brest", "Inet", 55, 1423, 804263221, 55},
	{"08.09.2023", "Gantsevichi", "Velcom", 29, 1253, 802222222, 29}
};
DWORD WINAPI ThreadFunc(LPVOID client_socket){
	SOCKET s2 = ((SOCKET*)client_socket)[0];
	char buf[100];
	while (recv(s2, buf, sizeof(buf), 0)){
		int counter = 0, length_buf = strlen(buf);
		for (int _city = 0; _city < 5; _city++) {
			int length_city = strlen(arr[_city].nameCity);
			if (length_buf != length_city)
				continue;
			for (int _letter = 0; _letter < length_city; _letter++)
				if (arr[_city].nameCity[_letter] != buf[_letter])
					break;
				else if (_letter + 1 == length_city)
					counter += arr[_city].callDuration;
		}
		char line[11];
		_itoa(counter, line, 10);
		int discharge = 0;
		do {
			counter /= 10; discharge++;
		} while (counter);
		line[discharge] = '\0';
		send(s2, line, discharge + 1, 0);
	}
	closesocket(s2);
	return 0;
}
int numcl = 0;
void print(){
	if (numcl) printf("%d client connected\n", numcl);
	else printf("No clients connected\n");
}
int main(void){
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0) { return -1; }
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, 5);
	cout << "Server receive ready" << endl;
	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);
	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size))){
		numcl++;
		print();
		DWORD thID;
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
	return 0;
}
