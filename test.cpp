#include <windows.h>
#include <string>
#include <iostream>

using namespace std;
 
HWND hwnd = GetForegroundWindow();
 
 
int main() {
    

    DialogBoxA( NULL, "NULL", NULL, NULL);
 
    cout << "resp2" << endl;
    

    system("pause");

    return 0;
}