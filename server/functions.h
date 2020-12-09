
#include <fstream> // работа с файлами
#include <iostream> // работа вводом/выводом данных
#include <sstream> // работа со стримами
#include <string> // работа со строками

#include <cstring> // работа со строками (strlen)

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace std;
using namespace rapidjson;

// int server(string);
// string readFile(string);
// void parserConfig();

using namespace std;

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
        // выводим на экран
        response << buff << endl;
    }
    
    // закрываем файл
    fin.close(); 

    // cout << response.str() << endl;
    
    //Освобождаем буффер
    // delete [] buff;

    return response.str();
}


string parserConfig(string field) {

    string response;

    string jsonFile = readFile("server.config.json");

    char szBuf[1024];
    // .copy('куда копировать', 'до какого номера символа', ['от какого номера символа'])
    int nLength = jsonFile.copy(szBuf, jsonFile.length()-1);
    szBuf[nLength] = '\0'; // завершаем строку в буфере
    
    const char* json = szBuf;

    Document document;
    document.Parse(json);

    Value& f = document[field.c_str()];
    response = f.GetString();


    return response;

}