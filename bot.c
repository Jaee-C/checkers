#include "board.h"

void bot_move(board_t board, int* count, char* player) {
    // Computer's move
    system("clear");
    perform_next_action(board, *player, *count);
    *player = change_player(*player);   // After every move, change player
}
