all:
	g++ -O3 -s hello.cpp -o hello.exe

clean:
	del -f hello.exe

c:
	g++ -O3 -s hell.c -o hell.exe