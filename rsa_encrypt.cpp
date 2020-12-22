// Шифрование данных по алгоритму RSA

#include <openssl/rsa.h>

#include <openssl/pem.h>

void main(int argc, char **argv)

{

    /* структура для хранения открытого ключа */

    RSA * pubKey = NULL;

    unsigned char *ptext, *ctext;

    FILE * pub_key_file = NULL;

/* Открываем входной и создаем выходной файлы */

    int inf = open(argv[1], O_RDWR);

    int outf = open("./rsa.file",

    O_CREAT|O_TRUNC|O_RDWR, 0600);

/* Считываем открытый ключ */

    pub_key_file = fopen(PUBLIC, "rb");

    pubKey = PEM_read_RSAPublicKey(pub_key_file, NULL, NULL, NULL);

/* Определяем длину ключа */

    int key_size = RSA_size(pubKey);

    ptext = malloc(key_size);

    ctext = malloc(key_size);

/* Шифруем содержимое входного файла */

    while(1) {

    inlen = read(inf, ptext, key_size - 11);

    if(inlen <= 0) break;

    outlen = RSA_public_encrypt(inlen, ptext, ctext, pubKey, RSA_PKCS1_PADDING);

    if(outlen != RSA_size(pubKey)) exit(-1);

    write(outf, ctext, outlen);

    }

}