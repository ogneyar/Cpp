#pragma comment(lib, "ws2_32.lib ssl.lib crypto.lib") // -l ws2_32 -l ssl -l crypto

#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>

#include "myLibs/base64.h"

#define BUF_SIZE 1024

using namespace std;

SSL *ssl;
int sock;


int RecvPacket() {
	int len;
	char buf[BUF_SIZE];
	do {
		len = SSL_read(ssl, buf, BUF_SIZE);
		buf[len] = '\0';
		printf(buf);
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

    printf(buf);

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


int main() {
    WSADATA ws;
    
    SOCKET s;
    struct sockaddr_in addr;
    hostent *d_addr;
    char text[1024];
    memset (&text, 0, sizeof(text));

    // инициализируем сокеты
    if ( WSAStartup ( MAKEWORD( 2, 1 ), &ws ) != 0 ) { 
        printf( "Error in WSAStartup...\n" );
        return 1; 
    }

    // создаем сокет
    s = socket (AF_INET, SOCK_STREAM, 0);
    if ( s == INVALID_SOCKET ) { 
        printf( "Error in socket...\n" );
        return 1; 
    }

    // получаем адрес сервера
    d_addr = gethostbyname ("smtp.mail.ru");
    if ( d_addr == NULL ) {
        printf( "Error in gethostbyname...\n" );
        return 1; 
    }

    // заполняем параметры адреса
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = *( (unsigned long *) d_addr->h_addr );
    addr.sin_port = htons (25);

    // устанавливаем соединение
    if ( SOCKET_ERROR == ( connect ( s, (sockaddr *) &addr, sizeof ( addr ) ) ) ) { 
        printf( "Error in connect...\n" );
        return 1; 
    }

    // ждем ответ от сервера
    recv( s, text, sizeof( text ), 0);
    printf( "Recv - %s", text );

    // приветствуем сервер, узнаём список команд
    strcpy(text,"EHLO smtp.mail.ru\r\n");
    send(s,text,strlen(text),0);
    printf("Send - %s", text);
    // ждем подтверждение от сервера
    recv(s,text,sizeof(text),0);
    printf("Recv - %s", text);


    // предлагаем tls соединение
    strcpy(text,"STARTTLS\r\n");
    send(s,text,strlen(text),0);
    printf("send - %s", text);
    // ждем подтверждение от сервера
    recv(s,text,sizeof(text),0);
    printf( "recv - %s", text );



    // регистрируем доступные шифры SSL / TLS.
	SSL_library_init();
    // дублирующая функция, для приложений, которым необходимо
    // использовать SHA2 в более ранних версиях OpenSSL
	SSLeay_add_ssl_algorithms();
    // загрузка сообщений об ошибках
	SSL_load_error_strings();
    // установка универсального метода (TLS_method) с гибкой версией 
    // версия протокола будет согласована с сервером (TLS1.2, TLS1.3, ...)
	const SSL_METHOD *meth =  TLS_method();
    // основа для функций с поддержкой TLS / SSL
    // настройки базового контекста (ctx)
	SSL_CTX *ctx = SSL_CTX_new( meth );
    // создание новой структуры SSL для соединения
	ssl = SSL_new( ctx );
	if ( !ssl ) {
		printf( "Error creating SSL.\n" );
        log_ssl();
		return -1;
	}
	// получаем дескриптор файла, связанный с объектом SSL
	sock = SSL_get_fd( ssl );
    // соединяем объект SSL с файловым дескриптором
	SSL_set_fd( ssl, s );
    // инициация рукопожатия с сервером
	int err = SSL_connect( ssl );	
	if ( err <= 0 ) {
		printf( "Error creating SSL connection.  err=%x\n", err );
        log_ssl();
		fflush( stdout );
		return -1;
	}
	
	printf ( "SSL connection using %s\n", SSL_get_cipher( ssl ) );




    // приветствуем сервер ещё раз (HELO), или узнаём список команд (EHLO)
    SendPacket( "HELO smtp.mail.ru\r\n" ); // но уже в зашифрованном виде
    RecvPacket();
    // предлагаем авторизоваться выбранным методом
    SendPacket( "AUTH LOGIN\r\n" );
    RecvPacket();

    const char * data;
    char * enc;

    data = "ya13th";
    enc = b64_encode( (const unsigned char *)data, strlen( data ) );
    // отправляем логин в кодировке base64
    SendPacket( enc );
    SendPacket( "\r\n" );
    RecvPacket();
    // сюда необходимо подставить пароль
    data = "********";
    enc = b64_encode( (const unsigned char *)data, strlen( data ) );
    // отправляем пароль в кодировке base64
    SendPacket( enc );
    SendPacket( "\r\n" );
    RecvPacket();


    // начинаем отправлять конверт состоящий из полей
    // MAIL FROM: и RCPT TO: После каждого поля ждем подтверждение

    // сообщаем отправителя
    strcpy( text, "MAIL FROM: ya13th@mail.ru\r\n" );
    SendPacket( text );
    // ждем подтверждение от сервера
    RecvPacket();

    // сообщаем получателя
    strcpy( text, "RCPT TO: hutor_yanin@mail.ru\r\n" );
    SendPacket( text );
    // ждем подтверждение от сервера
    RecvPacket();

    // подаем команду, что готовы начать передачу письма
    strcpy( text, "DATA\r\n" );
    SendPacket( text );
    // ждем подтверждение от сервера
    RecvPacket();

    // передаем заголовок

    // от кого письмо
    strcpy( text, "FROM: Ogneyar <ya13th@mail.ru>\r\n" );
    SendPacket( text );
    // кому письмо
    strcpy( text, "TO: Vasiliy Terkin <yaya13th@yandex.ru>\r\n" );
    SendPacket( text );    
    // тема письма
    strcpy( text, "SUBJECT: test\r\n" );
    SendPacket( text );

    // передаем текст письма
    strcpy( text, "Hi!\nIt is a new message for you!\n" );
    SendPacket( text );
    // говорим, что закончили
    strcpy( text, "\r\n.\r\n" );
    SendPacket( text );

    // ждем подтверждение от сервера
    RecvPacket();

    // прощаемся с сервером
    strcpy( text, "QUIT" );
    SendPacket( text );

    // закрываем сокет
    closesocket( s );
    WSACleanup();

    return 0;
}