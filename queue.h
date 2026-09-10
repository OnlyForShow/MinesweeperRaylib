#pragma once


typedef struct queue queue;

typedef struct value
{
    long x, y;
} value;


queue* init_queue();
void push_queue(queue *q, value e);
value pop_queue(queue *q);
value front_queue(queue *q);
int is_empty(queue *q);
size_t size_of_queue(queue *q);
void delete_queue(queue *q);
