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
    test(str);
  
    printf("%c%c%c%c%c %c%c%c %c%c%c\r\n",143,224,165,225,225,237,173,168,170,165,169);
    char y;  
    scanf("%c", &y);

  return 0;
}



void test(char * str) {
    char one[] = "А"; 

    if ((str[0] & 0x00ff) == (one[0] & 0x00ff)) printf("Rusian text\r\n");
    if (((str[0] & 0x00ff) == (one[0] & 0x00ff)) && ((str[1] & 0x00ff) == (one[1] & 0x00ff))) printf("first symbol equal %c\r\n", 128);
    else printf("first symbol NOT equal %c\r\n", 128);
  
    printf("\r\n");

    printf("First: \r\n");
    printf("%d", str[0] & 0x00ff);
    printf("\r\n");
    printf("Second: \r\n");
    printf("%d", str[1] & 0x00ff);
    printf("\r\n");
  

    printf("\r\n-----------------\r\n");
    
    int i = 0;
    while(str[i]) {
        if ( ((str[i] & 0x00ff) == 208) || ((str[i] & 0x00ff) == 209) ) {
            // НИЧЕГО НЕ НАДО ДЕЛАТЬ
            // printf("%d ",(str[i] & 0x00ff));
            // printf("(%02X) ",(str[i] & 0x00ff));
        }else if ( 
            ( 
                ( str[i] & 0x00ff ) == 226 
            ) || (
                ( 
                    ( str[i] & 0x00ff ) == 132 
                ) && ( 
                    ( str[i-1] & 0x00ff ) == 226 
                ) 
            ) 
        ) {
            // НИЧЕГО НЕ НАДО ДЕЛАТЬ
            // printf("%d ",(str[i] & 0x00ff));
            // printf("(0x%02X) ",(str[i] & 0x00ff));
        }else if ( ((str[i-1] & 0x00ff) == 208) || ((str[i-1] & 0x00ff) == 209) ) {
            printf("%d %d ", (str[i-1] & 0x00ff), (str[i] & 0x00ff));
            printf("(0x%02X%02X) ", (str[i-1] & 0x00ff), (str[i] & 0x00ff));
            if ((str[i-1] & 0x00ff) == 208) {
                if ((str[i] & 0xff) == 129) printf("- %c - %d (0x%02X)\r\n", 240, 240, 240); // Ё
                else printf("- %c - %d (0x%02X)\r\n", (str[i] & 0xff)-16, (str[i] & 0xff)-16, (str[i] & 0xff)-16);
            }else { // если 209
                if ((str[i] & 0xff) == 145) printf("- %c - %d (0x%02X)\r\n", 241, 241, 241); // ё
                else printf("- %c - %d (0x%02X)\r\n", (str[i] & 0xff)+96, (str[i] & 0xff)+96, (str[i] & 0xff)+96);
            }
        }else if ( 
            (
                ( 
                    ( str[i] & 0x00ff ) == 150 
                ) && ( 
                    ( str[i-1] & 0x00ff ) == 132 
                ) && ( 
                    ( str[i-2] & 0x00ff ) == 226 
                ) 
            ) 
        ) {
            printf("%d %d %d ", (str[i-2] & 0x00ff), (str[i-1] & 0x00ff), (str[i] & 0x00ff));
            printf("(0x%02X%02X%02X) ", (str[i-2] & 0x00ff), (str[i-1] & 0x00ff), (str[i] & 0x00ff));
            printf("- %c\r\n", 252);
        }else {
            printf("%d ",(str[i] & 0x00ff));
            printf("(0x%02X) ",(str[i] & 0x00ff));
            if ((str[i] & 0x00ff) == 32) printf("- %c%c%c%c%c%c%c%c\r\n",34,175,224,174,161,165,171,34);
            else printf("- %c\r\n",(str[i] & 0x00ff));
        }

        i++;
    }

    printf("\r\n----------------- eng\r\n");

    i = 32;
    while(i<=127) {
        printf("%c ",i);
        i++;
    }
    printf("\r\n\r\n----------------- rus\r\n");
    i = 128;
    while(i<=252) {
        printf("%c ",i);
        if (i == 175) i = 224;
        else if (i == 241) i = 252;
        else i++;
    }
    // printf("%c ",175); // п
    // printf("%c ",224); // р
    // printf("%c ",240); // Ё
    // printf("%c ",241); // ё
    // printf("%c ",252); // №
    // printf("%c ",0b10000000); // А русская (128)

    printf("\r\n\r\n-----------------\r\n");

    printf("\r\n%c\r\n",148);
    printf("%d\r\n", ( (("Ф"[0] & 0x00ff)<<8) | ("Ф"[1] & 0x00ff) ) );
    printf("0x%04X\r\n", ( (("Ф"[0] & 0x00ff)<<8) | ("Ф"[1] & 0x00ff) ) );
    printf("0b1101000010100100\r\n\r\n");
    
}
