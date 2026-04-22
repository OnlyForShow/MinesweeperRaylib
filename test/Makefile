CC = gcc

all: test


.PHONY: test clean

test:
	$(CC) -I".." ../queue.c test_queue.c -o test_queue
	./test_queue

clean:
	rm test_queue
