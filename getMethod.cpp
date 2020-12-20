#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "myLibs/parse_url.h"
#include "myLibs/getIp.h"


static char Rec[2097152]; // 2Mb


int main() {
    
    // char url[] = "https://hutoryanin.ru/test";

    // char url[] = "http://hutoryanin.ru/test?id=this&name=haski";

    // char url[] = "http://localhost:8000/shop/room?id=this&name=haski";

    char url[] = "http://localhost:8000/test";

    // char url[] = "https://youtube.com?ggg=222";

    // char url[] = "https://youtube.com";
    
    cout << "Start parser url. Url: " << url << endl << endl; 
    
    Url Parse(url);


    char *protocol = Parse.getProtocol();
    cout << "protocol: " << protocol << endl ;

    char *host = Parse.getHost();
    cout << "host: " << host << endl ;

    int port = Parse.getPort();
    cout << "port: " << port << endl ;
    
    char *link = Parse.getLink();
    cout << "link: " << link << endl;

    char *path = Parse.getPath();
    cout << "path: " << path << endl << endl;

    char *ip = getIp(host);
    cout << "Ip: " << ip << endl << endl;



    // посимвольное сравнение строк
    // if (strnicmp(url,"http://",7) == 0) || (strnicmp(url,"https://",8) cout << "strnicmp http://" << endl << endl;

    

    // служебная структура для хранение информации
    // о реализации Windows Sockets
    WSADATA wsaData;
    // старт использования библиотеки сокетов процессом
    // (подгружается Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
        system("pause");
        return result;
    }


    // // char hooost[1024] = "hutoryanin.ru";
    // // cout << "gethostbyname: " <<  inet_ntoa(*((in_addr*)gethostbyname(hooost)->h_addr_list[0])) << endl;



    // struct addrinfo* addr = NULL; // структура, хранящая информацию
    // // об IP-адресе  слущающего сокета

    // // Шаблон для инициализации структуры адреса
    // struct addrinfo hints;
    // ZeroMemory(&hints, sizeof(hints));

    // // AF_INET определяет, что используется сеть для работы с сокетом
    // hints.ai_family = AF_INET;
    // hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
    // hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
    // // Сокет биндится на адрес, чтобы принимать входящие соединения
    // hints.ai_flags = AI_PASSIVE;
    
    
    // // Инициализируем структуру, хранящую адрес сокета - addr.
    // // HTTP-сервер будет висеть на заданном порту заданного хоста
    // result = getaddrinfo(host, "80", &hints, &addr);
    // // Если инициализация структуры адреса завершилась с ошибкой,
    // // выведем сообщением об этом и завершим выполнение программы 
    // if (result != 0) {
    //     cerr << "getaddrinfo failed: " << result << "\n";
    //     WSACleanup(); // выгрузка библиотеки Ws2_32.dll
    //     system("pause");
    //     return 1;
    // }


    // // Создание сокета
    // int listen_socket = socket(addr->ai_family, addr->ai_socktype,
    //     addr->ai_protocol);

    // // int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    // // Если создание сокета завершилось с ошибкой, выводим сообщение,
    // // освобождаем память, выделенную под структуру addr,
    // // выгружаем dll-библиотеку и закрываем программу
    // if (listen_socket == INVALID_SOCKET) {
    //     cerr << "Error at socket: " << WSAGetLastError() << "\n";
    //     freeaddrinfo(addr);
    //     WSACleanup();
    //     system("pause");
    //     return 1;
    // }

    // // bind(listen_socket, (sockaddr *)&addr, sizeof(addr));

	// // соединяемся с сервером
	// if(connect(listen_socket, (sockaddr *)&addr, sizeof(addr)) != 0) {
	// 	cout << "Error: failed connect to server.\n" << endl;
	// 	system("pause");
	// 	return 1;
	// }
	// cout << "Connected in the server.\n" << endl;





    SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	// создаём сокет
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	// соединяемся с сервером
	if(connect(listen_socket, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n" << endl;
		system("pause");
		return 1;
	}
	cout << "Connected in the server.\n" << endl;





    
    char query[2048];
    const char * user_agent = "Search Engine";
    // const char * user_agent = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/68.0.3440.75 Chrome/68.0.3440.75 Safari/537.36";


    const char * accept = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8";

    strcpy( query, "GET " );
    strcat( query, path );
    strcat( query, " HTTP/1.1\nHost: ");
    strcat( query, host);
    strcat( query, "\nUser-agent: ");
    // strcat( query, "\nConnection: keep-alive\nUser-agent: ");
    strcat( query, user_agent);
    strcat( query, "\nAccept: */*\n\n");
    // strcat( query, "\nAccept: ");
    // strcat( query, accept);
    // strcat( query, "\n\n");

    int cnt = send(listen_socket, query, strlen(query), 0);

    if (cnt == SOCKET_ERROR) {
		cout << "Error: failed send to server.\n" << endl;
		system("pause");
		return 1;
	}

    char InBuff[2048];

    strcpy(Rec, "");
    
    while (cnt!=0) {
        // clear InBuff - fill 2048 bytes to NULL
        memset(&InBuff,0,2048);
        // receive a 2048 bytes from 's' sock
        cnt = recv (listen_socket, (char*)&InBuff, sizeof(InBuff),0);
        // append it to main buffer 'Rec'
        strcat( Rec, InBuff );
    }
    char* str = (char*)&Rec;



    // char* str;
    // recv (listen_socket, (char*)str, sizeof(str),0);

    cout << "Response: " << endl << str << endl << endl;;






    // Убираем за собой
    closesocket(listen_socket);
    // freeaddrinfo(addr);
    WSACleanup();





    system("pause");

    return 0;
}

