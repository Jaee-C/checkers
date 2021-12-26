#include <stdio.h>
#include "../board.h"

void print_head(void) {
    printf("BOARD SIZE: %dx%x\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", BOARD_SIZE * ROWS_WITH_PIECES / 2);
    printf("#WHITE PIECES: %d\n", BOARD_SIZE * ROWS_WITH_PIECES / 2);
}

void board_init(board_t board) {
    int row_has_piece = 0;
    char piece;
    for (int row = 0; row < BOARD_SIZE; row++) {
        if (row < ROWS_WITH_PIECES) {
            row_has_piece = 1;
            piece = CELL_WPIECE;
        } else if (BOARD_SIZE - ROWS_WITH_PIECES <= row) {
            row_has_piece = 1;
            piece = CELL_BPIECE;
        } else {
            row_has_piece = 0;
        }
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (row_has_piece && cell_has_piece(row, col)) {
                board[row][col] = piece;
            } else {
                board[row][col] = CELL_EMPTY;
            }
        }
    }

    print_head();
    print_board(board);
}

int cell_has_piece(int row, int col) {
    if ((row % 2 == 0 && col % 2 != 0) || 
        (row % 2 != 0 && col % 2 == 0)) {
        return 1;
    } else {
        return 0;
    }
}

void print_board(board_t board) {
    char col_name = 'A';
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("   %c", col_name);
        col_name++;
    }
    printf("\n");
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("   %s\n", ROW_SEP);
        printf("%2d |", row + 1);
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf(" %c |", board[row][col]);
        }
        printf("\n");
    }
    printf("   %s\n", ROW_SEP);
}

void print_delimiter(void) {
    for (int i = 0; i < DELIMITER_LEN; i++) {
        printf("%c",'=');
    }
    printf("\n");
}