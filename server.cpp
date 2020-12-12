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

                
        Server rex; // rex берёт настройки из server.config.json
        // можно и так подключить
        // Server rex("localhost","8080", "view"); 
        rex.run(); // старт сервера
        

    }catch(E e) {
        e.Exception();
    }



    return 0;
}
//===========================================================================