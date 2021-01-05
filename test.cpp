#include <windows.h>
#include <string>
#include <iostream>

using namespace std;
 
HWND hwnd = GetForegroundWindow();
 
 
int main(int argc, char **argv) {
    

    // DialogBoxA( NULL, "NULL", NULL, NULL);
 
    // cout << "argc " << argc << endl;
    // cout << "argv " << argv[2] << endl;
    // cout << "argv " << argv[argc-1] << endl;

    if (argc == 1) cout << "There is no necessary set of commands"  << endl  << endl << " --help or -h for help as"  << endl << endl;
    else if ( ( argc == 2 ) && ( ( argv[1] == "--help" ) || ( argv[1] == "-h" ) ) ) cout << "I can't help you, I'm sorry..." << endl;
    else cout << "I don't understand you, I'm sorry... " << endl << endl;
    

    system("pause");

    return 0;
}