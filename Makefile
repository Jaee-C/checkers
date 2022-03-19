CC = gcc
COPTS = -Wall -std=c99 -pedantic

OBJS = main.c board_init.o input_move.o minimax.o bot.o set_player.o set_level.o players.o utils.o

main: $(OBJS) Makefile board.h
	$(CC) $(COPTS) -o main $(OBJS)

board_init.o: utils/board_init.c board.h
	$(CC) $(COPTS) -c utils/board_init.c

input_move.o: utils/input_move.c board.h
	$(CC) $(COPTS) -c utils/input_move.c

minimax.o: utils/minimax.c board.h
	$(CC) $(COPTS) -c utils/minimax.c

bot.o: utils/bot.c board.h
	$(CC) $(COPTS) -c utils/bot.c

set_player.o: utils/set_player.c board.h
	$(CC) $(COPTS) -c utils/set_player.c

set_level.o: utils/set_level.c board.h
	$(CC) $(COPTS) -c utils/set_level.c

utils.o: utils/utils.c board.h
	$(CC) $(COPTS) -c utils/utils.c

players.o: utils/players.c board.h
	$(CC) $(COPTS) -c utils/players.c

test: main
	./main < tests/test1.txt | diff -b - tests/test-out/test1-out.txt

clean:
	rm -rf *.o *~ tt.txt test.txt