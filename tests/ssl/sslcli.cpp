
#pragma comment(lib, "ws2_32.lib ssl.lib crypto.lib") // -l ws2_32 -l ssl -l crypto

#include <WinSock2.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <openssl/ssl.h>
#include <openssl/err.h>

// #include "myLibs/getIp.h"

#define BUF_SIZE 1024

using namespace std;

SSL *ssl;
int sock;

// char * ip = getIp("google.ru");

int RecvPacket() {
	int len;
	char buf[BUF_SIZE];
	do {
		len = SSL_read(ssl, buf, BUF_SIZE);
		buf[len] = '\0';
		printf(buf);
		// cout << endl << "len(buf): " << len << endl;
	} while (len == BUF_SIZE);
	if (len < 0) {
		int err = SSL_get_error(ssl, len);
		if (err == SSL_ERROR_WANT_READ)
			return 0;
		if (err == SSL_ERROR_WANT_WRITE)
			return 0;
		if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
			return -1;
	}

	return 1;
}

int SendPacket(const char *buf) {
	int len = SSL_write(ssl, buf, strlen(buf));
	if (len < 0) {
		int err = SSL_get_error(ssl, len);
		switch (err) {
			case SSL_ERROR_WANT_WRITE:
			return 0;
			case SSL_ERROR_WANT_READ:
			return 0;
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
			default:
			return -1;
		}
	}

	return 1;
}

void log_ssl() {
	int err;
	while (err = ERR_get_error()) {
		char *str = ERR_error_string(err, 0);
		if (!str) return;
		printf(str);
		printf("\n");
		fflush(stdout);
	}
}


int main(int argc, char *argv[]) {
	// служебная структура для хранение информации о реализации Windows Sockets
    WSADATA wsaData;
    // старт использования библиотеки сокетов процессом (подгружается Ws2_32.dll)
    int result = WSAStartup(MAKEWORD(2, 1), &wsaData);
    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << "\n";
        return -1;
    }

	int s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	if (!s) {
		printf("Error creating socket.\n");
		return -1;
	}

	struct sockaddr_in sa;
	memset (&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	// можно так узнать ip-адресс
    sa.sin_addr.s_addr = inet_addr(inet_ntoa(*((in_addr*)gethostbyname("google.ru")->h_addr_list[0])));
	// а можно и так
	// sa.sin_addr.s_addr = inet_addr(ip);
	sa.sin_port = htons (443);

	if (connect(s, (struct sockaddr *)&sa, sizeof(sa))) {
		printf("Error connecting to server.\n");
		return -1;
	}
	
	cout << "Server connect.\n";	

	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	const SSL_METHOD *meth =  SSLv23_method();
	SSL_CTX *ctx = SSL_CTX_new (meth);

	ssl = SSL_new (ctx);
	if (!ssl) {
		printf("Error creating SSL.\n");
		log_ssl();
		return -1;
	}

	cout << "Creating SSL.\n";	
	
	sock = SSL_get_fd(ssl);
	SSL_set_fd(ssl, s);
	int err = SSL_connect(ssl);
	
	if (err <= 0) {
		printf("Error creating SSL connection.  err=%x\n", err);
		log_ssl();
		fflush(stdout);
		return -1;
	}
	
	printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

	char *request = (char *)"GET https://www.google.ru/intl/en/about/company/facts/ HTTP/1.1\r\n\r\n";
	SendPacket(request);
	RecvPacket();

	// char quit[] = "QUIT";
	// SSL_write(ssl, quit, strlen(quit));
	// SendPacket(quit);

	closesocket( s );
    WSACleanup();
	
	return 0;
}