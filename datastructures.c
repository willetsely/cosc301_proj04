#include "datastructures.h"

//declare global variables
extern request_t *head;
extern request_t *tail;

request_t *request_t_insert(int sock, char *ip_add, int port)
{
    request_t *new = (request_t *)malloc(sizeof(request_t *));
    if(new == NULL)
        return NULL;
    new->sock = sock;
    new->port = port;
    strcpy(new->ip_add, ip_add);
    new->next = head;
    return new;
}

request_t *request_t_remove() //returns the removed node
{
    request_t *temp = head;
    if(head != tail) //if more than one node left
    {    
        while(temp->next->next != NULL) //traverse the list until right before tail
        {
            temp = temp->next;
        }
        tail = temp; //move the tail pointer forward one node
        temp = temp->next; //temp is now the old tail
        tail->next = NULL; //cut off the old tail
    }
    else    //if head == tail: only one request in the list
    {
        head = NULL;
        tail = NULL;
    }
    return temp;
}

