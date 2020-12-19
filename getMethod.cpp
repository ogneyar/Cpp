#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// #include <stdio.h>
// #include <string>
#include <iostream> // работа вводом/выводом данных
// #include <sstream> // работа со стримами
// #include <cstring>

#define LENGTH_URL 256

using namespace std;

// typedef struct get;

// typedef struct {
//     char pr[100];
//     char *h[100];
//     char *p[100];
//     char *l[100];
//     get g;
// }object;


class Url { 
private:
    char protocol[LENGTH_URL];
    char host[LENGTH_URL];
    char port[LENGTH_URL];
    char link[LENGTH_URL];

public: 
    void parse(char * url) { 

        cout << "Start parser" << endl << endl; 

        parse_protocol(url);

    } 

    // парсим тип протокола передачи данных (http, ftp, ssh)
    void parse_protocol(char * url) {
        // unsigned long long переменная
        size_t pch2;
        // узнаём номер символа в строке url
        pch2 = strcspn(url,"://");
        // копируем в приватную переменную строку url
        strcpy(protocol, url);
        // обрезаем по двоеточие (://)
        protocol[pch2] = '\0';
        // cout << "protocol: ... " << protocol << endl << endl;
    }

    char * getProtocol() { return protocol; }
};

Url parse_url(char *);


