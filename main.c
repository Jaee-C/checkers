#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "board.h"

int main(int argc, char *argv[]) {
    system("clear");
    board_t board;
    locn_t source, target;
    int num_players = get_player_num();
    char action[MOVELEN+1], player;
    int input_len, count = 0, error = 0, level=1;

    if (num_players == 1) {
        player = set_player();
        level = set_level();
    }

    system("clear");
    board_init(board);

    if (num_players == 1) {
        // Makes sure that black goes first
        if (player == CELL_WPIECE) {
            player = CELL_BPIECE;
            count++;
            bot_move(board, &count, &player, level);
        }
    } else {
        player = CELL_BPIECE;
    }

    while ((input_len = get_input(action, player)) != EOF && input_len == MOVELEN) {
        // loop stops when non-move input ('A' or 'P') is read
        // Player's move
        system("clear");
        count++;

        process_input(action, &source, &target);
        error = check_error(board, source, target, player);
        print_error(error);

        update_board(board, source, target);

        print_move(board, count, action, player, INPUT);
        player = change_player(player);   // After every move, change player

        if (num_players == 1) {
            // Bot plays in single player mode
            count++;
            bot_move(board, &count, &player, level);
        }
    }

    if (input_len == 1 && action[0] == 'A') {
        // Get one action
        count++;
        perform_next_action(board, player, count, level);
    } else if (input_len == 1 && action[0] == 'P') {
        // Get next COMP_ACTIONS actions
        for (int i = 0; i < COMP_ACTIONS; i++) {
            count++;
            perform_next_action(board, player, count, level);
            player = change_player(player);
        }
    }

    return EXIT_SUCCESS;  // exit program with the success code
}

/* THE END -------------------------------------------------------------------*/
// algorithms are fun
