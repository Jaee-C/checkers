CC = gcc
COPTS = -Wall -std=c99 -pedantic

OBJS = main.c board_init.c input_move.c minimax.c

main: $(OBJS) Makefile
	$(CC) $(COPTS) -o main $(OBJS)

clean:
	rm -rf *.o *~ tt.txt test.txt