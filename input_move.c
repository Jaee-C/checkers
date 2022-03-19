#include "board.h"
int piece_capture(board_t, locn_t, locn_t, char);
int tower_capture(board_t, locn_t, locn_t, char);

/* Update board array to reflect a piece/tower move */
void update_board(board_t board, locn_t src, locn_t target) {
    char cell = board[src.row][src.col];

    if (cell == CELL_WPIECE && target.row == BOARD_SIZE - 1) {
        // white piece reached the other side (last row), promote
        board[target.row][target.col] = CELL_WTOWER;
        board[src.row][src.col] = '.';
    } else if (cell == CELL_BPIECE && target.row == 0) {
        // black piece reached the other side (first row), promote
        board[target.row][target.col] = CELL_BTOWER;
        board[src.row][src.col] = '.';
    } else {
        // no promotion to tower, regular move
        board[target.row][target.col] = cell;
        board[src.row][src.col] = '.';
    }
}

char change_player(char color) {
    if (color == CELL_BPIECE) {
        return CELL_WPIECE;
    } else {
        return CELL_BPIECE;
    }
}

/* Process string into integers indicating source and target row/column, 
   then stores them into locn_t structs */
void process_input(char *action, locn_t *src, locn_t *target) {
    src->col = action[SOURCE_COL] - ALPHA_TO_INT;
    src->row = action[SOURCE_ROW] - NUM_TO_INT;
    target->col = action[TARGET_COL] - ALPHA_TO_INT;
    target->row = action[TARGET_ROW] - NUM_TO_INT;
}

int calculate_cost(board_t board) {
    int total = 0;

    // Applies formula: b + 3V - w - 3W
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CELL_BPIECE) {
                total += COST_PIECE;
            } else if (board[row][col] == CELL_WPIECE) {
                total -= COST_PIECE;
            } else if (board[row][col] == CELL_BTOWER) {
                total += COST_TOWER;
            } else if (board[row][col] == CELL_WTOWER) {
                total -= COST_TOWER;
            }
        }
    }

    return total;
}

/* This function is adapted from getword.c written by Alistair Moffat,
   accessed via https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/getword.c.

   Reads user input from stdin and saves the string into `action`, returns 
   length of string read or EOF. */
int get_input(char action[], char color) {
    char c;
    int len=0;

    // Displays current player prompt
    if (color == CELL_BPIECE) {
        printf("BLACK: ");
    } else {
        printf("WHITE: ");
    }

    while ((c = getchar()) != EOF && !isalpha(c)) {
        // Skips through all non alphabetics
    }
    if (c == EOF) {
        return EOF;
    }
    action[len++] = c;      // First character found
    while((c = getchar()) != EOF && c != '\n' && len < MOVELEN) {
        action[len++] = c;  // Add remaining characters
    }
    action[len] = '\0';     // Close off the string
    return len;
}

/* Checks for invalid moves and remove captured piece (if any) */
int check_error(board_t board, locn_t source, locn_t target, char c) {
    char source_piece, target_cell;

    // Check if source and target cells are within board
    if (source.col >= BOARD_SIZE || source.row >= BOARD_SIZE ||
        source.col < 0 || source.col < 0) {
        return ERROR1;
    } else if (target.col >= BOARD_SIZE || target.row >= BOARD_SIZE || 
               target.col < 0 || target.row < 0) {
        return ERROR2;
    }
    
    // source and target cells are within board
    source_piece = board[source.row][source.col];
    target_cell = board[target.row][target.col];
    if (source_piece == CELL_EMPTY) {
        return ERROR3;
    } else if (target_cell != CELL_EMPTY) {
        return ERROR4;
    } else if (tolower(source_piece) != c) {
        return ERROR5;
    } else if (abs(source.row - target.row) != 1) {
        // Action indicating capture intention, check for error in capture
        if ((source_piece == CELL_WPIECE || source_piece == CELL_BPIECE) && 
             abs(source.row - target.row) == 2) {
            return piece_capture(board, source, target, source_piece);
        } else if ((source_piece == CELL_WTOWER || source_piece == CELL_BTOWER) 
                    && abs(source.row - target.row) == 2) {
            return tower_capture(board, source, target, source_piece);
        } else {
            return ERROR6;
        }
    // Action not capturing, check if move is valid
    } else if (source_piece == CELL_BPIECE && ((source.row - 1 != target.row) ||
                (abs(source.col - target.col) != 1))) {
        return ERROR6;
    } else if (source_piece == CELL_WPIECE && ((source.row + 1 != target.row) ||
                (abs(source.col - target.col) != 1))) {
        return ERROR6;
    }
    return 0;
}

