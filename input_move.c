#include "board.h"

// print information about the input action to stdout
void
print_action(board_t B, action_t* A, int turn, int is_computed) {
    assert(A!=NULL && B!=NULL);
    printf(STR_DELIMITER);                  // print action delimiter
    if (is_computed) {
        printf(STR_COMPUTED_ACTION_MARKER); // print computed action marker
    }
    if (turn%2) {
        printf(STR_WHITE_ACTION, turn+1);   // print white action number
    } else {
        printf(STR_BLACK_ACTION, turn+1);   // print black action number
    }
    // print action details (source and target cells)
    printf(STR_ACTION_INFO,A->scol+CAPITAL_A,A->srow+1,
            A->tcol+CAPITAL_A,A->trow+1);
    printf(STR_BOARD_COST,get_board_cost(B));   // print board cost into
    print_board(B);                             // print board configuration
}

// read input command from stdin
int
read_input_cmd(action_t *A) {
    assert(A!=NULL);
    char c=getchar();                       // get one character from stdin
    if (scanf(STR_ACTION_INFO_SFX,&A->srow,(char*)&A->tcol,&A->trow)==3) {
        // if three more inputs can be read from stdin ...
        A->srow--; A->trow--;   // adjust source and target rows
        A->scol=c-CAPITAL_A;    // set source column
        A->tcol-=CAPITAL_A;     // adjust target column
        return INPUT_ACTION;    // an action was read from stdin
    } else if (c==CAPITAL_A) {
        return INPUT_COMMAND_COMPUTE_ACTION;    // 'A' command was read
    } else if (c==CAPITAL_P) {
        return INPUT_COMMAND_PLAY;              // 'P' command was read
    }
    return INPUT_UNKNOWN;                       // unknown input encountered
}

// check action
int
check_action(board_t B, action_t* A, int turn) {
    assert(A!=NULL && B!=NULL);
    if (!is_on_board(A->srow,A->scol)) {
        return ACTION_ILLEGAL_SRC_OUTSIDE;  // source cell is not on the board
    } else if (!is_on_board(A->trow,A->tcol)) {
        return ACTION_ILLEGAL_TGT_OUTSIDE;  // target cell is not on the board
    } else if (B[A->srow][A->scol]==CELL_EMPTY) {
        return ACTION_ILLEGAL_SRC_EMPTY;    // source cell is empty
    } else if (B[A->trow][A->tcol]!=CELL_EMPTY) {
        return ACTION_ILLEGAL_TGT_NOT_EMPTY;// target cell is not empty
    } else if (turn%2 && (B[A->srow][A->scol]==CELL_BPIECE ||
    B[A->srow][A->scol]==CELL_BTOWER)) {    // source should hold white element
        return ACTION_ILLEGAL_SRC_HOLDS_OPPONENT;
    } else if (!(turn%2) && (B[A->srow][A->scol]==CELL_WPIECE || 
    B[A->srow][A->scol]== CELL_WTOWER)) {   // source should hold black element
        return ACTION_ILLEGAL_SRC_HOLDS_OPPONENT;
    } else if (!((abs(A->srow-A->trow)==1 && abs(A->scol-A->tcol)==1) ||
        (abs(A->srow-A->trow)==2 && abs(A->scol-A->tcol)==2))) {
        return ACTION_ILLEGAL_OTHER;        // non-diagonal move or capture
    } else if (B[A->srow][A->scol]==CELL_BPIECE && A->srow-A->trow<0) {
        return ACTION_ILLEGAL_OTHER;        // black piece cannot move down
    } else if (B[A->srow][A->scol]==CELL_WPIECE && A->srow-A->trow>0) {
        return ACTION_ILLEGAL_OTHER;        // white piece cannot move up
    } else if (abs(A->srow-A->trow)==2 && abs(A->scol-A->tcol)==2) {
        int r = (A->srow+A->trow)/2;
        int c = (A->scol+A->tcol)/2;
        if ((turn%2 && !(B[r][c]==CELL_BPIECE || B[r][c]==CELL_BTOWER)) ||
            (!(turn%2) && !(B[r][c]==CELL_WPIECE || B[r][c]==CELL_WTOWER))) {
            return ACTION_ILLEGAL_OTHER;    // wrong capture
        }
    }

    return ACTION_LEGAL;                // action is legal
}

// take input action in the input board
void
take_action(board_t B, action_t* A) {
    assert(A!=NULL && B!=NULL);
    B[A->trow][A->tcol]=B[A->srow][A->scol];    // transfer piece/tower
    B[A->srow][A->scol]=CELL_EMPTY;             // mark source cell as empty
    if (abs(A->srow-A->trow)==2 && abs(A->scol-A->tcol)==2) {
        // mark cell of the captured piece or tower as empty
        B[(A->srow+A->trow)/2][(A->scol+A->tcol)/2]=CELL_EMPTY;
    }
    if (A->trow==0 && B[A->trow][A->tcol]==CELL_BPIECE) {
        B[A->trow][A->tcol]=CELL_BTOWER;    // promote black piece to a tower
    } else if (A->trow==BOARD_SIZE-1 && B[A->trow][A->tcol]==CELL_WPIECE) {
        B[A->trow][A->tcol]=CELL_WTOWER;    // promote white piece to a tower
    }
}

// compute the cost of the input board
int
get_board_cost(board_t B) {
    assert(B!=NULL);
    // cost can be computed in a single pass through the board, but we reuse
    // the 'count_cells' function here :)
    return COST_PIECE*(count_cells(B,CELL_BPIECE)-
            count_cells(B,CELL_WPIECE))+COST_TOWER*
            (count_cells(B,CELL_BTOWER)-count_cells(B,CELL_WTOWER));
}

// print action error status to stdout
void
print_error(int status) {
    if (status==ACTION_ILLEGAL_SRC_OUTSIDE) {
        printf(STR_ERROR_1);
    } else if (status==ACTION_ILLEGAL_TGT_OUTSIDE) {
        printf(STR_ERROR_2);
    } else if (status==ACTION_ILLEGAL_SRC_EMPTY) {
        printf(STR_ERROR_3);
    } else if (status==ACTION_ILLEGAL_TGT_NOT_EMPTY) {
        printf(STR_ERROR_4);
    } else if (status==ACTION_ILLEGAL_SRC_HOLDS_OPPONENT) {
        printf(STR_ERROR_5);
    } else if (status==ACTION_ILLEGAL_OTHER) {
        printf(STR_ERROR_6);
    }
}

// check if the cell defined by row 'row' and column 'col' is on the board
int
is_on_board(int row, int col) {
    if (row<0 || row>=BOARD_SIZE || col<0 || col>=BOARD_SIZE) {
        return 0;   // cell is outside of the board
    }
    return 1;       // cell is on the board
}
