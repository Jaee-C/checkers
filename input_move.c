#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"

void input_move(board_t board, char color) {
    locn_t source, target;
    char action[MOVELEN+1];
    int input_len, count = 0;

    while ((input_len = get_input(action)) != EOF && input_len == MOVELEN) {
        source.col = action[0] - ALPHA_TO_INT;
        source.row = action[1] - NUM_TO_INT;
        target.col = action[3] - ALPHA_TO_INT;
        target.row = action[4] - NUM_TO_INT;

        check_input_error(board, source, target, color);

        board[target.row][target.col] = board[source.row][source.col];
        board[source.row][source.col] = '.';

        count++;

        if (color == CELL_BPIECE) {
            printf("BLACK ");
            color = CELL_WPIECE;
        } else if (color == CELL_WPIECE) {
            printf("WHITE ");
            color = CELL_BPIECE;
        }
        printf("ACTION #%d: %s\n", count, action);
        printf("BOARD COST: %d\n", cost(board));
        print_board(board);
    }
}

int cost(board_t board) {
    int total = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CELL_BPIECE) {
                total += 1;
            } else if (board[row][col] == CELL_WPIECE) {
                total -= 1;
            } else if (board[row][col] == CELL_BTOWER) {
                total += 3;
            } else if (board[row][col] == CELL_WTOWER) {
                total -= 3;
            }
        }
    }

    return total;
}

int get_input(char action[]) {
    char c;
    int len=0;
    while ((c = getchar()) != EOF && !isalpha(c)) {
        // Skips through all non alphabetics
    }

    if (c == EOF) {
        return EOF;
    }

    action[len++] = c;
    while((c = getchar()) != EOF && c != '\n' && len < 6) {
        action[len++] = c;
    }
    action[len] = '\0';
    return len;
}

void check_input_error(board_t board, locn_t source, locn_t target, char c) {
    if (source.col >= BOARD_SIZE || source.row >= BOARD_SIZE) {
        printf("ERROR: %s\n", ERROR1);
        exit(EXIT_FAILURE);
    } else if (target.col >= BOARD_SIZE || target.row >= BOARD_SIZE) {
        printf("ERROR: %s\n", ERROR2);
        exit(EXIT_FAILURE);
    } else if (board[source.row][source.col] == CELL_EMPTY) {
        printf("ERROR: %s\n", ERROR3);
        exit(EXIT_FAILURE);
    } else if (board[target.row][target.col] != CELL_EMPTY) {
        printf("ERROR: %s\n", ERROR4);
        exit(EXIT_FAILURE);
    } else if (board[source.row][source.col] != c) {
        printf("ERROR: %s\n", ERROR5);
        exit(EXIT_FAILURE);
    }

    if (c == 'b' && ((source.row - 1 != target.row) || 
                     (abs(source.col - target.col) != 1))) {
        printf("ERROR: %s\n", ERROR6);
        exit(EXIT_FAILURE);
    } else if (c == 'w' && ((source.row + 1 != target.row) || 
                            (abs(source.col - target.col) != 1))) {
        printf("ERROR: %s\n", ERROR6);
        exit(EXIT_FAILURE);
    }
}