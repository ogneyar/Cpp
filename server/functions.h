
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


int writeFile(string, string);
string readFile(string);
string parserConfig(string);
bool fileExist(string);
// const char *strToChar(string);

using namespace std;


int writeFile(string file, string text) {
    // создаём файл и привязываем его к объекту класс ofstream
    ofstream fout(file);
    if (!fout.is_open()) {// если файл не открыт
        cout << "File " << file << " not open for write!\n"; // сообщить об этом
        return -1;
    }
    // делаем запись в файл
    fout << text;
    // закрываем файл
    fout.close();
    return 0;
}


string readFile(string fileName) {

    int lenBuff = 1024;

    // буфер промежуточного хранения считываемого из файла текста
    char buff[lenBuff]; 

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
        fin.getline(buff, lenBuff);
        
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

    string response = " ";

    if (fileExist("server.config.json")) {
        string jsonFile = readFile("server.config.json");

        char szBuf[jsonFile.length()];
        // .copy('куда копировать', 'до какого номера символа', ['от какого номера символа'])
        int nLength = jsonFile.copy(szBuf, jsonFile.length()-1);
        szBuf[nLength] = '\0'; // завершаем строку в буфере
        
        const char* json = szBuf;

        Document document;
        document.Parse(json);

        Value& f = document[field.c_str()];
        response = f.GetString();

    }

    // char szBuf_2[response.length()];
    // int nLength_2 = response.copy(szBuf_2, response.length());
    // szBuf_2[nLength_2] = '\0';
    
    
    return response;

}


bool fileExist(string filePath) {
    bool isExist = false;
    ifstream fin(filePath.c_str());
 
    if(fin.is_open())
        isExist = true;
 
    fin.close();
    return isExist;
}


// const char *strToChar(string Str) {
//     char szBuf[Str.length()];
//     int nLength = Str.copy(szBuf, Str.length());
//     szBuf[nLength] = '\0';
//     return szBuf;
// }