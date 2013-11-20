#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

struct request {
    char *filename;
    int socket;
    //log entry
    request_t *next;
} request_t

#endif // __DATASTRUCTURES_H__
