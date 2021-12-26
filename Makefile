CC = gcc
COPTS = -Wall -std=c99 -pedantic

OBJS = main.c board_init.c input_move.c minimax.c

main: $(OBJS) Makefile
	$(CC) $(COPTS) -o main $(OBJS)

test: main
	./main < tests/test1.txt | diff -b - tests/test-out/test1-out.txt

clean:
	rm -rf *.o *~ tt.txt test.txt