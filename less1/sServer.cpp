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
		cout << "WSAStartup failed" << endl;
		return 1;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	// создаём сокет
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// привязываем сокет к ip-адресу
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	// слушаем сокет
	listen(sListen, SOMAXCONN);

	cout << "Server started..." << endl;

	SOCKET newConnection;
	while(true) {		
		// Принимаем входящие соединения
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if(newConnection == 0) {
			cout << "Error accept newConnection\n";
		} else {
			cout << "Client number: " << newConnection << " Connected!\n";
			char msg[256] = "Hello. It`s my first network program!";
			// Отправляем сообщение подключившемуся клиенту
			send(newConnection, msg, sizeof(msg), 0);
		}

		// closesocket(newConnection);
	}

	system("pause");
	closesocket(sListen);
	WSACleanup();

	return 0;
}