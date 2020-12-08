#ifdef _WIN64
#define OS_BIT 64
#endif // !WIN64

#ifdef _WIN32
#define OS_BIT 32
#endif // !WIN32

// #define _CRT_SECURE_NO_WARNINGS // для возможности использования scanf
#include <stdlib.h> // для перехода на русский язык
#include <stdio.h>

void set_content_type(char *content_type, char charset);
char set_page_title(char title);
char h1_text(char text);


int main() {
  
  printf("-= C =-  win%d\r\n\r\n", OS_BIT);  

  printf("HTTP/1.1 200 OK\r\nVersion: HTTP/1.1\r\n");  

  printf("Content-Type: ");
    
  printf("text/html");

  printf("; charset=");
    
  printf("utf-8");
  
  printf("\r\n\r\n"); 

  printf("<!DOCTYPE html>\n<html lang=\"ru\">\n<head>\n");

  printf("  <title>Hello Page</title>\n");

  printf("</head>\n<body>\n");
  
  printf("  <h1>Hell o World!</h1>\n");

  printf("</body>\n</html>");
  
  char y;  
  scanf("%c", &y);

  return 0;
}

