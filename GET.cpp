#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream> // работа вводом/выводом данных
#include <sstream> // работа со стримами

// #include <windows.h>

using namespace std;

int s;
WSADATA lpWSAData;

const char * http_user_agent = "Search Engine";

static char Rec[2048000];


int main(void) { 
    char url[256] = "http://www.codenet.ru/";

        
    // string for send request to server
    char query[2048];
    struct sockaddr_in ssin;
    struct hostent* hp;
    char strHlp[2048], *pch;
    int PortNum;
    char *http_host;
    char *http_path;

    // ensure that url starts with "HTTP://" or "http://"
    if (memcmp(url,"HTTP://",7)!=0 && memcmp(url,"http://",7)!=0) return(7);

    url+=7;// seek "http://"
    // initialize structure WSADATA lpWSAData defined in 'socket.h'
    if (WSAStartup(MAKEWORD(1,1),&lpWSAData)!=0) return(1);

    // use default port number - 80 if not specified other
    // strHlp - helper string that contains server name and path on the server
    strcpy(strHlp,url);
    pch = strchr((char*)strHlp,':');
    // port not defined in passed URL
    if (pch==0) {
        PortNum = 80;
    }else {// port defined in passed URL
        pch[0] = '\0'; // truncate port from URL
        PortNum = atoi(++pch);
        if(PortNum == 0) PortNum = 80;
    }

    // separate server name from path on server
    pch = strchr((char*)strHlp,'/');
    if (pch!=0) {
        pch[0] = '\0';
        http_path=pch+1;
    }else {
        http_path="";
    }

    // Get Host IP by Host name
    if ((hp = gethostbyname(strHlp)) == 0) 
    return(1);
    http_host = strHlp;

    // clear sockaddr_in ssin structure
    memset ((char *)&ssin, 0, sizeof(ssin));
    ssin.sin_family = AF_INET;
    ssin.sin_addr.s_addr = htonl(INADDR_ANY);
    ssin.sin_port = 0;
    // associate ssin with a socket 's'
    bind (s, (struct sockaddr *)&ssin, sizeof(ssin));

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s==INVALID_SOCKET) return(4);

    ssin.sin_family = AF_INET;
    ssin.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr[0];
    ssin.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr[1];
    ssin.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr[2];
    ssin.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr[3];
    ssin.sin_port = htons(PortNum);

    printf("Conecting to %d.%d.%d.%d...",
    (unsigned char)hp->h_addr[0],
    (unsigned char)hp->h_addr[1],
    (unsigned char)hp->h_addr[2],
    (unsigned char)hp->h_addr[3]);
    // establish a connection with socket 's'
    if (connect(s, (sockaddr *)&ssin, sizeof(ssin)) == -1) 
    return(3);
    printf("Ok\n");

    strcpy(query,"GET /");
    strcat(query,http_path);
    strcat(query," HTTP/1.0\nHost: ");
    strcat(query,http_host);
    strcat(query,"\nUser-agent: ");
    strcat(query,http_user_agent);
    strcat(query,"\nAccept: */*\n\n");

    printf("%s",query);

    // send request
    // if (!SendString(s,query)) return(5);

    int cnt = send(s,query,strlen(query),0);

    if (cnt == SOCKET_ERROR) return(5); 
    else return(0);

    // receive response
   
    char InBuff[2048];
    int cnt;

    strcpy(Rec, "");
    
    while (cnt!=0) {
        // clear InBuff - fill 2048 bytes to NULL
        memset(&InBuff,0,2048);
        // receive a 2048 bytes from 's' sock
        cnt = recv (s, (char*)&InBuff, sizeof(InBuff),0);
        // append it to main buffer 'Rec'
        strcat(Rec,InBuff);
    }
    char* str = (char*)&Rec;

    printf("%s",str);


    // if (err != 0) {
    //     printf("Error #%d: WSABASEERR+%d\n", err, WSAGetLastError()-WSABASEERR);
    // }

    WSACleanup();
}