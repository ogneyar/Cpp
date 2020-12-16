// Для компов с множественными интерфейсами:

#include <stdio.h>
#include <winsock2.h>

void main()
{
    WSADATA wsaData;
    if (!WSAStartup(WINSOCK_VERSION, &wsaData))
    {
        char chInfo[64];
        if (!gethostname(chInfo,sizeof(chInfo)))
        {
            hostent *sh;
            sh=gethostbyname((char*)&chInfo);
            if (sh!=NULL)
            {
                int   nAdapter = 0;
                while (sh->h_addr_list[nAdapter])
                {
                    struct   sockaddr_in   adr;
                    memcpy(&adr.sin_addr,sh->h_addr_list[nAdapter],sh->h_length);
                    printf("IP is: %s\n",inet_ntoa(adr.sin_addr));
                    nAdapter++;
                }
            }
        }
    }
    else
        printf("Winsock Error!\n");

    WSACleanup();
    return;
}