// https://code-live.ru/post/cpp-http-server-over-sockets/

#include <fstream> // работа с файлами
#include <iostream> // работа вводом/выводом данных
#include <sstream> // работа со стримами
#include <string> // работа со строками

#include "server.h"

using namespace std;

class E { public: void Exception() { cout << "exception" << endl; } };

int main() {
    try{
        string field = "index";
        string index = parserConfig(field);

        Server s;
        s.server(index);

    }catch(E e) {
        e.Exception();
    }

    // E e;
    // e.Exception();

    return 0;
}
//===========================================================================