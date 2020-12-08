#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <sys/socket.h>

// http://localhost:8000

#define SERVER_PORT    "8000"
#define MAX_CONNECTION 1000

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


int main()
{
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
  //char client_ip
  while(1)
  {
    socklen_t s_size = sizeof(client_addr);
    client_d = accept(sock, (struct sockaddr*)&client_addr, &s_size);

    if(client_d == -1)
    {
      fprintf(stderr, "error accept\n");
      return -1;
    }

    char ip[INET6_ADDRSTRLEN];
    inet_ntop(client_addr.ss_family, get_client_addr((struct sockaddr *)&client_addr), ip, sizeof ip);
    printf("server: got connection from %s\n", ip);

    // read
    http_request(client_d);

    close(client_d);
  }

  return 0;
}


void *get_client_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int create_socket(const char *apstrPort)
{
  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *p;

  memset(&hints, 0, sizeof(hints));

  // IPv4 or IPv6
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags    = AI_PASSIVE;

  int r = getaddrinfo(NULL, apstrPort, &hints, &servinfo);
  if( r != 0)
  {
    fprintf(stderr, "error getaddrinfo()\n");
    return -1;
  }

  int sock;
  int yes;
  for(p = servinfo; p != NULL; p = p->ai_next)
  {
    sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if(sock == -1)
      continue;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      fprintf(stderr, "error setsockopt\n");
      close(sock);
      freeaddrinfo(servinfo); // all done with this structure
      return -2;
    }

    if(bind(sock, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sock);
      continue;
    }
    break;
  }

  freeaddrinfo(servinfo); // all done with this structure

  if(p == NULL)
  {
    fprintf(stderr, "failed to find address\n");
    return -3;
  }

  if(listen(sock, MAX_CONNECTION) == -1)
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




