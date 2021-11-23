all: client server

client: src/client.c
	gcc -std=c11 -o build/client.exe src/client.c

server: src/server.c
	gcc -std=c11 -o build/server.exe src/server.c

clean:
	rm -rf build/*