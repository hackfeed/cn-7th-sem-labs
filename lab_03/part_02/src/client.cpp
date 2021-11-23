#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

#define MSG_LEN 1024 * 1024
#define SOCKET_ADDRESS "localhost"
#define SOCKET_PORT 8888

#define BUFLEN 1024 * 1024

std::string generate_get(std::string filename, std::string username)
{
    const std::string version = "HTTP/1.1";
    return "GET /" + filename + " " + version + "\r\n" + "User: " + username;
}

int main(void)
{
    srand(time(NULL));
    char message[MSG_LEN], username[MSG_LEN];

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        return sock;
    }

    struct hostent *host = gethostbyname(SOCKET_ADDRESS);
    if (!host)
    {
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCKET_PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        return EXIT_FAILURE;
    }

    std::cout << "Enter username: ";
    if (scanf("%s", username) != 1)
    {
        return EXIT_FAILURE;
    }

    std::cout << "Enter an URL: ";
    if (scanf("%s", message) != 1)
    {
        return EXIT_FAILURE;
    }

    if (!strcmp(message, "disconnect"))
    {
        std::cout << "Disconnected" << std::endl;
        close(sock);
        return EXIT_SUCCESS;
    }

    std::string request = generate_get(message, username);
    if (send(sock, request.c_str(), request.length(), 0) < 0)
    {
        return EXIT_FAILURE;
    }

    std::cout << "Client has send a get request, waiting for response" << std::endl;

    char buf[BUFLEN];
    if (recv(sock, buf, BUFLEN, 0) < 0)
    {
        return EXIT_FAILURE;
    }

    std::cout << "Client recieved an answer:" << std::endl
              << std::endl
              << buf;

    close(sock);

    return EXIT_SUCCESS;
}