all:
	g++ -O3 -s smtpcli.cpp -l ssl -l crypto -l ws2_32

clean:
	del -f a.exe

s:
	g++ -O3 -s smtpcli.cpp -o smtpcli.exe -l ssl -l crypto -l ws2_32

g:
	g++ -O3 -s myLibs/get.cpp -o myLibs/get.exe -l ws2_32

p:
	g++ -O3 -s myLibs/parse_url.cpp -o myLibs/parse_url.exe

ip:
	g++ -O3 -s myLibs/getIp.cpp -o myLibs/getIp.exe -l ws2_32


rsa:
	g++ -O3 -s rsa_generate.cpp -o rsa_generate.exe

c:
	g++ -O3 -s _curl.cpp -o _curl.exe -l curl

b:
	g++ -O3 -s base64.cpp -o base64.exe


ssl:
	g++ -O3 -s tests/ssl/sslcli.cpp -o tests/ssl/sslcli.exe -l ssl -l crypto -l ws2_32