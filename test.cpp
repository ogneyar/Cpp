// #include <windows.h>
// #include <string>
#include <cstring>
#include <iostream>
// #include <typeinfo>	

using namespace std;
 
// HWND hwnd = GetForegroundWindow();
 
 
int main(int argc, char **argv) {
    

    // DialogBoxA( NULL, "NULL", NULL, NULL);
 
    // cout << "argv " << argv[argc-1] << endl;

    if ( argc == 1 ) cout << "There is no necessary set of commands"  << endl  << endl << " --help or -h for help as"  << endl << endl;
    else if ( ( argc == 2 ) && ( ( strcmp(argv[1], "--help") == 0 ) || ( strcmp(argv[1], "-h") == 0 ) ) ) cout << "I can't help you, I'm sorry..." << endl;
    else cout << "I don't understand you, I'm sorry... " << endl << endl;
    

    system("pause");

    return 0;
}