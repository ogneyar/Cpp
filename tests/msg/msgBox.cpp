#include <windows.h>
#include <string>
#include <iostream>

using namespace std;
 
HWND hwnd = GetForegroundWindow();

// string text;
// string capt;
// MessageBox(NULL,text.c_str(),capt.c_str(),MB_OK);

 
LPCTSTR str1 = (LPCTSTR)"Programina";
LPCTSTR str2 = (LPCTSTR)"Welcome to C++";
 
int main() {
    
    // int resp = MessageBox( hwnd, str1, str2, MB_ABORTRETRYIGNORE );
    int resp = MessageBox( hwnd, str1, str2, MB_CANCELTRYCONTINUE );
    // int resp = MessageBox( hwnd, str1, str2, MB_HELP );
    // int resp = MessageBox( hwnd, str1, str2, MB_OK );
    // int resp = MessageBox( hwnd, str1, str2, MB_OKCANCEL );
    // int resp = MessageBox( hwnd, str1, str2, MB_RETRYCANCEL );
    // int resp = MessageBox( hwnd, str1, str2, MB_YESNO );
    // int resp = MessageBox( hwnd, str1, str2, MB_YESNOCANCEL );
    // int resp = MessageBox( hwnd, str1, str2, MB_ICONEXCLAMATION ); // со звуком и значком ! в треугольнике
    // int resp = MessageBox( hwnd, str1, str2, MB_ICONINFORMATION ); // со звуком и значком i в круге
    // int resp = MessageBox( hwnd, str1, str2, MB_ICONQUESTION ); // со значком ? в круге
    // int resp = MessageBox( hwnd, str1, str2, MB_ICONERROR );  // со звуком и значком Х в круге

    if (resp == IDNO) { // 7
        cout << "What no?" << endl;

    }else if (resp == IDYES) { // 6
        cout << "Realy yes?" << endl;

    }else if (resp == IDABORT) { // 3
        cout << "Why abort?" << endl;

    }else if (resp == IDCANCEL) { // 2 
        cout << "Realy cancel?" << endl;
        
    }else if (resp == IDCONTINUE) { // 11
        cout << "Realy continue?" << endl;
        
    }else if (resp == IDIGNORE) { // 5
        cout << "Why ignore?" << endl;

    }else if (resp == IDOK) { // 1
        cout << "Realy ok?" << endl;
        
    }else if (resp == IDRETRY) { // 4
        cout << "Realy retry?" << endl;

    }else if (resp == IDTRYAGAIN) { // 10
        cout << "Realy try again?" << endl;
        
    }


    return 0;
}