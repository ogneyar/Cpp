all:
	g++ -O3 -s sslcli.cpp -l ssl -l crypto -l ws2_32

clean:
	del -f a.exe

ssl:
	g++ -O3 -s sslcli.cpp -o sslcli.exe -l ssl -l crypto -l ws2_32