#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

struct request_t {
    char *filename;
    int socket;
    //log entry
    request_t *next;
} request_t

#endif // __DATASTRUCTURES_H__
