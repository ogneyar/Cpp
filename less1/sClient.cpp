// #include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h> // #include "targetver.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

using namespace std;

// #pragma warning(disable: 4996)

int main(int argc, char* argv[]) {
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "WSAStartup failed\n" << endl;
		system("pause");
		return 1;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	// создаём сокет
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// соединяемся с сервером
	if(connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n" << endl;
		system("pause");
		return 1;
	}
	cout << "Connected number " << Connection << "\n" << endl;

	char msg[256];
	// ждём сообщения от сервера
	recv(Connection, msg, sizeof(msg), 0);
	cout << msg << endl;

	system("pause");
	closesocket(Connection);
	WSACleanup();

	return 0;
}