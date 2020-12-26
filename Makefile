all:
	g++ -O3 -s rsa_encrypt.cpp -l ssl -l crypto

clean:
	del -f a.exe


test:
	g++ -O3 -s test.cpp


g:
	g++ -O3 -s myLibs/get.cpp -o myLibs/get.exe -l ws2_32

p:
	g++ -O3 -s myLibs/parse_url.cpp -o myLibs/parse_url.exe

ip:
	g++ -O3 -s myLibs/getIp.cpp -o myLibs/getIp.exe -l ws2_32

b:
	g++ -O3 -s myLibs/base64.cpp -o myLibs/base64.exe
	

ssl:
	g++ -O3 -s tests/ssl/sslcli.cpp -o tests/ssl/sslcli.exe -l ssl -l crypto -l ws2_32

smtp:
	g++ -O3 -s tests/smtp/smtpcli.cpp -o tests/smtp/smtpcli.exe -l ssl -l crypto -l ws2_32

msg:
	g++ -O3 -s tests/msg/msgBox.cpp -o tests/msg/msgBox.exe


rsae:
	g++ -O3 -s rsa_encrypt.cpp -o rsa_encrypt.exe

rsag:
	g++ -O3 -s rsa_generate.cpp -o rsa_generate.exe

curl:
	g++ -O3 -s _curl.cpp -o _curl.exe -l curl



