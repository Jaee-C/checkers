#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../board.h"

/* Update board array to reflect new move */
void update_board(board_t board, locn_t s, locn_t t) {
    char cell = board[s.row][s.col];

    if (cell == CELL_WPIECE && t.row == BOARD_SIZE - 1) {
        // white piece reached the other side (last row), promote
        board[t.row][t.col] = CELL_WTOWER;
        board[s.row][s.col] = '.';
    } else if (cell == CELL_BPIECE && t.row == 0) {
        // black piece reached the other side (first row), promote
        board[t.row][t.col] = CELL_BTOWER;
        board[s.row][s.col] = '.';
    } else {
        // no promotion to tower
        board[t.row][t.col] = board[s.row][s.col];
        board[s.row][s.col] = '.';
    }
}

/* Prints all the required information for a move */
void print_move(board_t board, int count, char *action, char *c) {
    print_delimiter();
    if (*c == CELL_BPIECE) {
        printf("BLACK ");
        *c = CELL_WPIECE;
    } else if (*c == CELL_WPIECE) {
        printf("WHITE ");
        *c = CELL_BPIECE;
    }
    printf("ACTION #%d: %s\n", count, action);
    printf("BOARD COST: %d\n", cost(board));
    print_board(board);
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
}

/* Check for any capture moves or illegal actions */
void capture_check(board_t board, locn_t source, locn_t target) {
    char source_cell = board[source.row][source.col];

    // Program ends if illegal action is encountered
    if ((source_cell == CELL_WPIECE || source_cell == CELL_BPIECE) && 
        abs(source.row-target.row) == 2) {
        piece_capture(board, source, target, source_cell);
    } else if ((source_cell == CELL_WTOWER || source_cell == CELL_BTOWER) &&
               abs(source.row - target.row) == 2) {
        tower_capture(board, source, target, source_cell);
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

void piece_capture(board_t board, locn_t s, locn_t t, char piece) {
    char enemy;
    int enemy_row;
    if (piece == CELL_BPIECE) {
        enemy = CELL_WPIECE;
        enemy_row = s.row - 1;
        if (enemy_row - 1 != t.row) {
            printf("ERROR: Illegal action.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        enemy = CELL_BPIECE;
        enemy_row = s.row + 1;
        if (enemy_row + 1 != t.row) {
            printf("ERROR: Illegal action.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (tolower(board[enemy_row][s.col + 1]) == enemy &&
        s.col + 2 == t.col) {
        board[enemy_row][s.col + 1] = CELL_EMPTY;
    } else if (tolower(board[enemy_row][s.col - 1]) == enemy &&
               s.col - 2 == t.col) {
        board[enemy_row][s.col - 1] = CELL_EMPTY;
    } else {
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
}

void tower_capture(board_t board, locn_t s, locn_t t, char tower) {
    char enemy;
    if (tower == CELL_BTOWER) {
        enemy = CELL_WPIECE;
    } else {
        enemy = CELL_BPIECE;
    }

    if (tolower(board[s.row + 1][s.col + 1]) == enemy &&
        s.col + 2 == t.col) {
        board[s.row + 1][s.col + 1] = CELL_EMPTY;
    } else if (tolower(board[s.row + 1][s.col - 1]) == enemy &&
               s.col - 2 == t.col) {
        board[s.row + 1][s.col - 1] = CELL_EMPTY;
    } else if (tolower(board[s.row - 1][s.col + 1]) == enemy &&
        s.col + 2 == t.col) {
        board[s.row - 1][s.col + 1] = CELL_EMPTY;
    } else if (tolower(board[s.row - 1][s.col - 1]) == enemy &&
               s.col - 2 == t.col) {
        board[s.row - 1][s.col - 1] = CELL_EMPTY;
    }else {
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
}