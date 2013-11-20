#include "datastructures.h"

//declare global variables

request_t *request_t_insert(int sock, char *ip_add, int port, request_t *head)
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

/*request_t *request_t_remove(request_t *head, request_t *tail)
{
    request_t *temp = head;
    if(head != tail) //if more than one node left
    {    
        while(temp->next->next != NULL) //traverse the list until right before tail
        {
            temp = temp->next;
        }
        temp->next = NULL; //cut off the tail
    }
    else    //if head == tail: only one request in the list
    {
        head = NULL;
        temp = NULL;
    }
    return temp;
}
*/

request_t *request_t_remove(request_t *head, int queue_cnt)
{
    request_t *temp = head;
    if(queue_cnt == 1){
        temp = NULL;}
    else{
        temp = temp->next;}
    return temp;
}
