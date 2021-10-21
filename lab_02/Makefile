all: client server

client: src/client.c include/opts.h include/utils.h
	gcc -I . -std=c11 -o client.exe src/client.c

server: src/server.c include/opts.h include/utils.h
	gcc -I . -std=c11 -o server.exe src/server.c