#include "datastructures.h"
//include a bunch of stuff

//declare global variables
extern request_t *head;
extern request_t *tail;

request_t *request_t_insert(int sock, char *ip_add, int port)
{
    request_t *new = malloc(sizeof(request_t *));
    if(new == NULL)
        return NULL;
    new->sock = sock;
    new->port = port;
    new->ip_add = ip_add;
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
        head = NULL;
        free(tail);
        temp = NULL;
    }
    return temp;
}
