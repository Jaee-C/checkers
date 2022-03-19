#include "../board.h"

/* Duplicates `old` board and stores it into `new` */
void duplicate_board(board_t old, board_t new)
{
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            new[row][col] = old[row][col];
        }
    }
}

/* Gets the position of the cell that's 1 cell away from `src` in
   the direction `direction`, returns a locn_t struct */
locn_t diagonal_move(locn_t src, int direction)
{
    locn_t dest;
    if (direction == NORTHEAST)
    {
        dest.col = src.col + 1;
        dest.row = src.row - 1;
    }
    else if (direction == SOUTHEAST)
    {
        dest.col = src.col + 1;
        dest.row = src.row + 1;
    }
    else if (direction == SOUTHWEST)
    {
        dest.col = src.col - 1;
        dest.row = src.row + 1;
    }
    else
    {
        dest.col = src.col - 1;
        dest.row = src.row - 1;
    }
    return dest;
}

/* Convert `src` and `tar` into a string corresponding to the action, then
   save the string into `action` */
void get_action_name(locn_t src, locn_t target, char action[])
{
    action[SOURCE_COL] = src.col + ALPHA_TO_INT;
    action[SOURCE_ROW] = src.row + NUM_TO_INT;
    action[2] = '-';
    action[TARGET_COL] = target.col + ALPHA_TO_INT;
    action[TARGET_ROW] = target.row + NUM_TO_INT;
    action[MOVELEN] = '\0';
}

char change_player(char color)
{
    if (color == CELL_BPIECE)
    {
        return CELL_WPIECE;
    }
    else
    {
        return CELL_BPIECE;
    }
}

/* Prints error codes and terminates program */
void print_error(int err)
{
    if (err == ERROR1)
    {
        printf("ERROR: Source cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    }
    else if (err == ERROR2)
    {
        printf("ERROR: Target cell is outside of the board.\n");
        exit(EXIT_FAILURE);
    }
    else if (err == ERROR3)
    {
        printf("ERROR: Source cell is empty.\n");
        exit(EXIT_FAILURE);
    }
    else if (err == ERROR4)
    {
        printf("ERROR: Target cell is not empty.\n");
        exit(EXIT_FAILURE);
    }
    else if (err == ERROR5)
    {
        printf("ERROR: Source cell holds opponent's piece/tower.\n");
        exit(EXIT_FAILURE);
    }
    else if (err == ERROR6)
    {
        printf("ERROR: Illegal action.\n");
        exit(EXIT_FAILURE);
    }
}

/* Prints all the required information for a move - action, cost, board */
void print_move(board_t b, int count, char *action, char player, int comp)
{
    int cost;
    cost = calculate_cost(b);
    printf("%s\n", DELIMITER);
    if (comp)
    { // Computed actions need to have "***" before action
        printf("*** ");
    }
    if (player == CELL_BPIECE)
    {
        printf("BLACK ");
    }
    else if (player == CELL_WPIECE)
    {
        printf("WHITE ");
    }
    printf("ACTION #%d: %s\n", count, action);
    printf("BOARD COST: %d\n", cost);
    print_board(b);
}
