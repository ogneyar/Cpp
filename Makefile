all:
	g++ -O3 -s hello.cpp -o hello.exe

clean:
	del -f hello.exe