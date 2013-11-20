#include "main.c"
//include a bunch of stuff

//declare global variables
extern request_t *head;
extern request_t *tail;

typedef struct request {
    char *filename;
    int socket;
    char *ip_add;
    int port;
    struct request *next;
} request_t;

request_t *request_t_insert(char *filename, int socket,)
{
    request_t *new = malloc(sizeof(request_t *));
    if(new == NULL)
        return NULL;
    new->filename = filename;
    new->socket = socket;
    new->next = head;
    return new;
}

request_t *request_t_remove()
{
    request_t *temp = head;
    if(head != tail) //if more than one node left
    {    
        while(temp->next->next != NULL) //traverse the list until right before tail
        {
            temp = temp->next;
        }
        temp->next = NULL; //cut off the tail
        free(tail); //free the tail
    }
    else    //if head == tail: only one request in the list
    {
        free(head);
        temp = NULL;
    }
    return temp;
}
