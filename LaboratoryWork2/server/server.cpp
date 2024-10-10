#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main(){
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData)){
		cout << "Error connecting the winsock2 library";
		return 1;
	}
	SOCKET socket_connection = socket(PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(1024);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(socket_connection, (struct sockaddr *)&address, sizeof(address));
	cout << "Server is running" << endl;
	char line[265];
	int size_address = sizeof(address);
	char address_line[50];
	while (true){
		recvfrom(socket_connection, line, sizeof(line), 0, (struct sockaddr*)&address, &size_address);
		inet_ntop(AF_INET, &(address.sin_addr), address_line, 49);
		cout << "--------------------------------------------------------" << endl
			<< "client_port = " << ntohs(address.sin_port) << endl
			<< "client_IP-addr = " << address_line << endl << endl
			<< "Client pass string(my):   " << line << endl;
		int length_line = strlen(line);
		if (length_line % 4 == 0) {
			int length_new_line = 0,
				current_number = 0,
				counter = 0;
			char new_line[265];
			new_line[0] = '\n';
			for (int _letter = 0; _letter < length_line; _letter++) {
				if (line[_letter] >= '0' && line[_letter] <= '9')
					current_number = current_number * 10 + line[_letter] - '0';
				else {
					if (current_number % 4 == 0)
						current_number /= 4, counter++;
					_itoa(current_number, new_line + length_new_line, 10);
					while (current_number)
						length_new_line++, current_number /= 10;
					new_line[length_new_line++] = line[_letter];
					current_number = 0;
				}
			}
			new_line[length_new_line++] = ' ';
			_itoa(counter, new_line + length_new_line, 10);
			while (counter)
				length_new_line++, counter /= 10;
			new_line[length_new_line++] = '\0';
			sendto(socket_connection, new_line, length_new_line, 0, (struct sockaddr*)&address, size_address);
			cout << "Server return string: " << new_line << endl << endl;
		}
		else {
			sendto(socket_connection, line, strlen(line) + 1, 0, (struct sockaddr*)&address, size_address);
			cout << "Server return string: " << line << endl << endl;
		}
	}
  	closesocket(socket_connection);
	WSACleanup();
	return 0;
}

//вывелось число кол-во разрядов умноженное на 2, а надо было посчитать кол-во их
//надо было удалить кратно 4, а не делить на 4
//надо было вывести отдельно строку и число
