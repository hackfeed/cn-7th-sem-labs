all: client server

client: src/client.cpp
	g++ -std=c++11 -Iinclude -pthread -o build/client.exe src/client.cpp

server: server.o tpool.o
	g++ -std=c++11 -Iinclude -pthread -o build/server.exe build/server.o build/tpool.o

server.o: src/server.cpp include/tpool.hpp
	g++ -std=c++11 -Iinclude -c -o build/server.o src/server.cpp 

tpool.o: src/tpool.cpp include/tpool.hpp
	g++ -std=c++11 -Iinclude -c -o build/tpool.o src/tpool.cpp 

clean:
	rm -rf build/*