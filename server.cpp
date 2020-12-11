// https://code-live.ru/post/cpp-http-server-over-sockets/

#include <fstream> // работа с файлами
#include <iostream> // работа вводом/выводом данных
#include <sstream> // работа со стримами
#include <string> // работа со строками

#include "include/rex/rex.server.h"

using namespace std;

class E { public: void Exception() { cout << "exception" << endl; } };

int main() {
    try{
        string field = "path";
        string path = parserConfig(field);

        // Server s(nullptr,"8080");
        Server rex;
        rex.run(path); 

    }catch(E e) {
        e.Exception();
    }

    // E e;
    // e.Exception();

    return 0;
}
//===========================================================================