
all:
	g++ -O3 -s server.cpp -o server.exe -l ws2_32

clean:
	del -f server.exe
	del -f hello.exe
	del -f hell.exe
	del -f ci.exe
	
hello:
	g++ -O3 -s tests/hello.cpp -o tests/hello.cgi

hell:
	g++ -O3 -s tests/hell.c -o tests/hell.exe

f:
	g++ -O3 -s tests/file.cpp -o tests/file.exe
	
m:
	g++ -O3 -s main.c -o main.exe -l wsock32

fcgi:
	g++ -O3 -s tests/FastCGI.cpp -o tests/FastCGI.exe -l fcgi -l pthread

test:
	gcc -O3 -s test.cpp -o test.exe -lpthread
