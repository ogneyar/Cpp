// example parse url (пример разбора ссылки)
#include "parse_url.h"


int main() {

    char url[] = "http://localhost:8000/shop/room?id=this&name=haski";    
    
    cout << "Start parser url." << endl << endl; 
    cout << url << endl << endl;

    // подключение класса парсера url
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

    system("pause");

    return 0;
}

