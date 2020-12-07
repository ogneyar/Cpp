#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

stringstream response;

string set_charset(string charset);
string set_content_type(string content_type, string charset);
string set_page_title(string title);
string h1_text(string text);

string string_content;
string *string_content2;

int main() {

  // string_content = "text/html";
  // string_content2 = &string_content;

  // cout << endl;
  // cout << "content_type: " << string_content << endl;
  // cout << "&content_type: " << &string_content << endl;
  // cout << endl;

  // cout << endl;
  // cout << "content_type2: " << string_content2 << endl;
  // cout << "&content_type2: " << *string_content2 << endl;
  // cout << endl;

  response << "HTTP/1.1 200 OK\r\n"
    << "Version: HTTP/1.1\r\n"
    // задаём тип контента и кодировку
    << set_content_type("text/html", "utf-8")
    << "<!DOCTYPE html>\n"
    << "<html lang=\"ru\">\n"
    << "<head>\n"
    // задаём титул
    << set_page_title("Hello Page")
    << "</head>\n<body>\n"
    // задаём текст заголовка
    << h1_text("Hell o World!")
    << "</body>\n" << "</html>";

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
