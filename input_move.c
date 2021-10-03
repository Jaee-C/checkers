#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"

void input_move(board_t board, char color) {
    locn_t source, target;
    char action[MOVELEN+1];
    int input_len, count = 0;

    while ((input_len = get_input(action)) != EOF && input_len == MOVELEN) {
        // Stops when non-move input ('A' or 'P') is read
        process_input(action, &source, &target);
        check_input_error(board, source, target, color);

        // Update board array to reflect new move
        board[target.row][target.col] = board[source.row][source.col];
        board[source.row][source.col] = '.';

        count++;

        // Output updated board after every input
        print_delimiter();
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

/* Process string into integers indicating source and target row/column */
void process_input(char *action, locn_t *s, locn_t *t) {
    s->col = action[SOURCE_COL] - ALPHA_TO_INT;
    s->row = action[SOURCE_ROW] - NUM_TO_INT;
    t->col = action[TARGET_COL] - ALPHA_TO_INT;
    t->row = action[TARGET_ROW] - NUM_TO_INT;
}

/* Calculate cost of the board */
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

/* Reads user input from stdin, returns length of string read or EOF */
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
    while((c = getchar()) != EOF && c != '\n' && len <= MOVELEN) {
        action[len++] = c;
    }
    action[len] = '\0';
    return len;
}

/* Checks for invalid moves */
void check_input_error(board_t board, locn_t source, locn_t target, char c) {
    char source_cell = board[source.row][source.col];
    char target_cell = board[target.row][target.col];
    if (source.col >= BOARD_SIZE || source.row >= BOARD_SIZE) {
        printf("ERROR: Source cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    } else if (target.col >= BOARD_SIZE || target.row >= BOARD_SIZE) {
        printf("ERROR: Target cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    } else if (source_cell == CELL_EMPTY) {
        printf("ERROR: Source cell is empty.\n");
        exit(EXIT_FAILURE);
    } else if (target_cell != CELL_EMPTY) {
        printf("ERROR: Target cell is not empty.\n");
        exit(EXIT_FAILURE);
    } else if (tolower(source_cell) != c) {
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    }

    // Check for invalid steps involving capture
    if (source_cell == CELL_BPIECE && source.row - 2 == target.row && 
        abs(source.col - target.col) == 2) {
        if (tolower(board[source.row-1][source.col-1]) == CELL_WPIECE) {
            board[source.row - 1][source.col - 1] = CELL_EMPTY;
        } else if (tolower(board[source.row-1][source.col+1]) == CELL_WPIECE) {
            board[source.row - 1][source.col + 1] = CELL_EMPTY;
        } else {
            printf("ERROR: Illegal action.\n");
            exit(EXIT_FAILURE);
        }
    } else if (source_cell == CELL_WPIECE && source.row + 2 == target.row && 
               abs(source.col - target.col) == 2) {
        if (tolower(board[source.row+1][source.col-1]) == CELL_BPIECE) {
            board[source.row + 1][source.col - 1] = CELL_EMPTY;
        } else if (tolower(board[source.row+1][source.col+1]) == CELL_BPIECE) {
            board[source.row + 1][source.col + 1] = CELL_EMPTY;
        } else {
            printf("ERROR: Illegal action.\n");
            exit(EXIT_FAILURE);
        }
    } else if (source_cell == CELL_BPIECE && ((source.row - 1 != target.row) || 
                     (abs(source.col - target.col) != 1))) {
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    } else if (source_cell == CELL_WPIECE && ((source.row + 1 != target.row) || 
                            (abs(source.col - target.col) != 1))) {
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
}