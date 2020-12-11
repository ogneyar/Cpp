#ifdef _WIN64
    #define OS_BIT 64
#else
    #ifdef _WIN32
    #define OS_BIT 32
    #endif
#endif

#include <fstream> // работа с файлами
// #include <iomanip> // манипуляторы ввода/вывода
#include <iostream> // функции ввода/вывода
#include <sstream>

#include <cstring>

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"


using namespace std;
using namespace rapidjson;

int writeFile();
string readFile(string);
void rapid();

class E { public: void Exception() { cout << "exception" << endl; } };


int main() {
    // корректное отображение Кириллицы (не работает)
    setlocale(LC_ALL, "Russian");
    cout << "Пиши по РУССКИ!\n\n";

    cout << "win" << OS_BIT << "\n\n";

    // writeFile();

    // cout << readFile("index.html");

    // cout << readFile("text.json").c_str();

    rapid();
  

    cout << endl;
    system("pause");

    return 0;
}


int writeFile() {

    // создаём файл и привязываем его к объекту класс ofstream
    ofstream fout("cpptest.txt");

    if (!fout.is_open()) {// если файл не открыт
        cout << "File cpptest.txt not open for write!\n"; // сообщить об этом
        return -1;
    }

    // делаем запись в файл
    fout << "Work with files on C++\n";
    fout << ".Work with files on C++\n";
    fout << "..Work with files on C++";
    // закрываем файл
    fout.close();
    cout << "Made an entry in the file cpptest.txt\n\n";

    return 0;
}


string readFile(string fileName) {

    // буфер промежуточного хранения считываемого из файла текста
    char buff[1024]; 

    stringstream response;
    
    // открыли файл для чтения
    ifstream fin(fileName);
    
    if (!fin.is_open()) {// если файл не открыт
        response << "File " << fileName << " not open for read!\n"; // сообщить об этом
        return response.str();
    }

    // считали первое слово из файла
    // fin >> buff; 
    // выводим на экран
    // cout << buff << endl;
    
    while (!fin.eof()) {
        // считали строку из файла
        fin.getline(buff, 1024);
        // запись в переменную
        response << buff << endl;
    }
    
    // закрываем файл
    fin.close(); 

    // cout << response.str() << endl;
    
    //Освобождаем буффер
    // delete [] buff;

    return response.str();
}


void rapid() {
    try {

        // 1. Parse a JSON string into DOM.
        string jsonFile = readFile("text.json");

        char szBuf[200];
        // .copy('куда копировать', 'до какого номера символа', ['от какого номера символа'])
        int nLength = jsonFile.copy(szBuf, jsonFile.length()-1);
        szBuf[nLength] = '\0'; // завершаем строку в буфере
        
        const char* json = szBuf;

        // const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";

        Document d;
        d.Parse(json);

    
        // 2. Modify it by DOM.
        Value& s = d["stars"];
        s.SetInt(s.GetInt() + 1);
    
        Value& st = d["project"];
        cout << "project: " << st.GetString() << endl;

    
        // 3. Stringify the DOM
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);
    
        // Output {"project":"rapidjson","stars":11}
        cout << buffer.GetString() << endl;

    } catch (E e) {
        e.Exception();
    }
}

