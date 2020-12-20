all:
	g++ -O3 -s smtpcli.cpp -l ws2_32

clean:
	del -f a.exe

s:
	g++ -O3 -s smtpcli.cpp -o smtpcli.exe -l ws2_32

g:
	g++ -O3 -s myLibs/get.cpp -o myLibs/get.exe -l ws2_32

p:
	g++ -O3 -s myLibs/parse_url.cpp -o myLibs/parse_url.exe

ip:
	g++ -O3 -s myLibs/getIp.cpp -o myLibs/getIp.exe -l ws2_32
