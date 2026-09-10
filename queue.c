#include <stdlib.h>
#include "queue.h"




typedef struct element
{
    long x,y;
    struct element *next;
}element;

element error_element = {.x = -133322,.y=443222,.next=NULL};

typedef struct queue
{
    element * head;
    element * last;
    size_t size;
} queue;




queue* init_queue()
{
    queue *q = (queue*) malloc(sizeof(queue));
    q->head = NULL;
    q->last = NULL;
    q->size = 0;
    return q;
}

void push_queue(queue *q, value e)
{
    if(!q->head)
    {
        q->head = (element *)malloc(sizeof(element));
        q->head->x = e.x;
        q->head->y = e.y;
        q->last = q->head;
        q->size++;
        return;
    }

    element * ptr = (element*)malloc(sizeof(element));
    ptr->x = e.x;
    ptr->y = e.y;
    ptr->next = NULL;
    
    q->last->next = ptr;
    q->last = ptr;
    q->size++;
}

value pop_queue(queue *q)
{
    if(!q->head)return (value){error_element.x, error_element.y};
    
    element * tmp = q->head->next;
    element ret = *(q->head);
    free(q->head);

    q->head = tmp;
    q->size--;
    
    return (value){ret.x, ret.y}; 
}

value front_queue(queue *q)
{
    if(!q->head)return (value){error_element.x, error_element.y};
    
    return (value){q->head->x, q->head->y};
}

int is_empty(queue *q)
{
    if(q->size == 0)return 1;
    return 0;
}

size_t size_of_queue(queue *q)
{
    return q->size;
}

void delete_queue(queue *q)
{
    element *next = q->head;
    while(next)
    {
        element *tmp = next->next;
        free(next);
        next = tmp;
    }

    free(q);
}
