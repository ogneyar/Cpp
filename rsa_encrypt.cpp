// Шифрование данных по алгоритму RSA
#include <fcntl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <iostream>

#define PRIVAT "./privat.key"
#define PUBLIC "./public.key"

#define INPUT "./input.file"

using namespace std;

/* структура для хранения открытого ключа */
RSA * pubKey = NULL;
int inf;
int outf;
FILE * pub_key_file = NULL;
int key_size;
 unsigned char *ptext, *ctext;
int inlen;
int outlen;


int main(int argc, char **argv)
{
    setlocale(0,"");

    wcout << L"Начало программы " << L"тут" << endl << endl;

/* Открываем входной и создаем выходной файлы */
    inf = open( INPUT, O_RDWR );
    wcout << L"Открыл файл " << L"INPUT" << endl << endl;

    outf = open( "./rsa.file", O_CREAT|O_TRUNC|O_RDWR, 0600 );
    wcout << L"Открыл файл " << L"rsa.file" << endl << endl;

/* Считываем открытый ключ */
    pub_key_file = fopen(PUBLIC, "rb");
    pubKey = PEM_read_RSAPublicKey(pub_key_file, NULL, NULL, NULL);
    wcout << L"Считываем открытый ключ " << L"PUBLIC" << endl << endl;

/* Определяем длину ключа */
    key_size = RSA_size(pubKey);
    wcout << L"Определяем длину ключа " << L"RSA_size" << endl << endl;

    ptext = (unsigned char *)malloc(key_size);
    ctext = (unsigned char *)malloc(key_size);

/* Шифруем содержимое входного файла */
    while(1) {
        wcout << L"Попал в цикл while" << L"(1)" << endl << endl;
        inlen = read(inf, ptext, key_size - 11);
        wcout << L"Прочёл файл " << L"input.file" << endl << endl;
        if(inlen <= 0) break;
        wcout << "Длина больше " << "нуля" << endl << endl;
        outlen = RSA_public_encrypt(inlen, ptext, ctext, pubKey, RSA_PKCS1_PADDING);
        wcout << L"Сработала функция " << L"RSA_public_encrypt" << endl << endl;
        if(outlen != RSA_size(pubKey)) return -1;
        wcout << L"Сработала функция " << L"RSA_size(pubKey)" << endl << endl;
        write(outf, ctext, outlen);
        wcout << L"Записал в файл " << L"rsa.file" << endl << endl;
    }

    system("pause");

    return 0;
}