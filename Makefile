all:
	g++ -O3 -s getMethod.cpp

clean:
	del -f a.exe

g:
	g++ -O3 -s getMethod.cpp -o get.exe
	
get:
	g++ -O3 -s getMethod.cpp -o get.exe -l ws2_32



