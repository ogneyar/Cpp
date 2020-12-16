all:
	g++ -O3 -s sniffer.cpp -l ws2_32

clean:
	del -f a.exe

s:
	g++ -O3 -s sniffer.cpp -o sniffer.exe -l ws2_32
	
sniffer:
	g++ -O3 -s sniffer.cpp -o sniffer.exe -l ws2_32



