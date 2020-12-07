#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

using namespace std;

stringstream response;

string set_charset(string charset) {
  return "; charset=" + charset;
}

void set_content_type(string content_type, string charset) {  
  cout << "Content-Type: " << content_type
       << set_charset(charset) << "\r\n\r\n";  
}

void set_page_title(string title) {
  cout << "  <title>" << title << "</title>\n";
}
void h1_text(string text) {
  cout << "  <h1>"  << text << "</h1>" << "\n";
}

int main() {

  // response << "HTTP/1.1 200 OK\r\n"
  //      << "Version: HTTP/1.1\r\n";

  cout << "HTTP/1.1 200 OK\r\n"
       << "Version: HTTP/1.1\r\n";

  // задаём тип контента и кодировку
  set_content_type("text/html", "utf-8");

  cout << "<!DOCTYPE html>\n"
       << "<html lang=\"ru\">\n"
       << "<head>\n";

  set_page_title("Hello Page");
  
  cout << "</head>\n";
  cout << "<body>\n";

  h1_text("Hell o World!");

  cout << "</body>\n";
  cout << "</html>";

  cin.get();
  return 0;
}

