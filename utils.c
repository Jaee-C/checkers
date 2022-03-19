#include "board.h"

/* Duplicates `old` board and stores it into `new` */
void duplicate_board(board_t old, board_t new) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            new[row][col] = old[row][col];
        }
    }
}

/* Gets the position of the cell that's 1 cell away from `src` in
   the direction `direction`, returns a locn_t struct */
locn_t diagonal_move(locn_t src, int direction) {
    locn_t dest;
    if (direction == NORTHEAST) {
        dest.col = src.col + 1;
        dest.row = src.row - 1;
    } else if (direction == SOUTHEAST) {
        dest.col = src.col + 1;
        dest.row = src.row + 1;
    } else if (direction == SOUTHWEST) {
        dest.col = src.col - 1;
        dest.row = src.row + 1;
    } else {
        dest.col = src.col - 1;
        dest.row = src.row - 1;
    }
    return dest;
}

/* Convert `src` and `tar` into a string corresponding to the action, then
   save the string into `action` */
void get_action_name(locn_t src, locn_t target, char action[]) {
    action[SOURCE_COL] = src.col + ALPHA_TO_INT;
    action[SOURCE_ROW] = src.row + NUM_TO_INT;
    action[2] = '-';
    action[TARGET_COL] = target.col + ALPHA_TO_INT;
    action[TARGET_ROW] = target.row + NUM_TO_INT;
    action[MOVELEN] = '\0';
}
