#include <stdlib.h>
#include <stdio.h>

#include <assert.h>
#include <queue.h>

void test1()
{
    queue * q = init_queue();
    assert(is_empty(q));
    printf("Test 1 - Empty [PASSED]\n");
}

void test2()
{
    queue *q = init_queue();
    push_queue(q, 12, 23);
    assert(!is_empty(q));
    delete_queue(q);
    printf("Test 2 - More than one [PASSED]\n");    
}

void test3()
{
    queue *q = init_queue();
    push_queue(q, 1,2);
    push_queue(q, 2,4);
    push_queue(q, 3,6);
    push_queue(q, 4,8);
    assert(size_of_queue(q) == 4);

    
    element e1 = pop_queue(q);
    assert(size_of_queue(q) == 3);    
    assert((e1.x == 1) && (e1.y == 2));

    element e2 = pop_queue(q);
    assert(size_of_queue(q) == 2);    
    assert((e2.x == 2) && (e2.y == 4));
    
    element e3 = pop_queue(q);
    assert(size_of_queue(q) == 1);    
    assert((e3.x == 3) && (e3.y == 6));

    element e4 = pop_queue(q);
    assert(size_of_queue(q) == 0);    
    assert((e4.x == 4) && (e4.y == 8));

    assert(is_empty(q));
    
    delete_queue(q);
    printf("Test 3 - Popped ? [PASSED]\n");    
}

void test4()
{
    queue *q = init_queue();
    push_queue(q, 1,2);
    push_queue(q, 2,4);
    push_queue(q, 3,6);
    push_queue(q, 4,8);
    assert(size_of_queue(q) == 4);

    
    element e1 = pop_queue(q);
    assert(size_of_queue(q) == 3);    
    assert((e1.x == 1) && (e1.y == 2));

    element e2 = pop_queue(q);
    assert(size_of_queue(q) == 2);    
    assert((e2.x == 2) && (e2.y == 4));

    push_queue(q, 5,10);
    assert(size_of_queue(q) == 3);        
    
    element e3 = pop_queue(q);
    assert(size_of_queue(q) == 2);    
    assert((e3.x == 3) && (e3.y == 4));

    element e4 = pop_queue(q);
    assert(size_of_queue(q) == 1);    
    assert((e4.x == 4) && (e4.y == 8));

    element e5 = pop_queue(q);
    assert(size_of_queue(q) == 0);    
    assert((e4.x == 5) && (e4.y == 10));

    
    assert(is_empty(q));
    
    delete_queue(q);
    printf("Test 4 - Push Pop Push [PASSED]\n");    
}

int main()
{
    printf("TEST BEGIN\n");
    test1();
    test2();
    test3();
    test4();

    printf("TEST RUN SUCCESSFUL\n");
    return 0;
}
    

