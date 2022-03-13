#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "board.h"

int main(int argc, char *argv[]) {
    board_t board;
    locn_t source, target;
    char action[MOVELEN+1], player = CELL_BPIECE;
    int input_len, count = 0, error = 0;

    system("clear");
    board_init(board);

    while ((input_len = get_input(action, player)) != EOF && input_len == MOVELEN) {
        // loop stops when non-move input ('A' or 'P') is read
        // Player's move
        count++;

        process_input(action, &source, &target);
        error = check_error(board, source, target, player);
        print_error(error);

        update_board(board, source, target);

        print_move(board, count, action, player, INPUT);
        player = change_player(player);   // After every move, change player
        system("clear");
        count++;
        bot_move(board, &count, &player);
    }

    if (input_len == 1 && action[0] == 'A') {
        // Get one action
        count++;
        perform_next_action(board, player, count);
    } else if (input_len == 1 && action[0] == 'P') {
        // Get next COMP_ACTIONS actions
        for (int i = 0; i < COMP_ACTIONS; i++) {
            count++;
            perform_next_action(board, player, count);
            player = change_player(player);
        }
    }

    return EXIT_SUCCESS;  // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/
// algorithms are fun
