#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define SOCKET_PORT 8888
#define BUF_SIZE 1024 * 1024

void write_file(FILE *fp, int sockfd)
{
    int n = 0;
    char data[BUF_SIZE] = {0};

    while (data[n] != EOF)
    {
        n = recv(sockfd, data, BUF_SIZE, 0);
        if (data[n - 1] == EOF)
        {
            n--;
        }
        fwrite(data, sizeof(char), n, fp);
    }
}

int main(void)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        perror("socket call error");
        return errno;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SOCKET_PORT);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    if (connect(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect call error");
        return errno;
    }

    char srvmsg[BUF_SIZE];
    size_t bytes = recv(sfd, &srvmsg, BUF_SIZE, 0);
    if (!bytes)
    {
        fprintf(stderr, "Failed to receive message from server (with fd = %d)\n", sfd);
        return errno;
    }
    else
    {
        srvmsg[bytes] = '\0';
        printf("Server files:\n%s\n", srvmsg);
    }

    for (;;)
    {
        char filename[BUF_SIZE];
        printf("Input filename to get it from the server (or type 'disconnect' to disconnect): ");
        scanf("%s", filename);
        if (strcmp(filename, "disconnect") == 0)
        {
            printf("Disconnected\n");
            return 0;
        }
        if (send(sfd, filename, strlen(filename), 0) == -1)
        {
            perror("send call error");
            return errno;
        }
        printf("Requested file: %s\n", filename);
        char recfilename[BUF_SIZE] = {0};
        sprintf(recfilename, "received-%s", filename);
        FILE *fp = fopen(recfilename, "w");
        if (fp == NULL)
        {
            perror("file open error");
            return errno;
        }
        write_file(fp, sfd);
        fclose(fp);
    }
}