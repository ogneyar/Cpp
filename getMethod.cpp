#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "myLibs/parse_url.h"
#include "myLibs/getIp.h"


static char Rec[2097152]; // 2Mb


int main() {
    
    // char url[] = "https://hutoryanin.ru/test";

    // char url[] = "http://localhost:8000/shop/room?id=this&name=haski";  

    // char url[] = "http://local:8000/test";

    char url[] = "http://ip.jsontest.com/";

    // char url[] = "http://headers.jsontest.com/";
    
    
    cout << "Start parser url. Url: " << url << endl << endl; 
    
    Url Parse(url);

    // функция возвращает тип протокола, по умолчанию http
    char *protocol = Parse.getProtocol();
    cout << "protocol: " << protocol << endl ;
    // функция возвращает имя хоста (доменное имя)
    char *host = Parse.getHost();
    cout << "host: " << host << endl ;
    // функция возвращает номер порта, по умолчанию 80
    int port = Parse.getPort();
    cout << "port: " << port << endl ;
    // функция возвращает путь ссылки вместе с GET параметрами 
    char *link = Parse.getLink();
    cout << "link: " << link << endl;
    // функция возвращает путь ссылки без GET параметров 
    char *path = Parse.getPath();
    cout << "path: " << path << endl << endl;
    // функция возвращает ip-адресс заданного хоста
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

    sockaddr_in addr;	
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	// создаём сокет
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	// соединяемся с сервером
	if(connect(listen_socket, (sockaddr*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n" << endl;
		system("pause");
		return 1;
	}
	cout << "Connected in the server.\n" << endl;

    
    char query[2048];
    // const char * user_agent = "Search Engine";
    const char * user_agent = "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.198 Safari/537.36 OPR/72.0.3815.400";

    const char * accept = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8";

    strcpy( query, "GET " );
    strcat( query, path );
    strcat( query, " HTTP/1.0\nHost: ");
    strcat( query, host);
    strcat( query, "\nUser-agent: ");    
    strcat( query, user_agent);
    // strcat( query, "\nAccept: */*\n\n");
    strcat( query, "\nAccept: ");
    strcat( query, accept);
    strcat( query, "\n\n");

    int cnt = send(listen_socket, query, strlen(query), 0);

    if (cnt == SOCKET_ERROR) {
		cout << "Error: failed send to server.\n" << endl;
		system("pause");
		return 1;
	}

    char InBuff[2048];

    strcpy(Rec, "");
    
    while (cnt!=0) {
        // очищаем буфер InBuff - устанавливаем 2048 байт нулями
        memset(&InBuff,0,2048);
        // принимаем 2048 байт от сокета
        cnt = recv (listen_socket, (char*)&InBuff, sizeof(InBuff),0);
        // добавляем в конец буфера 'Rec'
        strcat( Rec, InBuff );
    }
    // char* str = (char*)&Rec;

    cout << "Response: " << endl << Rec << endl << endl;;


    // Убираем за собой
    closesocket(listen_socket);    
    WSACleanup();

    system("pause");

    return 0;
}

