
#include "parse_url.h"


int main() {

    char url[] = "http://localhost:8000/shop/room?id=this&name=haski";
    
    cout << "Start parser url." << endl << endl; 
    cout << "url: " << url << endl << endl;


    Url Parse(url);
    
    char *protocol = Parse.getProtocol();
    cout << "protocol: " << protocol << endl ;

    char *host = Parse.getHost();
    cout << "host: " << host << endl ;

    int port = Parse.getPort();
    cout << "port: " << port << endl ;

    char *path = Parse.getPath();
    cout << "path: " << path << endl << endl;


    cout << "url: " << url << endl << endl;


    system("pause");

    return 0;
}

