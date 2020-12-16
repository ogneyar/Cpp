#include <stdio.h>
#include <winsock2.h>


DWORD GetCurentIP(LPSTR);

int main() {
    
	GetCurentIP();


    return 0;
}
//===========================================================================



// Если требуется создать строку из IP, то код будет выглядеть вот так:
DWORD GetCurentIP(LPSTR lpStringWithIP = 0) // lpStringWithIP - поинтер на массив, состоящий из 17 символов
// иначе:
//DWORD GetCurentIP()
//Если надо получить сами числа, то код будет такой:
//DWORD GetCurentIP(BYTE a1, BYTE a2, BYTE a3, BYTE a4)
// BYTE a1, BYTE a2, BYTE a3, BYTE a4 - биты адреса IP
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1,1), &wsaData); // инициализируем socket'ы используя Ws2_32.dll для процесса

    char HostName[1024]; // создаем буфер для имени хоста
    DWORD m_HostIP = 0;

    if(!gethostname(HostName, 1024)) // получаем имя хоста
    {
        if(LPHOSTENT lphost = gethostbyname(HostName)) // получаем IP хоста, т.е. нашего компа
            m_HostIP = ((LPIN_ADDR)lphost->h_addr)->s_addr; // преобразуем переменную типа LPIN_ADDR в DWORD
    }
    WSACleanup(); // освобождаем сокеты, т.е. завершаем использование Ws2_32.dll
	
	// Если требуется создать строку из IP, то код будет выглядеть вот так:
    if (lpStringWithIP) // если поинтер на строку не нулевой
        strcpy(lpStringWithIP, inet_ntoa(*((in_addr*)lphost->h_addr_list[0]))); // то преобразуем IP в строку
	// иначе если НЕ требуется, то закоментить текст выше

	// чтобы получить сами числа:
	//a1=((LPIN_ADDR)lphost->h_addr)->s_b1; // присваиваем значение 1-го байта адреса
    //a2=((LPIN_ADDR)lphost->h_addr)->s_b2; // присваиваем значение 2-го байта адреса
	//a3=((LPIN_ADDR)lphost->h_addr)->s_b3; // присваиваем значение 3-го байта адреса
    //a4=((LPIN_ADDR)lphost->h_addr)->s_b4; // присваиваем значение 4-го байта адреса
    /* // или можно еще так
    a1=lphost->h_addr_list[0];
    a2=lphost->h_addr_list[1];
    a3=lphost->h_addr_list[2];
    a4=lphost->h_addr_list[3]; */

    return m_HostIP;
}
