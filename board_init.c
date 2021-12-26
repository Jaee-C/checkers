#include "board.h"

// initialize input board by ensuring the initial setup of the pieces
void initialize_board(board_t B) {
    assert(B!=NULL);
    int row, col;
    for (row=0; row<BOARD_SIZE; row++) {        // iterate over rows
        for (col=0; col<BOARD_SIZE; col++) {    // iterate over columns
            if (row<ROWS_WITH_PIECES && (row+col)%2) {
                B[row][col] = CELL_WPIECE;      // put white piece in this cell
            } else if (row>=BOARD_SIZE-ROWS_WITH_PIECES && (row+col)%2) {
                B[row][col] = CELL_BPIECE;      // put black piece in this cell
            } else {
                B[row][col] = CELL_EMPTY;       // mark cell as empty
            }
        }
    }
}

// count the number of cells in the input board specified by the input character
int count_cells(board_t B, char c) {
    assert(B!=NULL);
    int row, col, res=0;                        // initiaize result to zero
    for (row=0; row<BOARD_SIZE; row++) {        // iterate over rows
        for (col=0; col<BOARD_SIZE; col++) {    // iterate over columns
            res += (B[row][col]==c);            // count cells of interest
        }
    }
    return res;
}

// print the input board configuration to stdout 
void print_board(board_t B) {
    assert(B!=NULL);
    int row, col;
    printf(STR_DOUBLE_SPACE);
    for (col=0; col<BOARD_SIZE; col++) { 
        printf(STR_BOARD_COL_TITLE,CAPITAL_A+col);  // print column headers
    }
    print_row_delim(BOARD_SIZE);                    // print row delimiter
    for (row=0; row<BOARD_SIZE; row++) {            // print row content
        printf(STR_BOARD_ROW_TITLE,row+1);
        for (col=0; col<BOARD_SIZE; col++) {
            printf(STR_BOARD_CELL_CONTENT,B[row][col]);
        }
        print_row_delim(BOARD_SIZE);                // print row delimiter
    }
}

// print the deliminter of the chess board rows (a helper function)
void print_row_delim(int size) {
    int col;
    printf(STR_BOARD_ROW_INDENT);                   // print row indentation
    for (col=0; col<BOARD_SIZE; col++) {
        printf(STR_BOARD_ROW_DELIM);                // print row delimiter
    }
    printf(STR_NEW_LINE);                           // print new line character
}
