// Дешифрование файла, зашифрованного по RSA-алгоритму

#include <openssl/rsa.h>

#include <openssl/pem.h>

void main(int argc, char **argv)

{

    RSA *privKey = NULL;

    FILE *priv_key_file;

    unsigned char *ptext, *ctext;

/* Открываем входной и создаем выходной файл */

    inf = open(argv[1], O_RDWR);

    outf = open("./test.rsa", O_CREAT|O_TRUNC|O_RDWR, 0600);

/* Открываем ключевой файл и считываем секретный ключ */

    priv_key_file = fopen(PRIVAT, "rb");

    privKey = PEM_read_RSAPrivateKey(priv_key_file, NULL, NULL, NULL);

/* Определяем размер ключа */

    key_size = RSA_size(privKey);

    ctext = malloc(key_size);

    ptext = malloc(key_size);

/* Дешифруем файл */

    while(1) {

    inlen = read(inf, ctext, key_size);

    if(inlen <= 0) break;

    outlen = RSA_private_decrypt(inlen, ctext, ptext, privKey, RSA_PKCS1_PADDING);

    if(outlen < 0) exit(0);

    write(outf, ptext, outlen);

    }

}