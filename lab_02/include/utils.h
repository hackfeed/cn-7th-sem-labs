#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>

void exabort(char *msg)
{
    perror(msg);
    exit(1);
}

#endif