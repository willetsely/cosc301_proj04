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
    char ip_add[32];
    struct request *next;
} request_t;

request_t *request_t_insert(int, char *, int);
request_t *request_t_remove();

#endif // __DATASTRUCTURES_H__
