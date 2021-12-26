#include <stdio.h>
#include "board.h"

/* Fills `board` array with the initial setup, then prints the initial
   summary of the board */
void board_init(board_t board) {
    int row_has_piece = 0;
    char piece;

    // Initialises and fills board array
    for (int row = 0; row < BOARD_SIZE; row++) {
        if (row < ROWS_WITH_PIECES) {
            // First `ROWS_WITH_PIECES` rows are white
            row_has_piece = 1;
            piece = CELL_WPIECE;
        } else if (BOARD_SIZE - ROWS_WITH_PIECES <= row) {
            // Last `ROWS_WITH_PIECES` rows are black
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

    // Prints header and initial board
    printf("BOARD SIZE: %dx%x\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", BOARD_SIZE * ROWS_WITH_PIECES / 2);
    printf("#WHITE PIECES: %d\n", BOARD_SIZE * ROWS_WITH_PIECES / 2);
    print_board(board);
}

/* Finds out if a specific cell in the initial board should have a piece */
int cell_has_piece(int row, int col) {
    // even row# has pieces in odd col#, odd row# has pieces in even col#
    if ((row % 2 == 0 && col % 2 != 0) || 
        (row % 2 != 0 && col % 2 == 0)) {
        return 1;
    } else {
        return 0;
    }
}

/* Prints board with row and column numbers */
void print_board(board_t board) {
    char col_name = 'A';    // The name of column 1 is 'A'
    
    // Print column names
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("   %c", col_name);    // Column headers
        col_name++;
    }
    printf("\n");

    // Print row numbers + content of `board`
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("   %s\n", ROW_SEP);    // Row separation for every new row
        printf("%2d |", row + 1);
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf(" %c |", board[row][col]);     // Prints individual cells
        }
        printf("\n");
    }
    printf("   %s\n", ROW_SEP);
}
