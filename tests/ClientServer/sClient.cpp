// #include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h> // #include "targetver.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>

using namespace std;

// #pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler() {
	int msg_size;
	while(true) {
		// ждём сообщения от сервера	
		recv(Connection, (char*)&msg_size, sizeof(int), 0);
		char *msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		// ждём сообщения от сервера	
		recv(Connection, msg, msg_size, 0);
		cout << msg << endl;
		delete[] msg;
	}
}

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
	addr.sin_port = htons(8000);
	addr.sin_family = AF_INET;

	// создаём сокет
	Connection = socket(AF_INET, SOCK_STREAM, 0);
	// соединяемся с сервером
	if(connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n" << endl;
		system("pause");
		return 1;
	}
	cout << "Connected in the server.\n" << endl;

	// thread - синхронный процесс (запускается беЗконечный цикл в функции ClientHandler)
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientHandler, 0, 0, 0);

	std::string msg1;
	while(true) {
		// ждём ввода сообщения от клиента
		getline(cin, msg1);
		int msg_size = msg1.size();
		// отправляем сообщения на сервер
		send(Connection, (char*)&msg_size, sizeof(int), 0);
		send(Connection, msg1.c_str(), msg_size, 0);
		Sleep(10);
	}

	system("pause");
	closesocket(Connection);
	WSACleanup();
	
	return 0;
}