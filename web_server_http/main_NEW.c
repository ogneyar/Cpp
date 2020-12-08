#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// platform detection
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
  #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
  #define PLATFORM PLATFORM_MAC
#else
  #define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
  #include <winsock2.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
  #include <netdb.h>
  #include <sys/socket.h>
  // эти библиотеки верояно не пригодятся
  #include <netinet/in.h>
  #include <fcntl.h>  
#endif

#if PLATFORM == PLATFORM_WINDOWS
  #pragma comment( lib, "wsock32.lib" )
#endif


// http://localhost:8000

#define SERVER_PORT    "8000"
#define MAX_CONNECTION 1000

#define INET6_ADDRSTRLEN 255

/*
Client
    socket()
  ? bind()    ?
    connect()

    ----------------
    write()   | read()
    send()    | recv()
    sendto()  | recvfrom()
    writev()  | readv()
    sendmsg() | recvmsg()
    ----------------

    close()

Server
    socket()
    bind()
    listen()
    accept()

    ----------------
    write()   | read()
    send()    | recv()
    sendto()  | recvfrom()
    writev()  | readv()
    sendmsg() | recvmsg()
    ----------------

    close()
*/

typedef enum
{
  eHTTP_UNKNOWN = 0
 ,eHTTP_CONNECT
 ,eHTTP_DELETE
 ,eHTTP_GET
 ,eHTTP_HEAD
 ,eHTTP_OPTIONS
 ,eHTTP_PATCH
 ,eHTTP_POST
 ,eHTTP_PUT
 ,eHTTP_TRACE
}eHTTPMethod;

typedef struct
{
  eHTTPMethod type;
  char        path[255];
}sHTTPHeader;

void *get_client_addr(struct sockaddr *);
int create_socket(const char *);

void http_request(int);
void parse_http_request(const char*, sHTTPHeader *);
void send_message(int, const char*);
void send_404(int);

inline void InitializeSockets();
inline void ShutdownSockets();


int main()
{
  InitializeSockets();

  int sock;

  sock = create_socket(SERVER_PORT);
  if(sock < 0)
  {
    fprintf(stderr, "error create socket\n");
    return -1;
  }

  printf("server created!\n");

  struct sockaddr_storage client_addr;
  int client_d;

  #if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
  #endif

  //char client_ip
  while(1)
  {
    socklen_t s_size = sizeof(client_addr);
    client_d = accept(sock, (struct sockaddr*)&client_addr, &s_size);

    if(client_d < 0)
    {
      fprintf(stderr, "error accept\n");
      return -1;
    }

    // char ip[INET6_ADDRSTRLEN];
    // inet_ntop(client_addr.ss_family, get_client_addr((struct sockaddr *)&client_addr), ip, sizeof ip);
    // printf("server: got connection from %s\n", ip);

    // read
    http_request(client_d);
  
    #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      close( client_d );
    #elif PLATFORM == PLATFORM_WINDOWS
      closesocket( client_d );
     #endif
  }

  ShutdownSockets();
  return 0;
}

// --------------------------

// void *get_client_addr(struct sockaddr *sa)
// {
//   if (sa->sa_family == AF_INET)
//   {
//     return &(((struct sockaddr_in*)sa)->sin_addr);
//   }

//   return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

int create_socket(const char *apstrPort)
{
  
  int sock;
  int yes;

  sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

  if (sock < 0) return -1;

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
  {
    fprintf(stderr, "error setsockopt\n");

    #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      close( sock );
    #elif PLATFORM == PLATFORM_WINDOWS
      closesocket( sock );
    #endif

    return -2;
  }
  
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( (unsigned short) apstrPort );

  if ( bind( sock, &address, sizeof(address) ) < 0 )
  {
      printf( "failed to bind socket\n" );

      #if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
        close( sock );
      #elif PLATFORM == PLATFORM_WINDOWS
        closesocket( sock );
      #endif

      return -1;
  }

  if (listen(sock, MAX_CONNECTION) < 0)
  {
    fprintf(stderr, "error listen\n");
    return -4;
  }

  return sock;
}


void http_request(int aSock)
{
  const int request_buffer_size = 65536;
  char      request[request_buffer_size];

  int bytes_recvd = recv(aSock, request, request_buffer_size - 1, 0);

  if (bytes_recvd < 0)
  {
    fprintf(stderr, "error recv\n");
    return;
  }
  request[bytes_recvd] = '\0';

  printf("request:\n%s\n",request);

  sHTTPHeader req;
  parse_http_request(request, &req);

  if(req.type == eHTTP_GET)
  {
    send_message(aSock, "sensor 1: 10<br> sensor 2: 20<br><a href=\"http://cppprosto.blogspot.com/2017/09/blog-post_23.html\">external</a><br><a href=\"internal\">internal</a>");
  }
  else
  {
    send_404(aSock);
  }
}

void parse_http_request(const char *apstrRequest, sHTTPHeader *apHeader)
{
  int  type_length = 0;
  char type[255]   = {0};
  int  index = 0;

  apHeader->type = eHTTP_UNKNOWN;

  sscanf(&apstrRequest[index], "%s", type);
  type_length = strlen(type);

  if(type_length == 3)
  {
    if(type[0] == 'G' && type[1] == 'E' && type[2] == 'T')
      apHeader->type = eHTTP_GET;

    index += type_length + 1;
    sscanf(&apstrRequest[index], "%s", apHeader->path);
  }
}

void send_message(int aSock, const char *apstrMessage)
{
  char buffer[65536] = { 0 };

  strcat(buffer, "HTTP/1.1 200 OK\n\n");
  strcat(buffer, "<h1>");
  strcat(buffer, apstrMessage);
  strcat(buffer, "</h1>");

  int len = strlen(buffer);
  send(aSock, buffer, len, 0);
}

void send_404(int aSock)
{
  const char *buffer = "HTTP/1.1 404 \n\n";
  int len = strlen(buffer);
  send(aSock, buffer, len, 0);
}




// server: got connection from 127.0.0.1
// request:
// GET /index.html HTTP/1.1
// Host: localhost:3490
// Connection: keep-alive
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/68.0.3440.75 Chrome/68.0.3440.75 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8
// Accept-Encoding: gzip, deflate, br
// Accept-Language: en-US,en;q=0.9,ru;q=0.8



inline void InitializeSockets()
{
    #if PLATFORM == PLATFORM_WINDOWS
      WSADATA WsaData;
      // return WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR;
      WSAStartup( MAKEWORD(2,2), &WsaData );

    // #else
    //   return true;
    #endif
}

inline void ShutdownSockets()
{
    #if PLATFORM == PLATFORM_WINDOWS
      WSACleanup();
    #endif
}

