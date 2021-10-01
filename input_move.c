#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"

void player_move(board_t board, char color) {
    locn_t source, target;
    char action[6];

    get_input(action);

    source.col = action[0] - ALPHA_TO_INT;
    source.row = action[1] - NUM_TO_INT;
    target.col = action[3] - ALPHA_TO_INT;
    target.row = action[4] - NUM_TO_INT;

    check_input_error(board, source, target, color);

    board[target.row][target.col] = board[source.row][source.col];
    board[source.row][source.col] = '.';    
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
    while((c = getchar()) != EOF && c != ' ' && len < 6) {
        action[len++] = c;
    }
    action[len] = '\0';
    return 1;
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