#include <iostream> 
#include <cstring>

#define LENGTH_URL 256

using namespace std;


class Url { 

private:
    char protocol[LENGTH_URL];
    char host[LENGTH_URL];
    int port;
    char link[LENGTH_URL]; // путь path с GET параметрами
    char path[LENGTH_URL];

public: 
    Url( char * url ) { // конструктор класса
        // cout << "Start parser" << endl << endl; 

        // парсим тип протокола передачи данных (http, ftp, ssh)
        parse_protocol( url );

        // парсим путь 'path' с 'GET' параметрами
        parse_link( url );

        // парсим имя домена и его порт
        parse_domain( url );

        // парсим GET параметры, если они есть
        parse_get_param( url );
    } 


    // парсим тип протокола передачи данных (http, ftp, ssh)
    char * parse_protocol(char * url) {
        
        // узнаём номер символа в строке url
        size_t pch2 = strcspn(url,":");
        // если есть такой символ в строке 
        if (pch2 != strlen(url)) {

            // сохраняем всё что после ':'
            char *uri = strchr( url, ':' );
            // проверяем, есть ли '/' после ':'
            if (uri[1] == '/') {
                
                // копируем в приватную переменную строку url
                strcpy(Url::protocol, url);
                // обрезаем по двоеточие (://)
                Url::protocol[pch2] = '\0';

                return Url::protocol;
            }
        }

        // запись значения по умолчанию 'http'
        strcpy(Url::protocol, "http");
        // функция возвращавет глобальную переменную
        return Url::protocol; 
        
    }


    char * parse_link( char * url ) {
        // отбрасываем от url имя протокола, если он есть
        char * uri = parse_uri( url );

        // номер позиции в строке
        int lin = strcspn( uri, "/" );
        // количество символов
        int len = strlen( uri );

        if ( len != lin ) {
            // путь url адреса после имени домена
            strcpy( Url::link, strchr( uri, '/' ) );
        }else {
            // сохраняем GET запрос
            if ( strcspn( uri, "?" ) != len ) {
                strcpy( Url::link, strchr( uri, '?' ) );
            }else strcpy( Url::link, "/" );
        }

        return Url::link;
    }


    // отбрасываем от url имя протокола
    char * parse_uri(char *url) {
        // узнаём номер символа в строке url
        size_t pch2 = strcspn( url, ":" );
        // если есть такой символ в строке 
        if ( pch2 != strlen( url ) ) {
            // сохраняем всё что после ':'
            char *uri = strchr( url, ':' );
            // проверяем, есть ли '/' после ':'
            if (uri[1] == '/') { // uri получается типа '://localhost.ru/'
                uri+=3; // сдвигаем строку на три символа
                return uri;
            }        
        }
        // функция возвращавет полученый url
        return url; 
    }


    // парсим имя домена и его порт
    char * parse_domain(char *url) {
        // отбрасываем от url имя протокола, если он есть
        char * uri = parse_uri( url );

        // находим номер символа ':' в строке 
        char *pch = strchr( uri, ':' );
        // копируем в глобальную переменную класса весь url, без протокола
        strcpy(Url::host, uri);
        // если не указан номер порта
        if ( pch == 0 ) {
            // тогда по умолчанию 80 порт
            Url::port = 80;

            // номер позиции в строке
            int lin = strcspn( uri, "/" );
            // количество символов
            int len = strlen( uri );

            if ( len != lin ) {
                // обрезаем всё что после имени домена
                Url::host[ lin ] = '\0';
            }else {
                // сохраняем GET запрос
                if ( strcspn( uri, "?" ) != strlen( uri ) ) Url::host[strcspn( uri, "?")] = '\0';
            }
            
        }else {
            // нахождения позиции символа ':' и вместо него установка указателя конца строки
            Url::host[strlen(uri) - strlen(pch)] = '\0';
            // переводим строку в число и записываем в глобальную переменную
            Url::port = atoi( ++pch );
            // по умолчанию 80 порт
            if ( Url::port == 0 ) Url::port = 80;
        }

        return Url::host;
    }


    // парсим GET параметры, если они есть
    void parse_get_param(char *url) {
        
        char linker[LENGTH_URL];
        strcpy( linker, parse_link( url ) );

        // копируем в переменую path весь путь, если есть GET параметры то их позже удалим        
        strcpy( Url::path, linker );
                       
        // номер позиции в строке
        int lin = strcspn( linker, "?" );
        int len = strlen(linker);

        // если есть GET параметры
        if ( len != lin ) {

            // сохраняем GET запрос
            char *get = strchr( linker, '?' );

            // unsigned long long переменная
            size_t pch2;
            // сохраняем в ней номер позиции знака ?
            pch2 = strcspn( linker, "?" );
            
            // удаляем из пути GET параметры
            Url::path[pch2] = '\0'; // можно было так: Url::path[strcspn(linker,"?")] = '\0'; 

            if ( pch2 == 0 ) strcpy( Url::path, "/" );

            // удаляем знак '?'
            ++get;
            // устанавливаем счётчик количества GET параметров на 0
            int count = 0;

            // char *name[100][100];
            // char *value[100][100];

            char getParam[LENGTH_URL]; 
            char *par;

            size_t t = strcspn( get, "&" );
            len = strlen( get );

            while( t < len ) {
                
                strcpy(getParam, get);                
                getParam[ strcspn( get, "&" ) ] = '\0';

                par = strchr( getParam, '=' );
                // value[ count ][ strlen( par ) ] = ++par;
                // cout << "value[count]: " << ++par << endl << endl;

                getParam[ strcspn( getParam, "=" ) ] = '\0';
                // name[ count ][ strlen( getParam ) ] = getParam;
                // cout << "name[count]: " << getParam << endl << endl;

                // cout << "name[count]: " << name[count] << endl << endl;
                // cout << "value[count]: " << value[count] << endl << endl;
                
                get = strchr( get, '&' );
                ++get;

                t = strcspn( get, "&" );
                len = strlen( get );

                count++;
            }


            par = strchr( get, '=' );
            // value[ count ][ strlen( par ) ] = ++par;
            // cout << "value[count]: " << ++par << endl << endl;

            get[ strcspn( get, "=" ) ] = '\0';
            // name[ count ][ strlen( get ) ] = get;
            // cout << "name[count]: " << get << endl << endl;


            // cout << "name[count]: " << *name[count] << endl << endl;
            // cout << "value[count]: " << *value[count] << endl << endl;

            count++;


        }else { // если нет GET параметров, то ничего не надо делать :)
            // cout << "path: " << linker << endl << endl;            
        }

    }

    char * getProtocol() { return Url::protocol; }
    char * getHost() { return Url::host; }
    int getPort() { return Url::port; }
    char * getLink() { return Url::link; }
    char * getPath() { return Url::path; }

};
