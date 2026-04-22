CC = gcc

CFLAGS = -Wall -Wextra -MMD -MP

SOURCE = $(wildcard *.c)
OBJS = $(SOURCE:.c=.o)

INCLUDE_PATH = -I"./include"
LIBRARY_PATH = -L"./lib"

LIBRARIES = -lraylib -lm -lpthread -ldl -lrt -lX11


all: minesweeper

minesweeper: $(OBJS)
	$(CC) $^  $(LIBRARY_PATH) $(LIBRARIES) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@
	echo $<

-include $(OBJS:.o=.d)

.PHONY: clean run test

clean:
	rm -f *.o *.d minesweeper

run: minesweeper
	./minesweeper

test: queue.c
	$(CC) -I"." queue.c test/test_queue.c -o test/test_queue
	test/test_queue
