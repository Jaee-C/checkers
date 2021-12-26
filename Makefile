CC = gcc
COPTS = -Wall -std=c99

OBJS = main.o board_init.o input_move.o

main: $(OBJS) Makefile
	$(CC) $(COPTS) -o main $(OBJS)

main.o:	main.c board.h Makefile

board_init.o: board_init.c board.h Makefile

input_move.o: input_move.c board.h Makefile

.c .o:
	$(CC) $(COPTS) -c -o $@ $<

clean:
	rm -rf *.o *~ tt.txt test.txt