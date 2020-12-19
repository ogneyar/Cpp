all:
	g++ -O3 -s getMethod.cpp -l ws2_32

clean:
	del -f a.exe

g:
	g++ -O3 -s getMethod.cpp -o get.exe
	
get:
	g++ -O3 -s getMethod.cpp -o get.exe -l ws2_32

p:
	g++ -O3 -s myLibs/parse_url.cpp -o myLibs/parse_url.exe


