#ifdef _WIN64
    #define OS_BIT 64
#else
    #ifdef _WIN32
    #define OS_BIT 32
    #endif
#endif

// #define _CRT_SECURE_NO_WARNINGS // для возможности использования scanf
// #include <stdlib.h> // для перехода на русский язык
#include <stdio.h>


void test(char * str);


int main() {
    
    printf("\r\n");
    printf("-= C =-  win%d\r\n\r\n", OS_BIT);
    printf("\r\n");

    printf("Hello :)\r\n");
    
    char str[] = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ " // 208 177 - 208 149, 208 129, 208 150 - 208 175, 32 (0xD090 - 0xD095, 0xD081, 0xD096 - 0xD0AF, 0x20)
        "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"//208 176 - 208 181, 209 145, 208 182 - 208 191, 209 128 - 209 143 (0xD0B0 - 0xD0B5, 0xD191, 0xD0B6 - 0xD0BF, 0xD180 - 0xD18F)
        "!\"#$%&'()*+,-./" // 33 - 47 (0x21 - 0x2F)
        "0123456789:;<=>?@" // 48 - 64 (0x30 - 0x40)
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // 65 - 90 (0x41 - 0x5A)
        "[\\]^_`" // 91 - 96 (0x5B - 0x60)
        "abcdefghijklmnopqrstuvwxyz" // 97 - 122 (0x61 - 0x7A)
        "{|}~" // 123, 124, 125, 126 (0x7B, 0x7C, 0x7D, 0x7E)
        "№"; // 226 132 150 (0xE28496)

    // test(str);

    // не работает :'(
    FILE * fp = fopen("test.txt", "r");

    fgets(str, 10, fp);

    fclose(fp);

    printf(str);
  
    printf("%c%c%c%c%c %c%c%c %c%c%c\r\n",143,224,165,225,225,237,173,168,170,165,169);
    char y;  
    scanf("%c", &y);

  return 0;
}



void test(char * str) {
    
}
