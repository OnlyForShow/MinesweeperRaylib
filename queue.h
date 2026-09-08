#pragma once


typedef struct
{
    long xpos, long ypos;
} value;


void* init_queue();
void push_queue(void *q, value e);
value pop_queue(void *q);
value front_queue(void *q);
int is_empty(void *q);
size_t size_of_queue(void *q);
void delete_queue(void *q);
