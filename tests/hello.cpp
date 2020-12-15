#ifdef _WIN64
    #define OS_BIT 64
#else
    #ifdef _WIN32
    #define OS_BIT 32
    #endif
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

// библиотека си для printf
#include <stdio.h>

using namespace std;

stringstream response;
stringstream response_body;

string set_charset(string);
string set_content_type(string, string);
string set_page_title(string);
string h1_text(string);

string string_content;
string *string_content2;

int main() {
     
  // response << "-= C++ =-  " << "win" << OS_BIT << "\r\n\r\n" << "HTTP/1.1 200 OK\r\n"  
  //   << "Version: HTTP/1.1\r\n"
  //   // задаём тип контента и кодировку
  //   << set_content_type("text/html", "utf-8")

  response_body
    << "<!DOCTYPE html>\n"
    << "<html lang=\"ru\">\n"
    << "<head>\n"
    // задаём титул
    << set_page_title("Hello Page")
    << "</head>\n<body>\n"
    // задаём текст заголовка
    << h1_text("Hell o World!")
    << "</body>\n" << "</html>";

  response << "HTTP/1.1 200 OK\r\n"
    << "Version: HTTP/1.1\r\n"
    << "Content-Type: text/html; charset=utf-8\r\n"
    << "Content-Length: " << response_body.str().length()
    << "\r\n\r\n"
    << response_body.str();

  cout << response.str().c_str();

  cin.get();
  return 0;
}


string set_content_type(string content_type, string charset) {  
  return "Content-Type: " + content_type
       + set_charset(charset) + "\r\n\r\n"; 
}

string set_charset(string charset) {
  return "; charset=" + charset;
}

string set_page_title(string title) {
  return "  <title>" + title + "</title>\n";
}

string h1_text(string text) {
  return + "  <h1>" + text + "</h1>" + "\n";
}
