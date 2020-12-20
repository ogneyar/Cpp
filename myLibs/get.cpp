#include "get.h"


int main() {    
    
    // char url[] = "http://localhost:8000";    
    char url[] = "http://ip.jsontest.com/";
    // char url[] = "http://headers.jsontest.com/";    
    
    cout << "Start parser url. \nUrl: " << url << endl << endl; 
    
    char * response = get( url );
    cout << "response: " << response << endl;

    system("pause");

    return 0;
}

