#pragma comment(lib, "Ws2_32.lib")

#include <conio.h>
#include <stdio.h>
#include <winsock2.h>

#include <iostream>

#define MAX_PACKET_SIZE 0x10000
#define SIO_RCVALL 0x98000001
// Буфер для приёма данных
char Buffer[MAX_PACKET_SIZE]; // 64 Kb

//Структура заголовка IP-пакета
typedef struct IPHeader {
    UCHAR iph_verlen; // версия и длина заголовка
    UCHAR iph_tos; // тип сервиса
    USHORT iph_length; // длина всего пакета
    USHORT iph_id; // Идентификация
    USHORT iph_offset; // флаги и смещения
    UCHAR iph_ttl; // время жизни пакета
    UCHAR iph_protocol; // протокол
    USHORT iph_xsum; // контрольная сумма
    ULONG iph_src; // IP-адрес отправителя
    ULONG iph_dest; // IP-адрес назначения
} IPHeader;

char src[1000];
char dest[10];
char ds[15];
unsigned short lowbyte;
unsigned short hibyte;

int main() {
    WSADATA wsadata; // Инициализация WinSock.
    SOCKET s; // Cлущающий сокет.
    char name[128]; // Имя хоста (компьютера).
    HOSTENT* phe; // Информация о хосте.
    SOCKADDR_IN sa; // Адрес хоста
    IN_ADDR sa1; //
    unsigned long flag = 1; // Флаг PROMISC Вкл/выкл.

    // инициализация
    WSAStartup(MAKEWORD(2,2), &wsadata);
    s = socket( AF_INET, SOCK_RAW, IPPROTO_IP );
    gethostname(name, sizeof(name));
    phe = gethostbyname( name );
    ZeroMemory( &sa, sizeof(sa) );
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = ((struct in_addr *)phe->h_addr_list[0])->s_addr;
    bind(s, (SOCKADDR *)&sa, sizeof(SOCKADDR));
    
    // Включение promiscuous mode.
    ioctlsocket(s, SIO_RCVALL, &flag);

    double valuePackages = 0;

    std::cout << "Started..." << std::endl;

    // Бесконечный цикл приёма IP-пакетов.
    while( !_kbhit() ) {
        int count;
        count = recv( s, Buffer, sizeof(Buffer), 0 );
        // обработка IP-пакета
        if( count >= sizeof(IPHeader) ) {
            IPHeader* hdr = (IPHeader *)Buffer;
            //Начинаем разбор пакета...

            strcpy(src,"Package: ");
            CharToOem(src,dest);
            printf(dest);
            // Преобразуем в понятный вид адрес отправителя.
            printf("From ");
            sa1.s_addr = hdr->iph_src;
            printf(inet_ntoa(sa1));

            // Преобразуем в понятный вид адрес получателя.
            printf(" To ");
            sa1.s_addr = hdr->iph_dest;
            printf(inet_ntoa(sa1));

            // Вычисляем протокол. Полный список этих констант
            // содержится в файле winsock2.h
            printf(" Prot: ");
            if(hdr->iph_protocol == IPPROTO_TCP) printf("TCP ");
            if(hdr->iph_protocol == IPPROTO_UDP) printf("UDP ");

            // Вычисляем размер. Так как в сети принят прямой порядок
            // байтов, а не обратный, то прийдётся поменять байты местами.
            printf("Size: ");
            lowbyte = hdr->iph_length>>8;
            hibyte = hdr->iph_length<<8;
            hibyte = hibyte + lowbyte;

            char *str1;
            // itoa(hibyte,str1,10);
            // printf("%s",str1);
            std::cout << hibyte;

            valuePackages += hibyte;

            // Вычисляем время жизни пакета.
            char *str2;
            // itoa(hdr->iph_ttl,str2,10);
            // printf(" TTL:%s", str2);
            std::cout << " TTL: " << hdr->iph_ttl;

            printf("\n");

        }
    }

    closesocket( s );
    WSACleanup();

    double b = valuePackages;
    double kb = b / 1024;
    double mb = kb / 1024;
    double gb = mb / 1024;

    std::cout << std::endl << "Summary: " << b << " b" << std::endl;
    std::cout << std::endl << "Summary: " << kb << " Kb" << std::endl;
    std::cout << std::endl << "Summary: " << mb << " Mb" << std::endl;
    std::cout << std::endl << "Summary: " << gb << " Gb" << std::endl << std::endl;

    system("pause");

    return 0;
}