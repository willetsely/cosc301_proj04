#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

typedef struct request {
    int sock;
    int port;
    char *ip_add;
    struct request *next;
} request_t;

#endif // __DATASTRUCTURES_H__
