#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

typedef struct request {
    int sock;
    int port;
    char *ip_add;
    struct request *next;
} request_t;

#endif // __DATASTRUCTURES_H__