/* Checks if a 'piece' capture move is valid. If so update board to remove 
   captured piece; if not, return error */
int piece_capture(board_t board, locn_t src, locn_t target, char piece) {
    char enemy;       // enemy player color
    int enemy_row;    // The row number in which enemy piece should be at
    if (piece == CELL_BPIECE) {
        enemy = CELL_WPIECE;
        enemy_row = src.row - 1;
        if (enemy_row - 1 != target.row) {
            return ERROR6;
        }
    } else {
        enemy = CELL_BPIECE;
        enemy_row = src.row + 1;
        if (enemy_row + 1 != target.row) {
            return ERROR6;
        }
    }

    if (tolower(board[enemy_row][src.col + 1]) == enemy &&
        src.col + 2 == target.col){
        // Piece capturing enemy to the right
        board[enemy_row][src.col + 1] = CELL_EMPTY;
        return 0;
    } else if (tolower(board[enemy_row][src.col - 1]) == enemy &&
               src.col - 2 == target.col) {
        // Piece capturing enemy to the left
        board[enemy_row][src.col - 1] = CELL_EMPTY;
        return 0;
    } else {
        return ERROR6;
    }

    return 0;
}

/* Checks if a 'tower' capture move is valid. If so update board to remove 
   captured piece; if not, return error */
int tower_capture(board_t board, locn_t src, locn_t target, char tower) {
    char enemy;      // Enemy player color
    if (tower == CELL_BTOWER) {
        enemy = CELL_WPIECE;
    } else {
        enemy = CELL_BPIECE;
    }

    if (tolower(board[src.row + 1][src.col + 1]) == enemy &&
        src.col + 2 == target.col) {
        // Tower capturing enemy bottom right
        board[src.row + 1][src.col + 1] = CELL_EMPTY;
    } else if (tolower(board[src.row + 1][src.col - 1]) == enemy &&
            src.col - 2 == target.col) {
        // Tower capturing enemy bottom left
        board[src.row + 1][src.col - 1] = CELL_EMPTY;
    } else if (tolower(board[src.row - 1][src.col + 1]) == enemy &&
        src.col + 2 == target.col) {
        // Tower capturing enemy top right
        board[src.row - 1][src.col + 1] = CELL_EMPTY;
    } else if (tolower(board[src.row - 1][src.col - 1]) == enemy &&
            src.col - 2 == target.col) {
        // Tower capturing enemy top left
        board[src.row - 1][src.col - 1] = CELL_EMPTY;
    }else {
        return ERROR6;
    }
    return 0;
}

/* Prints error codes and terminates program */
void print_error(int err) {
    if (err == ERROR1) {
        printf("ERROR: Source cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    } else if (err == ERROR2) {
        printf("ERROR: Target cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    } else if (err == ERROR3) {
        printf("ERROR: Source cell is empty.\n");
        exit(EXIT_FAILURE);
    } else if (err == ERROR4) {
        printf("ERROR: Target cell is not empty.\n");
        exit(EXIT_FAILURE);
    } else if (err == ERROR5) {
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    } else if (err == ERROR6) {
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
}

/* Checks if a piece in `b` at `src` moving in `direction` by `player` is a
   valid move. Returns pointer to a data_t struct of the move if valid, else 
   return NULL */
data_t *is_valid_move(board_t b, locn_t src, int direction, char player) {
    int error = 0;
    locn_t dest;
    data_t *new;
    new = malloc(sizeof(data_t));
    assert(new);

    duplicate_board(b, new->board);
    dest = diagonal_move(src, direction);    // Dest is one cell away from src
    error = check_error(new->board, src, dest, player);

    if (error == ERROR4) {
        // Dest cell has opponent piece/tower
        // Move dest one more cell away to test for capture
        dest = diagonal_move(dest, direction);
        error = check_error(new->board, src, dest, player);
    }

    if (!error) {
        // Move is valid! Save updated board into `new`
        update_board(new->board, src, dest);
        new->src = src;
        new->dest = dest;
    } else {
        // Move is invalid, free `new`
        free(new);
        new = NULL;
    }
    return new;
}

/* Prints all the required information for a move - action, cost, board */
void print_move(board_t b, int count, char *action, char player, int comp) {
    int cost;
    cost = calculate_cost(b);
    printf("%s\n", DELIMITER);
    if (comp) {    // Computed actions need to have "***" before action
        printf("*** ");
    }
    if (player == CELL_BPIECE) {
        printf("BLACK ");
    } else if (player == CELL_WPIECE) {
        printf("WHITE ");
    }
    printf("ACTION #%d: %s\n", count, action);
    printf("BOARD COST: %d\n", cost);
    print_board(b);
}
