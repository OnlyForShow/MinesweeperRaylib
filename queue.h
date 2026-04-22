#pragma once

typedef char BOOL;

typedef struct element
{
    long x,y;
    struct element *next;
}element;

struct queue
{
    element * head;
    element * last;
    size_t size;
};

typedef struct queue queue;


queue* init_queue();
void push_queue(queue *q, long xpos, long ypos);
element pop_queue(queue *q);
element front_queue(queue *q);
BOOL is_empty(queue *q);
size_t size_of_queue(queue *q);
void delete_queue(queue *q);
