#include <iostream> // работа с вводом/выводом информации
#include <thread> // работа с потоками
#include <mutex> // запреты для thread (lock/unlock) 
#include <unistd.h> // sleep()

using namespace std;

mutex mute;

void foo(int a) { // якобы асинхронная функция
    mute.lock();
    
    cout << "this_thread::get_id(): " << this_thread::get_id() << endl;

    sleep(1);
    cout << "function foo(" << a << ")\n";
    mute.unlock();
}
void bar() { // якобы асинхронная функция (без параметров)
    sleep(1);
    cout << "function bar()" << '\n';
}

int main() {

    setlocale(0, "Russian"); // работа русского языка в консоли
    wcout << L"Прива ребзя :)" << endl;

    thread th(foo, 10); // thread работает синхронно

    // th.detach();
    
    wcout << L"Вывод информации после задержки #1" << endl;

    cout << "th.get_id: " << th.get_id() << endl;
    
    // this_thread::yield();
    // this_thread::sleep_for(5s);

    // bar(); // якобы асинхронная функция
    
    wcout << L"Вывод информации после задержки #2" << endl;
    
    // foo(20); // якобы асинхронная функция

    thread th2(bar); // thread работает синхронно  

    thread th3(foo, 20); // thread работает синхронно  

    wcout << L"Вывод информации после задержки #3" << endl;          

    cout << "th2.get_id: " << th2.get_id() << endl;

    wcout << L"Вывод информации после задержки #4" << endl;     

    th3.join(); // thread перестаёт синхронно работать   
    
    th.join(); // thread перестаёт синхронно работать

    th2.join(); // thread перестаёт синхронно работать

    wcout << L"Вывод информации закончил" << endl;

    system("pause");

    return 0;
}