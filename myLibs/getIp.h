#include <stdio.h>
#include <iostream>
#include <winsock2.h>
// #include <string>


char * GetIP(char *);


using namespace std;

// int main() {

//     char * ip = GetIP((char *)"hutoryanin.ru");

//     cout << "IP: " << ip << endl << endl;

//     system("pause");
//     return 0;
// }
//===========================================================================


char * GetIP(char * host) {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(1,1), &wsaData); // инициализируем socket'ы используя Ws2_32.dll для процесса

    char HostName[1024];
    char *m_HostIP;

    strcpy(HostName, host);

    // gethostname(HostName, 1024); // получаем имя хоста


    if(LPHOSTENT lphost = gethostbyname(HostName)) {// получаем IP хоста, т.е. нашего компа

        // DWORD m_HostIP;
        // m_HostIP = ((LPIN_ADDR)lphost->h_addr)->s_addr; // преобразуем переменную типа LPIN_ADDR в DWORD
        
        m_HostIP = inet_ntoa(*((in_addr*)lphost->h_addr_list[0]));


    }else strcpy(m_HostIP, "Error");

    WSACleanup(); // освобождаем сокеты, т.е. завершаем использование Ws2_32.dll

    return m_HostIP;

}
