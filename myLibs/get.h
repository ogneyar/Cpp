#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include "parse_url.h"
#include "getIp.h"

static char Rec[2097152]; // 2Mb


char * get(char url[]) {
    
    // подключение класса парсера url
    Url Parse(url);
    // функция возвращает имя хоста (доменное имя)
    char *host = Parse.getHost();
    // функция возвращает номер порта, по умолчанию 80
    int port = Parse.getPort();
    // функция возвращает путь ссылки вместе с GET параметрами 
    char *link = Parse.getLink();
    // функция возвращает ip-адресс заданного хоста
    char *ip = getIp(host);

    // служебная структура для хранение информации о реализации Windows Sockets
    WSADATA wsaData;
    // старт использования библиотеки сокетов процессом (подгружается Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
        return (char *)"Error";
    }

    sockaddr_in addr;	
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	// создаём сокет
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	// соединяемся с сервером
	if(connect(listen_socket, (sockaddr*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n" << endl;
		return (char *)"Error";
	}
    
    char query[2048];
    const char * user_agent = "Search Engine";
    // формируем запрос к серверу
    strcpy( query, "GET " );
    strcat( query, link );
    strcat( query, " HTTP/1.0\nHost: ");
    strcat( query, host);
    strcat( query, "\nUser-agent: ");    
    strcat( query, user_agent);
    strcat( query, "\nAccept: */*\n\n");
    // отправляем запрос на сервер
    int cnt = send(listen_socket, query, strlen(query), 0);
    if (cnt == SOCKET_ERROR) {
		cout << "Error: failed send to server.\n" << endl;
		return (char *)"Error";
	}

    char InBuff[2048];
    strcpy(Rec, "");
    
    while ( cnt != 0 ) {
        // очищаем буфер InBuff - устанавливаем 2048 байт нулями
        memset( &InBuff, 0, 2048 );
        // принимаем 2048 байт от сокета
        cnt = recv( listen_socket, (char *)&InBuff, sizeof( InBuff ), 0 );
        // добавляем в конец буфера 'Rec'
        strcat( Rec, InBuff );
    }
    
    int count = 0;
    // ищем в ответе сервера два энтера подряд (\r\n\r\n)
    for (int i = 0; i < strlen(Rec); i++) {
        if ( Rec[i] == '\r' )
            if ( Rec[i + 1] == '\n' )
                if ( Rec[i + 2] == '\r' )
                    if ( Rec[i + 3] == '\n' ) count = i + 4;
    }

    // Убираем за собой
    closesocket( listen_socket );    
    WSACleanup();

    return &Rec[count];
}

