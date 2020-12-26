// Генерация ключей алгоритма RSA

#include <stdio.h>

#include <openssl/rsa.h>

#include <openssl/pem.h>

/* Имена ключевых файлов */

#define PRIVAT "./privat.key"

#define PUBLIC "./public.key"

int main()

{

    /* указатель на структуру для хранения ключей */

    RSA * rsa = NULL;

    unsigned long bits = 2048; /* длина ключа в битах */

    FILE *priv_key_file = NULL, *pub_key_file = NULL;

    /* контекст алгоритма шифрования */

    const EVP_CIPHER *cipher = NULL;

    priv_key_file = fopen(PRIVAT, "wb");

    pub_key_file = fopen(PUBLIC, "wb");

/* Генерируем ключи */

    rsa = RSA_generate_key(bits, RSA_F4, NULL, NULL);

/* Формируем контекст алгоритма шифрования */

    OpenSSL_add_all_ciphers();

    cipher = EVP_get_cipherbyname("bf-ofb");

/* Получаем из структуры rsa открытый и секретный ключи и сохраняем в файлах.

 * Секретный ключ шифруем с помощью парольной фразы «hello»

 */

    PEM_write_RSAPrivateKey(priv_key_file, rsa, cipher,

    NULL, 0, NULL, (void *)"hello");

    PEM_write_RSAPublicKey(pub_key_file, rsa);

/* Освобождаем память, выделенную под структуру rsa */

    RSA_free(rsa);

    return 0;

}