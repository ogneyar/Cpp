#include <iostream> 
#include <cstring>

#define LENGTH_URL 256

using namespace std;


class Url { 

private:
    char protocol[LENGTH_URL];
    char host[LENGTH_URL];
    int port;
    char *link;

public: 
    Url( char * url ) { 

        cout << "Start parser" << endl << endl; 

        parse_protocol( url );
        char * uri = parse_uri( url );
        Url::link = strchr( uri, '/' );
        parse_domain( uri );
        parse_get_param( Url::link );

    } 

    // парсим тип протокола передачи данных (http, ftp, ssh)
    void parse_protocol(char * url) {
        // unsigned long long переменная
        size_t pch2;
        // узнаём номер символа в строке url
        pch2 = strcspn(url,"://");
        // копируем в приватную переменную строку url
        strcpy(Url::protocol, url);
        // обрезаем по двоеточие (://)
        Url::protocol[pch2] = '\0';
        // cout << "protocol: ... " << Url::protocol << endl << endl;
    }

    // отбрасываем от url имя протокола
    char * parse_uri(char *url) {
        char *uri = strchr( url, '/' );
        if ( uri != 0 ) {
            uri = ++uri;
            uri = ++uri;
        }
        return uri;
    }

    // парсим имя домена и его порт
    void parse_domain(char *uri) {

        char *pch = strchr( uri, ':' );

        strcpy(Url::host, uri);
        
        if ( pch == 0 ) {
            Url::port = 80;
            pch = strchr( uri, '/' );
            Url::host[strlen(uri) - strlen(pch)] = '\0';
        }else {
            // нахождения позиции символа ':' и вместо него установка указателя конца строки
            Url::host[strlen(uri) - strlen(pch)] = '\0';
            
            Url::port = atoi( ++pch );
            if ( Url::port == 0 ) Url::port = 80;
        }
        // cout << "domain: " << Url::host << endl << endl;
        // cout << "port: " << Url::port << endl << endl;
    }

    // парсим GET параметры, если они есть
    void parse_get_param(char *linker) {
        // сохраняем GET запрос
        char *get = strchr( linker, '?' );
        // если есть GET параметры
        if ( strlen(get) > 0 ) {
            // unsigned long long переменная
            size_t pch2;
            // сохраняем в ней номер позиции знака ?
            pch2 = strcspn(linker,"?");
            // удаляем из пути GET параметры
            Url::link[pch2] = '\0'; // можно было так: Url::link[strcspn(linker,"?")] = '\0'; 

            // cout << "link: " << Url::link << endl << endl;

            // удаляем знак '?'
            ++get;
            int count = 0;

            // char *name[100][100];
            // char *value[100][100];

            char getParam[LENGTH_URL]; 
            char *par;

            size_t t = strcspn( get, "&" );
            int len = strlen( get );

            while( t < len ) {
                
                strcpy(getParam, get);                
                getParam[ strcspn( get, "&" ) ] = '\0';

                par = strchr( getParam, '=' );
                // value[ count ][ strlen( par ) ] = ++par;
                cout << "value[count]: " << ++par << endl << endl;

                getParam[ strcspn( getParam, "=" ) ] = '\0';
                // name[ count ][ strlen( getParam ) ] = getParam;
                cout << "name[count]: " << getParam << endl << endl;

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
            cout << "value[count]: " << ++par << endl << endl;

            get[ strcspn( get, "=" ) ] = '\0';
            // name[ count ][ strlen( get ) ] = get;
            cout << "name[count]: " << get << endl << endl;


            // cout << "name[count]: " << *name[count] << endl << endl;
            // cout << "value[count]: " << *value[count] << endl << endl;

            count++;



        }else { // если нет GET параметров, то ничего не надо делать :)
            // cout << "link: " << linker << endl << endl;
        }
    }

    char * getProtocol() { return protocol; }
    char * getHost() { return host; }
    int getPort() { return port; }
    char * getLink() { return link; }

};
