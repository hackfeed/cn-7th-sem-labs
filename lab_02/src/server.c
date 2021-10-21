#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "include/opts.h"
#include "include/utils.h"

int convert(int num, int base, char *res)
{
    char letters[16] = {"0123456789ABCDEF"};
    int mod = num % base;

    num /= base;
    if (num == 0)
    {
        res[0] = letters[mod];
        return 1;
    }

    int k = convert(num, base, res);
    res[k++] = letters[mod];

    return k;
}

void print_converted(char *strnum)
{
    int n = atoi(strnum);
    char bin[RESLEN] = {};
    char six[RESLEN] = {};

    convert(n, 2, bin);
    convert(n, 6, six);

    printf("DEC: %d\nBIN: %s\nOCT: %o\nHEX: %x\nSIX: %s\n", n, bin, n, n, six);
}

int main(void)
{
    struct sockaddr_in server_addr, client_addr;
    int sock, slen = sizeof(client_addr);
    char buf[MSGLEN];

    printf("Server started\n");

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exabort("error while initializing socket");
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        exabort("error while binding socket to address");
    }

    while (1)
    {
        if (recvfrom(sock, buf, MSGLEN, 0, (struct sockaddr *)&client_addr, &slen) == -1)
        {
            exabort("error while receiving from client");
        }

        if (strcmp(buf, "stop\n") == 0)
        {
            printf("! Received termination signal from %s:%d. Aborting\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            close(sock);
            return 0;
        }

        printf("! Received from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        print_converted(buf);
    }
}
