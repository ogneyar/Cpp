server:
	g++ -O3 -s server.cpp -o server.exe -l ws2_32

clean:
	del -f server.exe
	del -f hello.exe
	del -f hell.exe
	del -f ci.exe
	
hello:
	g++ -O3 -s hello.cpp -o hello.exe

hell:
	g++ -O3 -s hell.c -o hell.exe

f:
	g++ -O3 -s file.cpp -o file.exe
	