int main() {

    char protocol[] = "http";
    char host[] = "localhost.ru";
    char port[] = "8000";
    char link[] = "/shop/room?id=this";

    char url[LENGTH_URL];

    if ( strlen(protocol) > 0 ) {
        strcpy(url, protocol);
        strcat(url, "://");
    }
    strcat(url, host);
    if ( strlen(port) > 0 ) {
        strcat(url, ":");
        strcat(url, port);
    }
    strcat(url, link);

    cout << endl << url << endl << endl;

    // char url[] = "http://localhost:8000";



    
    Url _URI = parse_url(url);
    cout << "protocol: " << _URI.getProtocol() << endl << endl;




    // if (strnicmp(url,"http://",7) == 0) || (strnicmp(url,"https://",8) cout << "strnicmp http://" << endl << endl;


    char *uri = strchr( (char*)url, '/' );
    if ( uri != 0 ) {
        uri = ++uri;
        uri = ++uri;
    }    
    // cout << uri << endl << endl;

    char *linker = strchr( (char*)uri, '/' );
    
    char *get = strchr( (char*)linker, '?' );

    
    int PortNum;
    char *pch = strchr( (char*)uri, ':' );
    
    if ( pch == 0 ) {
        PortNum = 80;
    }else {
        // нахождения позиции символа ':' и вместо него установка указателя конца строки
        uri[strlen(uri) - strlen(pch)] = '\0';
        
        PortNum = atoi( ++pch );
        if ( PortNum == 0 ) PortNum = 80;
    }

    cout << "domain: " << uri << endl << endl;

    cout << "port: " << PortNum << endl << endl;


    // cout << "link: " << linker << endl << endl;

    size_t pch2;
    pch2 = strcspn(linker,"?");
    if ( pch2 > 0 ) {
        
        char lin[strlen(linker)];
        strcpy(lin, linker);
        lin[pch2] = '\0';

        cout << "link: " << lin << endl << endl;
     }else cout << "link: " << linker << endl << endl;

    


    if ( strlen(get) > 0 ) {

        // cout << "get: " << ++get << endl << endl;

        ++get;

        char *value = strchr( (char*)get, '=' );

        // cout << "value: " << value << endl << endl;
        
        
        char name[strlen(get)];
        strcpy(name, get);

        // cout << "name: " << name << endl << endl;

        pch2 = strcspn(name,"=");

        name[pch2] = '\0';

        value = ++value;
        cout << "_GET['" << name << "']: " << value << endl << endl;
    }


    // // служебная структура для хранение информации
    // // о реализации Windows Sockets
    // WSADATA wsaData;
    // // старт использования библиотеки сокетов процессом
    // // (подгружается Ws2_32.dll)
    // int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    // // Если произошла ошибка подгрузки библиотеки
    // if (result != 0) {
    //     cerr << "WSAStartup failed: " << result << "\n";
    //     return result;
    // }

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

    //     // Если инициализация структуры адреса завершилась с ошибкой,
    //     // выведем сообщением об этом и завершим выполнение программы 
    //     if (result != 0) {
    //         cerr << "getaddrinfo failed: " << result << "\n";
    //         WSACleanup(); // выгрузка библиотеки Ws2_32.dll
    //         return 1;
    //     }

    //     // Создание сокета
    //     int listen_socket = socket(addr->ai_family, addr->ai_socktype,
    //         addr->ai_protocol);
    //     // Если создание сокета завершилось с ошибкой, выводим сообщение,
    //     // освобождаем память, выделенную под структуру addr,
    //     // выгружаем dll-библиотеку и закрываем программу
    //     if (listen_socket == INVALID_SOCKET) {
    //         cerr << "Error at socket: " << WSAGetLastError() << "\n";
    //         freeaddrinfo(addr);
    //         WSACleanup();
    //         return 1;
    //     }


    //     // Привязываем сокет к IP-адресу
    //     result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    //     // Если привязать адрес к сокету не удалось, то выводим сообщение
    //     // об ошибке, освобождаем память, выделенную под структуру addr.
    //     // и закрываем открытый сокет.
    //     // Выгружаем DLL-библиотеку из памяти и закрываем программу.
    //     if (result == SOCKET_ERROR) {
    //         cerr << "bind failed with error: " << WSAGetLastError() << "\n";
    //         freeaddrinfo(addr);
    //         closesocket(listen_socket);
    //         WSACleanup();
    //         return 1;
    //     }


    //     // Инициализируем слушающий сокет
    //     if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
    //         cerr << "listen failed with error: " << WSAGetLastError() << "\n";
    //         closesocket(listen_socket);
    //         WSACleanup();
    //         return 1;
    //     }

    
    //     cout << "Server rex hes been started on " << host << ".\n";
    //     cout << "Listen port " << port << "... ";

    //     const int max_client_buffer_size = 2048;
    //     char buf[max_client_buffer_size];

    //     long long unsigned int client_socket = INVALID_SOCKET;

    //     for (;;) {

    //         // Принимаем входящие соединения
    //         client_socket = accept(listen_socket, NULL, NULL);
    //         if (client_socket == INVALID_SOCKET) {
    //             cerr << "accept failed: " << WSAGetLastError() << "\n";
    //             closesocket(listen_socket);
    //             WSACleanup();
    //             return 1;
    //         }
        

    //         result = recv(client_socket, buf, max_client_buffer_size, 0);

    //         stringstream response; // сюда будет записываться ответ клиенту
    //         stringstream response_body; // тело ответа

    //         if (result == SOCKET_ERROR) {
    //             // ошибка получения данных
    //             cerr << "recv failed: " << result << "\n";
    //             closesocket(client_socket);
    //         } else if (result == 0) {
    //             // соединение закрыто клиентом
    //             cerr << "connection closed...\n";
    //         } else if (result > 0) {
    //             // Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
    //             // В буфере запроса.
    //             // (-4) поставил так как при считывании данных дублировалась строка 'cookies' 
    //             buf[result-4] = '\0';


    //             // Переданные данные от клиента записываем в строку JSON
    //             string json = Rex::getRequest(buf);

    //             mkdir("temp");
    //             string temp = "temp/rex.request.json";

    //             // и сохраняем в файл (почему-то пока только с файлом работает rapidjson)
    //             writeFile(temp, json);

    //             // запрашиваем поле path (путь в адресной строке браузера)
    //             string path = parserFile(temp, "path");   


    //             // буфер для перевода string в массив char
    //             char pathBuf[path.length()];                
    //             int pathLength = path.copy(pathBuf, path.length());
    //             // если в конце адреса нет слеша, то добавляем
    //             if (pathBuf[pathLength-1] != '/') path = path + "/";
               
                
    //             // запрашиваем поле method (GET PUT POST...)
    //             string method = parserFile(temp, "method");
                
                
    //             if (path != "/favicon.ico/") {
    //                 // в консоли выводим сообщение, кроме когда браузером запрашивается иконка
    //                 cout << "Request method: " << method << " " << path << "\n";
                                    

    //                 // Данные успешно получены
    //                 // формируем тело ответа (HTML)
    //                 string route = Rex::getRoutes(path);
    //                 // string route = parserFile("routes/route.json", path);
    //                 // если есть такая запись в файле route.json
    //                 if (route != "Error") {
    //                     string html;
    //                     // если view в корневом каталоге, то возвращаем только название html файла без '/'
    //                     if (view == "/") html = route;
    //                     // иначе полный путь к html файлу
    //                     else html = view + "/" + route;

    //                     // если такой файл существует
    //                     if (fileExist(html)) {
    //                         // считываем файл
    //                         response_body << readFile(html);
    //                     }else { // иначе создаём 'базовый' html
    //                         response_body << Rex::createIndexHtml(view, html);
    //                     }
    //                 }else { // вывод информации 'страница не найдена'
    //                     response_body << "<br><br><center>";
    //                     response_body << "<h1>Error 404. Page not found.</h1>";
    //                     response_body << "</center><br><smal>Server rex.</smal>";
    //                     response_body << "<br><hr>";
    //                 }                        

    //                 cout << "Listen port " << port << "... ";               

    //                 // Формируем весь ответ вместе с заголовками
    //                 response << "HTTP/1.1 200 OK\r\n"
    //                     << "Version: HTTP/1.1\r\n"
    //                     << "Content-Type: text/html; charset=utf-8\r\n"
    //                     << "Content-Length: " << response_body.str().length()
    //                     << "\r\n\r\n"
    //                     << response_body.str(); 

    //                 // Отправляем ответ клиенту с помощью функции send
    //                 result = send(client_socket, response.str().c_str(),
    //                     response.str().length(), 0);

    //                 if (result == SOCKET_ERROR) {
    //                     // произошла ошибка при отправле данных
    //                     cerr << "send failed: " << WSAGetLastError() << "\n";
    //                 }
                    
    //             }

    //             // Закрываем соединение к клиентом
    //             closesocket(client_socket);
    //         }


    //     }

    //     // Убираем за собой
    //     closesocket(listen_socket);
    //     freeaddrinfo(addr);
    //     WSACleanup();





    system("pause");

    return 0;
}



Url parse_url(char * url) {

    Url u;
    u.parse(url);

    return u;
}