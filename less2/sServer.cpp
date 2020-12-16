// #include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h> // #include "targetver.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

// #include <sstream> // работа со стримами

using namespace std;

// #pragma warning(disable: 4996)

SOCKET Connections[100];
int Counter = 0;

// функция рассылки сообщения от клиента
void ClientHandler(int index) {
	char msg[256];

	while(true) {
		// принимаем сообщение от сокета (Connections) под номером index
		recv(Connections[index], msg, sizeof(msg), 0);
		for(int i = 0; i < Counter; i++) {
			if(i == index) { // отсылаем всем кроме самого отправителя
				cout << Connections[index] << ": " << msg << endl;
				continue;
			}
			
			// отсылаем сообщение всем подключеным клиентам
			send(Connections[i], msg, sizeof(msg), 0);
		}
	}
}

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
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	// привязываем сокет к ip-адресу
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	// слушаем сокет
	listen(sListen, SOMAXCONN);

	cout << "Server started...\n" << endl;

	SOCKET newConnection;

	for(int i = 0; i < 100; i++) {
		// Принимаем входящие соединения
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if(newConnection == 0) {
			cout << "Error accept newConnection\n";
		} else {
			cout << "Client number: " << newConnection << " Connected!\n";
			char msg[256] = "Hello. Welcome on our server!";
			// отправляем сообщение клиенту
			send(newConnection, msg, sizeof(msg), 0);

			Connections[i] = newConnection;
			Counter++;

			// thread - синхронный процесс (запускается беЗконечный цикл в функции ClientHandler)
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), 0, 0);
		}
	}
	for(int j = 0; j < Counter; j++) {
		closesocket(Connections[j]);
	}

	// system("pause");
	closesocket(sListen);
	WSACleanup();

	return 0;
}