#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;


char * getIp(char * host) {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(1,1), &wsaData); // инициализируем socket'ы используя Ws2_32.dll для процесса

    char HostName[1024];
    char *m_HostIP;

    if (host == "local") gethostname(HostName, 1024); // получаем имя хоста
    else strcpy(HostName, host);

    if(LPHOSTENT lphost = gethostbyname(HostName)) {// получаем IP хоста, т.е. нашего компа
        
        m_HostIP = inet_ntoa(*((in_addr*)lphost->h_addr_list[0]));

    }else strcpy(m_HostIP, "Error");

    WSACleanup(); // освобождаем сокеты, т.е. завершаем использование Ws2_32.dll

    return m_HostIP;

}
