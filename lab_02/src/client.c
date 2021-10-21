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

int main(void)
{
    struct sockaddr_in server_addr;
    int sock, slen = sizeof(server_addr);
    char buf[MSGLEN];

    printf("Client started\n");

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exabort("error while initializing socket");
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_aton(IP, &server_addr.sin_addr) == 0)
    {
        exabort("error while checking address");
    }

    printf("Input message to be delivered: ");

    fgets(buf, MSGLEN, stdin);
    if (sendto(sock, buf, MSGLEN, 0, (struct sockaddr *)&server_addr, slen) == -1)
    {
        exabort("error while sending message to the server");
    }

    close(sock);

    return 0;
}