CC = gcc
COPTS = -Wall -std=c99 -pedantic

OBJS = main.c board_init.c input_move.c minimax.c bot.c set_player.c set_level.c

main: $(OBJS) Makefile board.h
	$(CC) $(COPTS) -o main $(OBJS)

test: main
	./main < tests/test1.txt | diff -b - tests/test-out/test1-out.txt

clean:
	rm -rf *.o *~ tt.txt test.txt